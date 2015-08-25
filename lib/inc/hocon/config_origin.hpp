#pragma once

namespace hocon {

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
         * Returns a line number where the value or exception originated. This will
         * return -1 if there's no meaningful line number.
         *
         * @return line number or -1 if none is available
         */
         virtual int lineNumber() const = 0;
    };

}  // namespace hocon
