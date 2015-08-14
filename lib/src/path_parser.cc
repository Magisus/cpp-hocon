#include <internal/path_parser.hpp>
#include <internal/tokens.hpp>
#include <sstream>
#include <internal/values/config_string.hpp>

#include <boost/algorithm/string/trim.hpp>

using namespace std;

namespace hocon {

    /** Element */
    path_parser::element::element(string initial, bool can_be_empty) : _value(move(initial)),
                                                                       _can_be_empty(can_be_empty) { }

    string path_parser::element::to_string() const {
        return "Element(" + _value + "," + std::to_string(_can_be_empty) + ")";
    }

    /** Path parser */
    simple_config_origin path_parser::api_origin = simple_config_origin("path parameter");

    config_node_path path_parser::parse_path_node(string const& path_string) {
        return parse_path_node(move(path_string), config_syntax::CONF);
    }

    config_node_path path_parser::parse_path_node(string const& path_string, config_syntax flavor) {
        token_iterator tokens = token_iterator(api_origin,
                                               unique_ptr<istringstream>(new istringstream(path_string)),
                                               (flavor != config_syntax::JSON));

        tokens.next();  // drop start token
        return parse_path_node_expression(tokens, api_origin, path_string, flavor);
    }

    path path_parser::parse_path(string const& path_string) {
        path speculated = speculative_fast_parse_path(path_string);
        if (speculated != path()) {
            return speculated;
        }

        token_iterator tokens = token_iterator(api_origin,
                                               unique_ptr<istringstream>(new istringstream(path_string)),
                                               true);
        tokens.next();  // drop start token
        return parse_path_expression(tokens, api_origin, path_string);
    }

    path path_parser::parse_path_expression(token_iterator& expression, simple_config_origin origin,
                                            string const& original_text) {
        return parse_path_expression(expression, origin, original_text, nullptr, config_syntax::CONF);
    }

    path path_parser::parse_path_expression(token_iterator& expression, simple_config_origin origin) {
        return parse_path_expression(expression, origin, "", nullptr, config_syntax::CONF);
    }

    config_node_path path_parser::parse_path_node_expression(token_iterator& expression,
                                                             simple_config_origin origin) {
        return parse_path_node_expression(expression, origin, "", config_syntax::CONF);
    }

    config_node_path path_parser::parse_path_node_expression(token_iterator& expression,
                                                             simple_config_origin origin,
                                                             string const& original_text,
                                                             config_syntax flavor)
    {
        token_list tokens;
        path path = parse_path_expression(expression, origin, original_text, &tokens, flavor);
        return config_node_path(path, tokens);
    }

    path path_parser::parse_path_expression(token_iterator& expression, simple_config_origin origin,
                                            string const& original_text, token_list* path_tokens,
                                            config_syntax flavor)
    {
        vector<element> elements = { element("", false) };

        if (!expression.has_next()) {
            throw config_exception("Invalid path: " + original_text +
                                           " -- Expecting a field name or path here, but got nothing");
        }

        while (expression.has_next()) {
            shared_token t = expression.next();

            if (path_tokens != nullptr) {
                path_tokens->push_back(t);
            }

            if (t->get_token_type() == token_type::IGNORED_WHITESPACE) {
                continue;
            }

            if (tokens::is_value_with_type(t, config_value_type::STRING)) {
                auto value = tokens::get_value(t);
                // this is a quoted string, so any periods in here don't count as path separators
                string s = value->transform_to_string();
                add_path_text(elements, true, s);
            } else if (t == tokens::end_token()) {
                // ignore this; when parsing a file, it should not happen
                // since we're parsing a token list rather than the main
                // token iterator, and when parsing a path expression from the
                // API, it's expected to have an END.
            } else {
                // any periods outside of a quoted string count as separators
                string text;
                if (t->get_token_type() == token_type::VALUE) {
                    // appending a number here may add a period, but we _do_ count those as path
                    // separators, because we basically want "foo 3.0bar" to parse as a string even
                    // though there's a number in it. The fact that we tokenize non-string values
                    // is largely an implementation detail.
                    auto value = tokens::get_value(t);

                    // We need to split the tokens on a . so that we can get sub-paths but still preserve
                    // the original path text when doing an insertion
                    if (path_tokens != nullptr) {
                        path_tokens->pop_back();
                        for (auto&& token : split_token_on_period(t, flavor)) {
                            path_tokens->push_back(token);
                        }
                    }
                    text = value->transform_to_string();
                } else if (t->get_token_type() == token_type::UNQUOTED_TEXT) {
                    // We need to split the tokens on a . so that we can get sub-paths but still preserve
                    // the original path text when doing an insertion on ConfigNodeObjects
                    if (path_tokens != nullptr) {
                        path_tokens->pop_back();
                        for (auto&& token : split_token_on_period(t, flavor)) {
                            path_tokens->push_back(token);
                        }
                    }
                    text = t->token_text();
                } else {
                    throw config_exception("Invalid path: " + original_text +
                                           " -- Token not allowed in path expression: " +
                                           t->to_string() +
                                           " (you can double-quote this token if you really want it here)");
                }

                add_path_text(elements, false, text);
            }
        }

        path_builder builder;
        for (element e : elements) {
            if (e._value.length() == 0 && !e._can_be_empty) {
                throw config_exception("Invalid path: " + original_text +
                                               " -- path has a leading, trailing, or two adjacent '.'" +
                " (use quoted \"\" empty string if you want an empty element)");
            } else {
                builder.append_key(e._value);
            }
        }

        return builder.result();
    }

