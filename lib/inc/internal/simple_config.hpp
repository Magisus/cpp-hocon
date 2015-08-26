#pragma once

#include <hocon/config.hpp>
#include <internal/mergeable_value.hpp>
#include <internal/path.hpp>
#include <internal/objects/abstract_config_object.hpp>
#include <memory>

namespace hocon {

    class simple_config : public config, public mergeable_value, public std::enable_shared_from_this<simple_config> {
    public:
        simple_config(std::shared_ptr<abstract_config_object> object);

        std::shared_ptr<const config_object> root() const override;
        shared_origin origin() const override;
        shared_config resolve() const override;
        shared_config resolve(config_resolve_options options) const override;
        shared_config resolve_with(shared_config source) const override;
        shared_config resolve_with(shared_config source, config_resolve_options options) const override;
        bool has_path(std::string path_expression) const override;
        bool has_path_or_null(std::string path) const override;
        bool is_empty() const override;
        std::set<std::pair<std::string, std::shared_ptr<const config_value>>> entry_set() const override;
        bool get_is_null(std::string path) const override;
        std::shared_ptr<config_mergeable> with_fallback(std::shared_ptr<config_mergeable> other) const override;
        shared_value to_fallback_value() const override;
        bool is_resolved() const override;
        void check_valid(shared_config reference, std::vector<std::string> restrict_to_paths) const override;
        shared_config with_only_path(std::string path_expression) const override;
        shared_config without_path(std::string path_expression) const override;
        shared_config with_value(std::string path_expression, std::shared_ptr<const config_value> v) const override;
        shared_config at_key(std::string key) const override;
        shared_config at_path(std::string path) const override;

        bool get_bool(std::string path) const override;
        int get_int(std::string path) const override;
        int get_long(std::string path) const override;
        double get_double(std::string path) const override;
        std::string get_string(std::string path) const override;
        std::shared_ptr<const config_object> get_object(std::string path) const override;
        shared_config get_config(std::string path) const override;
        std::shared_ptr<const config_value> get_value(std::string path) const override;
        std::vector<bool> get_bool_list(std::string path) const override;
        std::vector<int> get_int_list(std::string path) const override;
        std::vector<int64_t> get_long_list(std::string path) const override;
        std::vector<double> get_double_list(std::string path) const override;
        std::vector<std::string> get_string_list(std::string path) const override;
        std::vector<std::shared_ptr<const config_object>> get_object_list(std::string path) const override;
        std::vector<shared_config> get_config_list(std::string path) const override;

        bool operator==(simple_config const& other) const;

        shared_value find(path path_expression, config_value_type expected, path original_path) const;
        shared_value find(path path_expression, config_value_type expected) const;
        shared_config at_key(shared_origin origin, std::string key) const;

        // TODO: memory and duration parsing

    private:
        shared_value has_path_peek(std::string path_expression) const;
        shared_value peek_path(path desired_path) const;

        static void find_paths(std::set<std::pair<std::string, std::shared_ptr<const config_value>>>& entries,
                               path parent, shared_object obj);
        static shared_value throw_if_null(shared_value v, config_value_type expected, path original_path);
        static shared_value find_key(shared_object self, std::string key,
                                     config_value_type expected, path original_path);
        static shared_value find_key_or_null(shared_object self, std::string key,
                                             config_value_type expected, path original_path);
        static shared_value find_or_null(shared_object self, path desired_path,
                                         config_value_type expected, path original_path);
        static shared_value find_or_null(path path_expression, config_value_type expected, path original_path);
        static shared_value find_or_null(std::string path_expression, config_value_type expected);

        shared_object _object;
    };


}  // namespace hocon
