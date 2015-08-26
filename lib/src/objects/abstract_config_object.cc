#include <internal/objects/abstract_config_object.hpp>
#include <internal/simple_config.hpp>
#include <internal/config_exception.hpp>

using namespace std;

namespace hocon {

    abstract_config_object::abstract_config_object(shared_origin origin) : abstract_config_value(move(origin)),
            _config(make_shared<simple_config>(shared_from_this())) { }

    shared_value abstract_config_object::peek_path(path desired_path) const {
        return peek_path(this, move(desired_path));
    }

    shared_value abstract_config_object::peek_path(const abstract_config_object* self, path desired_path) {
        try {
            path next = desired_path.remainder();
            shared_value v = self->attempt_peek_with_partial_resolve(*desired_path.first());

            if (next.empty()) {
                return v;
            } else {
                if (auto object = dynamic_pointer_cast<const abstract_config_object>(v)) {
                    return peek_path(object.get(), next);
                } else {
                    return nullptr;
                }
            }
        } catch (config_exception& ex) {
            throw config_exception(desired_path.render() +
                                           " has not been resolved, you need to call config::resolve()");
        }
    }

    shared_value abstract_config_object::peek_assuming_resolved(std::string const& key, path original_path) const {
        try {
            return attempt_peek_with_partial_resolve(key);
        } catch (config_exception& ex) {
            throw config_exception(original_path.render() +
                                           " has not been resolved, you need to call config::resolve()");
        }
    }

}  // namespace hocon
