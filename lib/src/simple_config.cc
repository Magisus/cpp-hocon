#include <internal/simple_config.hpp>
#include <internal/values/config_null.hpp>
#include <internal/config_exception.hpp>
#include <internal/default_transformer.hpp>

using namespace std;

namespace hocon {

    simple_config::simple_config(shared_ptr<abstract_config_object> object) : _object(move(object)) { }

    shared_ptr<const config_object> simple_config::root() const {
        return _object;
    }

    shared_origin simple_config::origin() const {
        return _object->origin();
    }

    shared_config simple_config::resolve() const {
        return resolve(config_resolve_options());
    }

    shared_config simple_config::resolve(config_resolve_options options) const {
        return resolve_with(shared_from_this(), options);
    }

    shared_config simple_config::resolve_with(shared_config source) const {
        return resolve_with(source, config_resolve_options());
    }

    shared_config simple_config::resolve_with(shared_config source, config_resolve_options options) const {
        // TODO: implement all the resolve classes, then this method
        // until then DO NOT TRY TO PARSE INPUT WITH SUBSTITUTIONS
        return shared_from_this();
    }

    shared_value simple_config::has_path_peek(string path_expression) const {
        path raw_path = path::new_path(path_expression);
        shared_value peeked;
        try {
            peeked = _object->peek_path(raw_path);
        } catch (config_exception& ex) {
            throw config_exception(raw_path.render() + " has not been resolved, you need to call config::resolve()");
        }
        return peeked;
    }

    bool simple_config::has_path(string path_expression) const {
        shared_value peeked = has_path_peek(path_expression);
        return peeked && peeked->value_type() != config_value_type::CONFIG_NULL;
    }

    bool simple_config::has_path_or_null(string path) const {
        shared_value peeked = has_path_peek(path);
        return peeked != nullptr;
    }

    bool simple_config::is_empty() const {
        return _object->is_empty();
    }

    void simple_config::find_paths(set<pair<string, shared_ptr<const config_value>>>& entries, path parent,
                                   shared_object obj) {
        for (auto&& entry : obj->entry_set()) {
            string elem = entry.first;
            shared_value v = entry.second;
            path new_path = path::new_key(elem);
            if (!parent.empty()) {
                new_path = new_path.prepend(parent);
            }
            if (auto object = dynamic_pointer_cast<const abstract_config_object>(v)) {
                find_paths(entries, new_path, object);
            } else if (auto null_value = dynamic_pointer_cast<const config_null>(v)) {
                // nothing; nulls are conceptually not in a config
            } else {
                entries.insert(make_pair(new_path.render(), v));
            }
        }
    }

    set<pair<string, shared_ptr<const config_value>>> simple_config::entry_set() const {
        set<pair<string, shared_ptr<const config_value>>> entries;
        find_paths(entries, path(), _object);
        return entries;
    }

    shared_value simple_config::throw_if_null(shared_value v, config_value_type expected, path original_path) {
        if (v->value_type() == config_value_type::CONFIG_NULL) {
            throw config_exception(original_path.render() + " was null");
        } else {
            return v;
        }
    }

    shared_value simple_config::find_key(shared_object self, string key, config_value_type expected,
                                         path original_path) {
        return throw_if_null(find_key_or_null(self, key, expected, original_path), expected, original_path);
    }

    shared_value simple_config::find_key_or_null(shared_object self, string key, config_value_type expected,
                                                 path original_path) {
        shared_value v = self->peek_assuming_resolved(key, original_path);
        if (!v) {
            throw config_exception("Value missing at " + original_path.render());
        }

        if (expected != config_value_type::UNSPECIFIED) {
            v = default_transformer::transform(v, expected);
        }

        if (expected != config_value_type::UNSPECIFIED &&
                v->value_type() != expected &&
                v->value_type() != config_value_type::CONFIG_NULL) {
            throw config_exception(original_path.render() + " could not be converted to the requested type");
        } else {
            return v;
        }
    }

    shared_value simple_config::find_or_null(shared_object self, path desired_path,
                                             config_value_type expected, path original_path) {
        try {
            string key = *desired_path.first();
            path next = desired_path.remainder();
            if (next.empty()) {
                return find_key_or_null(self, key, expected, original_path);
            } else {
                shared_object o = dynamic_pointer_cast<const abstract_config_object>(
                        find_key(self, key, config_value_type::OBJECT,
                                 original_path.sub_path(0, original_path.length() - next.length())));
                return find_or_null(o, next, expected, original_path);
            }
        } catch (config_exception& ex) {
            throw config_exception(desired_path.render() +
                                           " has not been resolved, you need to call config::resolve()");
        }
    }

}  // namespace hocon
