#pragma once

#include "abstract_config_node_value.hpp"
#include "config_node_path.hpp"


namespace hocon {

    class config_node_field : public abstract_config_node_value {
    public:
        config_node_field(std::vector<std::shared_ptr<abstract_config_node>> children);

        token_list get_tokens() const override;

        std::shared_ptr<config_node_field> replace_value(std::shared_ptr<abstract_config_node_value> new_value);
        std::shared_ptr<abstract_config_node_value> get_value() const;
        shared_token separator() const;
        std::vector<std::string> comments() const;
        std::shared_ptr<config_node_path> path() const;


    private:
        std::vector<std::shared_ptr<abstract_config_node>> _children;
    };

}  // namespace hocon
