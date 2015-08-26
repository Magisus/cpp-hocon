#include <internal/objects/simple_config_object.hpp>
#include <internal/values/abstract_config_value.hpp>

using namespace std;

namespace hocon {

    simple_config_object::simple_config_object(shared_origin origin,
                                               unordered_map <std::string, shared_value> value,
                                               resolved_status status, bool ignores_fallbacks) :
        abstract_config_object(move(origin)), _value(move(value)) { }

        shared_value simple_config_object::attempt_peek_with_partial_resolve(std::string const& key) const {
            return _value.at(key);
        }

    bool simple_config_object::is_empty() const {
        return _value.empty();
    }

    unordered_map<string, shared_value> const& simple_config_object::entry_set() const {
        return _value;
    }

}  // namespace hocon
