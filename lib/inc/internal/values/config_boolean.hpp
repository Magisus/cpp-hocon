#pragma once

#include "abstract_config_value.hpp"

namespace hocon {

    class config_boolean : public abstract_config_value {
    public:
        config_boolean(shared_origin origin, bool value);

        config_value_type value_type() const override;
        std::string transform_to_string() const override;

        bool bool_value() const;

    private:
        bool _value;
    };
}  // namespace hocon;

