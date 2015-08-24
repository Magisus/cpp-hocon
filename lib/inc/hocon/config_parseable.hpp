#pragma once

#include "config_parse_options.hpp"
#include "config_object.hpp"
#include <memory>

namespace hocon {

    /**
     * An opaque handle to something that can be parsed, obtained from
     * {@link config_include_context}.
     *
     * <p>
     * <em>Do not implement this interface</em>; it should only be implemented by
     * the config library. Arbitrary implementations will not work because the
     * library internals assume a specific concrete implementation. Also, this
     * interface is likely to grow new methods over time, so third-party
     * implementations will break.
     */
    class config_parseable {

        /**
         * Parse whatever it is. The options should come from
         * {@link ConfigParseable#options options()} but you could tweak them if you
         * like.
         *
         * @param options
         *            parse options, should be based on the ones from
         *            {@link ConfigParseable#options options()}
         * @return the parsed object
         */
        std::shared_ptr<config_object> parse(config_parse_options options);

        std::shared_ptr<config_origin>
    };

}  // namespace hocon
