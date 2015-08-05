#include <internal/values/config_null.hpp>

using namespace std;

namespace hocon {

    config_null::config_null(shared_ptr<simple_config_origin> origin) :
            abstract_config_value(origin) { }

    config_value_type config_null::value_type() const {
        return config_value_type::CONFIG_NULL;
    }

    string config_null::transform_to_string() const {
        return "null";
    }

}  // namespace hocon
