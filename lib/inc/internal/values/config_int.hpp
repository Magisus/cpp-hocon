#pragma once

#include "config_number.hpp"

namespace hocon {

    class config_int : public config_number {
    public:
        config_int(std::shared_ptr<simple_config_origin> origin,
                    int value, std::string original_text);

        std::string transform_to_string() const override;

        long long_value() const override;
        double double_value() const override;

    private:
        int _value;
    };

}
