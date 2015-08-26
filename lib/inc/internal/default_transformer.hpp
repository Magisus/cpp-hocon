#pragma once

#include <internal/values/abstract_config_value.hpp>

namespace hocon { namespace default_transformer {

    shared_value transform(shared_value value, config_value_type requested);

}}  // namespace hocon::default_transformer