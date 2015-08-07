#pragma once

#include "config_number.hpp"
#include <internal/simple_config_origin.hpp>

#include <string>

namespace hocon {

    class config_long : public config_number {
    public:
        config_long(std::shared_ptr<simple_config_origin> origin,
                    int64_t value, std::string original_text);

        std::string transform_to_string() const override;

        int64_t long_value() const override;
        double double_value() const override;

    private:
        int _value;
    };

}  // namespace hocon