    token_list path_parser::split_token_on_period(shared_token t, config_syntax flavor) {
        string token_text = t->token_text();
        if (token_text == ".") {
            return token_list { t };
        }

        vector<string> token_strings;
        size_t next = 0;
        size_t last = 0;
        while ((next = token_text.find(".")) != string::npos) {
            string sub = token_text.substr(last, next-last);
            token_strings.push_back(sub);
            last = next + 1;
        }
        token_strings.push_back(token_text.substr(last));

        token_list split_tokens;
        for (string s : token_strings) {
            if (flavor == config_syntax::CONF) {
                split_tokens.push_back(make_shared<unquoted_text>(t->origin(), s));
            } else {
                split_tokens.push_back(make_shared<value>(
                        unique_ptr<config_string>(new config_string(
                                t->origin(), "\"" + s + "\"", config_string_type::UNQUOTED))));
            }
            split_tokens.push_back(make_shared<unquoted_text>(t->origin(), "."));
        }
        if (token_text[token_text.length() - 1] != '.') {
            split_tokens.pop_back();
        }
        return split_tokens;
    }

    void path_parser::add_path_text(std::vector<element>& buff, bool was_quoted, std::string new_text) {
        unsigned long i = was_quoted ? string::npos : new_text.find('.');
        element& current = buff.back();
        if (i == string::npos) {
            // add the current path element
            current._value += new_text;
            // any empty quoted string means this element can now be empty
            if (was_quoted && current._value.empty()) {
                current._can_be_empty = true;
            }
        } else {
            // buff plus up to the period is an element
            current._value += new_text.substr(0, i);
            // then start a new element
            buff.push_back(element("", false));
            // recurse to consume the remainder of new_text
            add_path_text(buff, false, new_text.substr(i + 1));
        }
    }

    bool path_parser::looks_unsafe_for_fast_parser(std::string s) {
        bool last_was_dot = true;  // start of a path is also a "dot"
        size_t len = s.length();
        if (s.empty()) {
            return true;
        }
        if (s[0] == '.') {
            return true;
        }
        if (s[len - 1] == '.') {
            return true;
        }

        for (size_t i = 0; i < len; i++) {
            char c = s[i];
            if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_') {
                last_was_dot = false;
                continue;
            } else if (c == '.') {
                if (last_was_dot) {
                    return true;  // ".." means we need to throw an error
                }
                last_was_dot = true;
            } else if (c == '-') {
                if (last_was_dot) {
                    return true;
                }
                continue;
            } else {
                return true;
            }
        }

        return last_was_dot;
    }

    path path_parser::fast_path_build(path tail, string s) {
        size_t split_at = s.find_last_of('.');
        token_list tokens;
        tokens.push_back(make_shared<unquoted_text>(nullptr, s));
        path with_one_more_element = path(s.substr(split_at + 1), tail);
        if (split_at == string::npos) {
            return with_one_more_element;
        } else {
            return fast_path_build(with_one_more_element, s.substr(0, split_at));
        }
    }

    path path_parser::speculative_fast_parse_path(std::string const& path_string) {
        string s = path_string;
        boost::trim(s);
        if (looks_unsafe_for_fast_parser(s)) {
            return path { };
        }

        return fast_path_build(path(), s);
    }

}  // namespace hocon
