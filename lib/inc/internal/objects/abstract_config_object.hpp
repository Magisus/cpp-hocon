#pragma once

#include <internal/values/abstract_config_value.hpp>
#include <hocon/config_object.hpp>
#include <hocon/config.hpp>
#include <internal/path.hpp>

#include <unordered_map>

namespace hocon {

    class abstract_config_object : public abstract_config_value, public config_object,
                                   std::enable_shared_from_this<abstract_config_object> {
    public:
        abstract_config_object(shared_origin origin);

        virtual shared_value attempt_peek_with_partial_resolve(std::string const& key) const = 0;
        shared_value peek_path(path desired_path) const;
        shared_value peek_assuming_resolved(std::string const& key, path original_path) const;

        virtual bool is_empty() const = 0;
        virtual std::unordered_map<std::string, shared_value> const& entry_set() const = 0;

        static shared_value peek_path(const abstract_config_object* self, path desired_path);

    private:
        shared_config _config;
    };

    using shared_object = std::shared_ptr<const abstract_config_object>;
}  //  namespace hocon
