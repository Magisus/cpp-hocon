#pragma once

namespace hocon {

    class config_exception : public std::runtime_error {
    public:
        config_exception(std::string const& message) : runtime_error(message) { }
    };

}  // namespace hocon
