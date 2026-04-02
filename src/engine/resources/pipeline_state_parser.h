#pragma once

#include <medusa/medusa.h>
#include <medusa/graphics/enum.h>
#include <medusa/renderer/pipeline_state.h>


namespace medusa
{
    // -----------------------------------------------------------------------
    // String conversion helpers
    // Each returns false and leaves the output unchanged if the string is
    // not recognised, allowing the caller to emit a diagnostic.
    // All comparisons are case-insensitive.
    // -----------------------------------------------------------------------

    bool fromString(const std::string& s, Capability& out);
    bool fromString(const std::string& s, BlendFunction& out);
    bool fromString(const std::string& s, BlendEquation& out);
    bool fromString(const std::string& s, Function& out);
    bool fromString(const std::string& s, StencilOperation& out);
    bool fromString(const std::string& s, CullMode& out);
    bool fromString(const std::string& s, FaceDirection& out);
    bool fromString(const std::string& s, PolygonMode& out);
    bool fromString(const std::string& s, LogicOperation& out);
    bool fromString(const std::string& s, FaceSelector& out);

    std::string lower(const std::string& s);

    // -----------------------------------------------------------------------
    // PipelineStateParser
    // Accepts pre-parsed key/value string pairs and applies them to a
    // PipelineState. No file I/O is performed here.
    // -----------------------------------------------------------------------

    /// <summary>
    /// Applies a flat map of case-insensitive key/value string pairs to a
    /// PipelineState. Each key corresponds to a single pipeline property;
    /// the value is either a plain scalar (float, int, bool) or the
    /// case-insensitive name of an enum enumerator.
    ///
    /// Returns the number of keys that could not be applied. The caller is
    /// responsible for providing the map; this class performs no file I/O.
    /// </summary>
    class PipelineStateParser
    {
    public:
        PipelineStateParser() = delete;

        /// <summary>
        /// Applies every key/value pair in the map to state.
        /// Unknown or malformed entries are logged and counted.
        /// </summary>
        static uint32_t apply(PipelineState& state,
            const std::unordered_map<std::string, std::string>& pairs);

    private:
        // Normalise a string to lowercase for case-insensitive comparison


        // Scalar parsers
        static bool parseBool(const std::string& v, bool& out);
        static bool parseFloat(const std::string& v, float& out);
        static bool parseDouble(const std::string& v, double& out);
        static bool parseInt(const std::string& v, int32_t& out);
        static bool parseUInt(const std::string& v, uint32_t& out);
    };

}  // namespace medusa
