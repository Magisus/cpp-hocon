#pragma once

#include <string>

namespace hocon {

    bool is_whitespace(char codepoint);

    bool is_whitespace_not_newline(char codepoint);

    bool is_C0_control(char c);

    std::string render_json_string(std::string s);

}  // namespace hocon
