#include <internal/nodes/config_node_comment.hpp>
#include <internal/tokens.hpp>

using namespace std;

namespace hocon {

    config_node_comment::config_node_comment(shared_ptr<token> comment) :
            config_node_single_token(move(comment)) { }

    string config_node_comment::comment_text() const {
        return dynamic_pointer_cast<comment>(get_token())->text();
    }

}  // namespace hocon
