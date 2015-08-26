#pragma once

#include <string>

namespace hocon {

    class config_origin;

    using shared_origin = std::shared_ptr<const config_origin>;

    /**
     * Represents the origin (such as filename and line number) of a
     * {@link config_value} for use in error messages. Obtain the origin of a value
     * with {@link config_value#origin}. Exceptions may have an origin, see
     * {@link config_exception#origin}, but be careful because
     * <code>config_exception.origin()</code> may return null.
     *
     * <p>
     * It's best to use this interface only for debugging; its accuracy is
     * "best effort" rather than guaranteed, and a potentially-noticeable amount of
     * memory could probably be saved if origins were not kept around, so in the
     * future there might be some option to discard origins.
     *
     * <p>
     * <em>Do not implement this interface</em>; it should only be implemented by
     * the config library. Arbitrary implementations will not work because the
     * library internals assume a specific concrete implementation. Also, this
     * interface is likely to grow new methods over time, so third-party
     * implementations will break.
     */
    class config_origin {
    public:
        /**
         * Returns a string describing the origin of a value or exception. This will
         * never return null.
         *
         * @return string describing the origin
         */
        virtual std::string description() const = 0;

        /**
         * Returns a {@code ConfigOrigin} based on this one, but with the given
         * line number. This origin must be a FILE, URL or RESOURCE. Does not modify
         * this instance or any {@code ConfigValue}s with this origin (since they are
         * immutable).  To set the returned origin to a  {@code ConfigValue}, use
         * {@link ConfigValue#withOrigin}.
         *
         * <p>
         * Note that when the given lineNumber are equal to the lineNumber on this
         * object, a new instance may not be created and {@code this} is returned
         * directly.
         *
         * @since 1.3.0
         *
         * @param lineNumber the new line number
         * @return the created ConfigOrigin
         */
        virtual shared_origin with_line_number(int line_number) const = 0;

        /**
         * Returns a line number where the value or exception originated. This will
         * return -1 if there's no meaningful line number.
         *
         * @return line number or -1 if none is available
         */
        virtual int line_number() const = 0;

    };

}  // namespace hocon
