#pragma once

#include "config_document.hpp"
#include <hocon/config_parse_options.hpp>

/**
 * Factory for creating config_document instances.
 */
namespace hocon { namespace config_document_factory {

    /**
     * Parses a stream into a config_document instance.
     *
     * @param stream
     *       the stream to parse
     * @param options
     *       parse options to control how the reader is interpreted
     * @return the parsed configuration
     */
    std::shared_ptr<config_document> parse_stream(std::unique_ptr<std::istream> stream, config_parse_options options);

    /** Parses a stream into a config_document instance using default options. */
    std::shared_ptr<config_document> parse_stream(std::unique_ptr<std::istream> stream);

        /**
         * Parses a file into a config_document instance.
         *
         * @param file
         *       the file to parse
         * @param options
         *       parse options to control how the file is interpreted
         * @return the parsed configuration
         */
        std::shared_ptr<config_document> parse_file(std::unique_ptr<std::istream> stream, config_parse_options options);

        /** Parses a file into a config_document instance using default options. */
        std::shared_ptr<config_document> parse_file(std::unique_ptr<std::istream> stream);

        /**
         * Parses a string which should be valid HOCON or JSON.
         *
         * @param s string to parse
         * @param options parse options
         * @return the parsed configuration
         */
        std::shared_ptr<config_document> parse_string(std::string s; config_parse_options options);

        /** Parses a string into a config_document instance using default options. */
        std::shared_ptr<config_document> parse_string(std::string s);

}}  // namespace hocon::config_document_factory
