#pragma once

#include <internal/values/abstract_config_value.hpp>

namespace hocon {

    class mergeable_value : public config_mergeable {
    public:
        /** Converts a config to its root object and a config_value to itself. */
        virtual shared_value to_fallback_value() const = 0;
    };


}  // namespace hocon
