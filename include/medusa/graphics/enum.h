#pragma once

namespace medusa
{
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


}
