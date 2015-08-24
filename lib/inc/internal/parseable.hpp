#pragma once

#include <hocon/config_parseable.hpp>
#include <boost/nowide/fstream.hpp>
#include <internal/simple_config_origin.hpp>

namespace hocon {

    class parseable_file;
    class parseable_string;

    class parseable : public config_parseable {
    public:
        static parseable_file new_file(std::string input_file_path, shared_parse_options options);

        static parseable_string new_string(std::string s, shared_parse_options options);

        void post_construct(shared_parse_options base_options);

        std::shared_ptr<config_document> parse_config_document();

        virtual std::unique_ptr<std::istream> reader() = 0;
        virtual shared_origin create_origin() = 0;
        virtual config_syntax guess_syntax();

    private:
        std::shared_ptr<config_document> parse_document(shared_parse_options base_options);
        std::shared_ptr<config_document> parse_document(shared_origin origin, shared_parse_options final_options);
        config_document raw_parse_document(std::istream stream, shared_origin origin,
                                           shared_parse_options options);
        std::shared_ptr<config_document> raw_parse_document(shared_origin origin,
                                                            shared_parse_options options);

        shared_parse_options fixup_options(shared_parse_options base_options);

        shared_origin _initial_origin;
        std::shared_ptr<config_parse_options> _initial_options;
        // TODO: implement include_context
    };

    class parseable_file : public parseable {
    public:
        parseable_file(std::string input_file_path, shared_parse_options options);
        std::unique_ptr<std::istream> reader() override;
        shared_origin create_origin() override;
        config_syntax guess_syntax() override;

    private:
        std::string _input;
    };

    class parseable_string : public parseable {
    public:
        parseable_string(std::string s, shared_parse_options options);
        std::unique_ptr<std::istream> reader() override;
        shared_origin create_origin() override;

    private:
        std::string _input;
    };

}  // namespace hocon
