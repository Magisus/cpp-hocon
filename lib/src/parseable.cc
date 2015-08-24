#include <internal/parseable.hpp>
#include <sstream>
#include <boost/algorithm/string/predicate.hpp>
#include <internal/nodes/abstract_config_node.hpp>
#include <internal/nodes/config_node_object.hpp>
#include <internal/simple_config_document.hpp>
#include <internal/config_exception.hpp>

using namespace std;

namespace hocon {

    parseable_file parseable::new_file(std::string input_file_path, shared_parse_options options) {
        return parseable_file(move(input_file_path),  move(options));
    }

    parseable_string parseable::new_string(std::string s, shared_parse_options options) {
        return parseable_string(move(s), move(options));
    }

    void parseable::post_construct(shared_parse_options base_options) {
        _initial_options = fixup_options(base_options);

        // TODO: set include context

        if (!_initial_options->get_origin_description()->empty()) {
            _initial_origin = make_shared<simple_config_origin>(*_initial_options->get_origin_description());
        } else {
            _initial_origin = create_origin();
        }
    }

    shared_parse_options parseable::fixup_options(shared_parse_options base_options) {
        config_syntax syntax = base_options->get_syntax();
        if (syntax == config_syntax::UNSPECIFIED) {
            syntax = guess_syntax();
        }
        if (syntax == config_syntax::UNSPECIFIED) {
            syntax = config_syntax::CONF;
        }
        config_parse_options modified = base_options->set_syntax(syntax);

        // TODO: add include stuff
        return make_shared<config_parse_options>(modified);
    }

    config_syntax parseable::guess_syntax() {
        return config_syntax::UNSPECIFIED;
    }

    shared_ptr<config_document> parseable::parse_config_document() {
        return parse_document(_initial_options);
    }

    shared_ptr<config_document> parseable::parse_document(shared_parse_options base_options) {
        // note that we are NOT using our "initialOptions",
        // but using the ones from the passed-in options. The idea is that
        // callers can get our original options and then parse with different
        // ones if they want.
        shared_parse_options options = fixup_options(base_options);

        // passed in options can override origin
        shared_origin origin = _initial_origin;
        if (!options->get_origin_description()->empty()) {
            origin = make_shared<simple_config_origin>(*options->get_origin_description());
        }
        return parse_document(origin, move(options));
    }

    std::shared_ptr<config_document> parseable::parse_document(shared_origin origin,
                                                               shared_parse_options final_options) {
        try {
            return raw_parse_document(origin, final_options);
        } catch (runtime_error& e) {
            if (final_options->get_allow_missing()) {
                shared_node_list children;
                children.push_back(make_shared<config_node_object>(shared_node_list { }));
                return make_shared<simple_config_document>(make_shared<config_node_root>(children, origin),
                                                           final_options);
            } else {
                throw config_exception("exception loading " + origin->description() + ": " + e.what());
            }
        }
    }

    std::shared_ptr<config_document> parseable::raw_parse_document(shared_origin origin,
                                                                   shared_parse_options options) {
        istream stream = reader(options);
        

    }

    /** Parseable file */
    parseable_file::parseable_file(std::string input_file_path, shared_parse_options options) :
        _input(move(input_file_path)) {
        post_construct(options);
    }

    unique_ptr<istream> parseable_file::reader() {
        return unique_ptr<istream>(new boost::nowide::ifstream(_input));
    }

    shared_origin parseable_file::create_origin() {
        return make_shared<simple_config_origin>("file: " + _input);
    }

    config_syntax parseable_file::guess_syntax() {
        if (boost::algorithm::ends_with(_input, ".json")) {
            return config_syntax::JSON;
        } else if (boost::algorithm::ends_with(_input, ".conf")) {
            return config_syntax::CONF;
        } else {
            return config_syntax::UNSPECIFIED;
        }
    }

    /** Parseable string */
    parseable_string::parseable_string(std::string s, shared_parse_options options) : _input(move(s)) {
        post_construct(options);
    }

    unique_ptr<istream> parseable_string::reader() {
        return unique_ptr<istringstream>(new istringstream(_input));
    }

    shared_origin parseable_string::create_origin() {
        return make_shared<simple_config_origin>("string");
    }
}  // namespace hocon
