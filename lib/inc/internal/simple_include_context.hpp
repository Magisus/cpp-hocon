#pragma once

#include <hocon/config_include_context.hpp>
#include "parseable.hpp"

namespace hocon {

    class simple_include_context : public config_include_context {
    public:
        simple_include_context(std::shared_ptr<parseable> parseable);

        std::shared_ptr<simple_include_context> with_parseable(std::shared_ptr<parseable> new_parseable);

        std::shared_ptr<config_parseable> relative_to(std::string file_name) override;
        config_parse_options parse_options() override;

    private:
        std::shared_ptr<parseable> _parseable;
    };
}  // namespace hocon
