#pragma once

#include "abstract_config_object.hpp"

namespace hocon {

    class simple_config_object : public abstract_config_object {
    public:
        simple_config_object(shared_origin origin, std::unordered_map<std::string, shared_value> value,
                             resolved_status status = resolved_status::RESOLVED, bool ignores_fallbacks = false);

        shared_value attempt_peek_with_partial_resolve(std::string const& key) const override;

        bool is_empty() const override;
        std::unordered_map<std::string, shared_value> const& entry_set() const override;

    private:
        std::unordered_map<std::string, shared_value> _value;
        // TODO: Put these back when needed, currently cause an unused error
//        bool _resolved;
//        bool _ignores_fallbacks;
    };

}  // namespace hocon
