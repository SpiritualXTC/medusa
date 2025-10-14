#pragma once

#include <medusa/medusa.h>

namespace medusa
{
    enum class BufferType
    {
        Array,
        //AtomicCounter,
        //CopyRead,
        //CopyWrite,
        //DispatchIndirect,
        DrawIndirect,
        ElementArray,
        //PixelPack,
        //PixelUnpack,
        //Query,
        ShaderStorage,
        Texture,
        //TransformFeedback,
        Uniform,
    };

    enum class BufferUsage
    {
        StaticDraw,
        StaticRead,
        StaticCopy,
        DynamicDraw,
        DynamicRead,
        DynamicCopy,
        StreamDraw,
        StreamRead,
        StreamCopy,
    };
}
