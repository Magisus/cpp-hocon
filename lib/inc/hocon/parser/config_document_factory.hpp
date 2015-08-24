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
    config_document parse_stream(std::unqiue_ptr<std::istream> stream, config_parse_options options);

    

}}  // namespace hocon
