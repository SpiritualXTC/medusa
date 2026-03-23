#pragma once

namespace medusa
{
    /// <summary>
    /// Server-side GL capabilities that can be toggled with glEnable / glDisable.
    /// Only the subset relevant to the pipeline abstraction is listed here.
    /// Additional capabilities can be added as needed.
    /// </summary>
    enum class Capability
    {
        // Fragment operations
        Blend,
        ColorLogicOp,
        DepthTest,
        StencilTest,
        ScissorTest,
        AlphaTest,

        // Rasterisation
        CullFace,
        PolygonOffsetFill,
        PolygonOffsetLine,
        PolygonOffsetPoint,
        LineSmooth,
        PolygonSmooth,
        Multisample,
        SampleAlphaToCoverage,
        SampleAlphaToOne,
        SampleCoverage,
        SampleShading,

        // Primitive / geometry
        PrimitiveRestart,
        PrimitiveRestartFixedIndex,
        RasterizerDiscard,

        // Framebuffer / output
        FramebufferSRGB,
        Dither,

        // Miscellaneous
        DepthClamp,
        TextureCubeMapSeamless,
        ProgramPointSize,
        DebugOutput,
        DebugOutputSynchronous,
    };


    enum class PrimitiveType
    {
        Points,
        Lines,
        LineStrip,
        LineLoop,
        Polygon,
        Quads,
        QuadStrip,
        Triangles,
        TriangleStrip,
        TriangleFan,
    };


    enum class FaceDirection
    {
        ClockWise,
        CounterClockWise,
    };

    enum class CullMode
    {
        Front,
        Back,
        FrontAndBack,
    };

    enum class PolygonMode
    {
        Fill,
        Line,
    };

    enum class Function // Another name
    {
        Never,
        Less,
        Equal,
        LessEqual,
        Greater,
        NotEqual,
        GreaterEqual,
        Always,
    };

    enum class BlendEquation
    {
        Add,
        Subtract,
        ReverseSubtract,
        Min,
        Max,
    };

    enum class BlendFunction
    {
        Zero,
        One,
        SourceColour,
        OneMinusSourceColour,
        DestinationColour,
        OneMinusDestinationColour,
        SourceAlpha,
        OneMinusSourceAlpha,
        DestinationAlpha,
        OneMinusDestinationAlpha,
        ConstantColour,
        OneMinusConstantColour,
        ConstantAlpha,
        OneMinusConstantAlpha,
        AlphaSaturate,
    };

    enum class StencilOperation
    {
        Zero,
        Keep,
        Replace,
        Increment,
        IncrementWrap,
        Decrement,
        DecrementWrap,
        Invert,
    };


    enum class LogicOperation
    {
        Clear,          // 0
        Set,            // 1
        Copy,           // s
        CopyInverted,   // ~s
        Noop,           // d
        Invert,         // ~d
        And,            // s & d
        Nand,           // ~(s & d)
        Or,             // s | d
        Nor,            // ~(s | d)
        Xor,            // s ^ d
        Equivalent,     // ~(s ^ d)
        AndReverse,     // s & ~d
        AndInverted,    // ~s & d
        OrReverse,      // s | ~d
        OrInverted,     // ~s | d
    };

    enum class FaceSelector
    {
        Front,
        Back,
        FrontAndBack,
    };
}
