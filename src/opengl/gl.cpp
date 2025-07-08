#include "gl.h"

#include <medusa/medusa.h>
#include <core/utilities/logging.h>

using namespace medusa;
using namespace medusa::opengl;


namespace std
{
    template <> struct hash<DType>
    {
        std::size_t operator()(DType const& r) const noexcept
        {
            return 0;
        }
    };
}



namespace medusa
{

    namespace opengl
    {
        template <typename E>
        GLenum _gl_map(const std::unordered_map<E, GLenum>& m, E e)
        {
            auto it = m.find(e);
            return it == m.end() ? GL_INVALID_ENUM : it->second;
        }

        GLenum getCheckError(const std::string& message)
        {
            GLenum error = glGetError();
            if (error != 0)
            {
                const unsigned char* s_error = glewGetErrorString(error);
                logging::error(fmt::format("{}: {} `{}`", message, error, (char*)s_error));
            }

            return error;
        }



        GLenum mapEnum(const DType& dtype)
        {
            static const std::unordered_map<DType, GLenum> items{
                {types::FloatV1, GL_FLOAT},
                {types::FloatV2, GL_FLOAT},
                {types::FloatV3, GL_FLOAT},
                {types::FloatV4, GL_FLOAT},

                {types::IntV1, GL_INT},
                {types::IntV2, GL_INT},
                {types::IntV3, GL_INT},
                {types::IntV4, GL_INT},
            };
            return _gl_map(items, dtype);
        }


        GLenum mapEnum(ShaderType shaderType)
        {
            static const std::unordered_map<ShaderType, GLenum> items{
                {ShaderType::VertexShader, GL_VERTEX_SHADER},
                {ShaderType::FragmentShader, GL_FRAGMENT_SHADER},
                {ShaderType::GeometryShader, GL_GEOMETRY_SHADER},
            };
            return _gl_map(items, shaderType);
        }


        GLenum mapEnum(BufferUsage bufferUsage)
        {
            static const std::unordered_map<BufferUsage, GLenum> items{
                {BufferUsage::StaticDraw, GL_STATIC_DRAW},
                {BufferUsage::StaticRead, GL_STATIC_READ},
                {BufferUsage::StaticCopy, GL_STATIC_COPY},
                {BufferUsage::DynamicDraw, GL_DYNAMIC_DRAW},
                {BufferUsage::DynamicRead, GL_DYNAMIC_READ},
                {BufferUsage::DynamicCopy, GL_DYNAMIC_COPY},
                {BufferUsage::StreamDraw, GL_STREAM_DRAW},
                {BufferUsage::StreamRead, GL_STREAM_READ},
                {BufferUsage::StreamCopy, GL_STREAM_COPY},
            };
            return _gl_map(items, bufferUsage);
        }


        GLenum mapEnum(BufferType bufferType)
        {
            static const std::unordered_map<BufferType, GLenum> items{
                {BufferType::Array, GL_ARRAY_BUFFER},
                {BufferType::ElementArray, GL_ELEMENT_ARRAY_BUFFER},
                {BufferType::ShaderStorage, GL_SHADER_STORAGE_BUFFER},
                {BufferType::Texture, GL_TEXTURE_BUFFER},
                {BufferType::Uniform, GL_UNIFORM_BUFFER},
            };
            return _gl_map(items, bufferType);
        }


        GLenum mapEnum(PrimitiveType primitiveType)
        {
            static const std::unordered_map<PrimitiveType, GLenum> items{
                {PrimitiveType::Points, GL_POINTS},

                {PrimitiveType::Lines, GL_LINES},
                {PrimitiveType::LineStrip, GL_LINE_STRIP},
                {PrimitiveType::LineLoop, GL_LINE_LOOP},

                {PrimitiveType::Polygon, GL_POLYGON},

                {PrimitiveType::Quads, GL_QUADS},
                {PrimitiveType::QuadStrip, GL_QUAD_STRIP},

                {PrimitiveType::Triangles, GL_TRIANGLES},
                {PrimitiveType::TriangleStrip, GL_TRIANGLE_STRIP},
                {PrimitiveType::TriangleFan, GL_TRIANGLE_FAN},
            };
            return _gl_map(items, primitiveType);
        }


        GLenum mapEnum(TextureFormat textureFormat)
        {
            static const std::unordered_map<TextureFormat, GLenum> items{
                {TextureFormat::RGBA8, GL_RGBA8},
                {TextureFormat::RGBA16F, GL_RGBA16F},
                {TextureFormat::RGBA32F, GL_RGBA32F},

                {TextureFormat::Depth16, GL_DEPTH_COMPONENT16},
                {TextureFormat::Depth24, GL_DEPTH_COMPONENT24},
                {TextureFormat::Depth32, GL_DEPTH_COMPONENT32},
                {TextureFormat::DepthStencil, GL_DEPTH_STENCIL},
            };
            return _gl_map(items, textureFormat);
        }

        GLenum mapEnum(FaceDirection faceDirection)
        {
            static const std::unordered_map<FaceDirection, GLenum> items{
                {FaceDirection::ClockWise, GL_CW},
                {FaceDirection::CounterClockWise, GL_CCW},
            };
            return _gl_map(items, faceDirection);
        }

        GLenum mapEnum(CullMode cullMode)
        {
            static const std::unordered_map<CullMode, GLenum> items{
                {CullMode::Front, GL_FRONT},
                {CullMode::Back, GL_BACK},
                {CullMode::FrontAndBack, GL_FRONT_AND_BACK},
            };
            return _gl_map(items, cullMode);
        }

        GLenum mapEnum(PolygonMode polygonMode)
        {
            static const std::unordered_map<PolygonMode, GLenum> items
            {
                {PolygonMode::Fill, GL_FILL},
                {PolygonMode::Line, GL_LINE},
            };
            return _gl_map(items, polygonMode);
        }


        GLenum mapEnum(Function function)
        {
            static const std::unordered_map<Function, GLenum> items
            {
                {Function::Never, GL_NEVER,},
                {Function::Less, GL_LESS, },
                {Function::Equal, GL_EQUAL,},
                {Function::LessEqual, GL_LEQUAL,},
                {Function::Greater, GL_GREATER, },
                {Function::NotEqual, GL_NOTEQUAL,},
                {Function::GreaterEqual, GL_GEQUAL,},
                {Function::Always, GL_ALWAYS},
            };
            return _gl_map(items, function);
        }




        /*


        inline GLenum convBlendEquation(BlendEquation equation)
        {
            static GLenum dat[]
            {
                GL_FUNC_ADD,
                GL_FUNC_SUBTRACT,
                GL_FUNC_REVERSE_SUBTRACT,
                GL_MIN,
                GL_MAX
            };

            return dat[(Int32)equation];
        }

        inline GLenum convBlendFunction(BlendFunction function)
        {
            static GLenum dat[]
            {
                GL_ZERO,
                GL_ONE,

                GL_SRC_COLOR,
                GL_ONE_MINUS_SRC_COLOR,

                GL_DST_COLOR,
                GL_ONE_MINUS_DST_COLOR,

                GL_SRC_ALPHA,
                GL_ONE_MINUS_SRC_ALPHA,

                GL_DST_ALPHA,
                GL_ONE_MINUS_DST_ALPHA,

                GL_CONSTANT_COLOR,
                GL_ONE_MINUS_CONSTANT_COLOR,

                GL_CONSTANT_ALPHA,
                GL_ONE_MINUS_CONSTANT_ALPHA,

                GL_SRC_ALPHA_SATURATE
            };

            return dat[(Int32)function];
        }



        inline GLenum convStencilOperation(StencilOperation op)
        {
            static GLenum dat[]
            {
                GL_ZERO,
                GL_KEEP,
                GL_REPLACE,
                GL_INCR,
                GL_INCR_WRAP,
                GL_DECR,
                GL_DECR_WRAP,
                GL_INVERT
            };

            return dat[(Int32)op];
        }


        inline GLenum convDataType(DataType dataType)
        {
            static GLenum dat[]
            {
                GL_BYTE,
                GL_UNSIGNED_BYTE,
                GL_SHORT,
                GL_UNSIGNED_SHORT,
                GL_INT,
                GL_UNSIGNED_INT,
                GL_FLOAT,
                GL_FIXED,
            };

            return dat[(Int32)dataType];
        }


        inline GLenum convStorageFormatInternal(StorageFormat format)
        {
            static GLenum dat[]
            {
                GL_RGBA,

                GL_RGBA16F,
                GL_RGBA32F,

                GL_DEPTH_COMPONENT16,
                GL_DEPTH_COMPONENT24,
                GL_DEPTH_COMPONENT32,

                GL_DEPTH_STENCIL
            };


            return dat[(Int32)format];
        }


        inline GLenum convStorageFormat(StorageFormat format)
        {
            // Same as above... only the simpler version... lol
            // https://www.opengl.org/sdk/docs/man/html/glTexImage2D.xhtml
            static GLenum dat[]
            {
                GL_RGBA,

                GL_RGBA,
                GL_RGBA,

                GL_DEPTH_COMPONENT,
                GL_DEPTH_COMPONENT,
                GL_DEPTH_COMPONENT,

                GL_DEPTH_STENCIL
            };


            return dat[(Int32)format];
        }


        inline GLint convTextureMagFilter(TextureMagFilter filter)
        {
            static GLenum dat[]
            {
                GL_NEAREST,
                GL_LINEAR,
            };

            return dat[(Int32)filter];
        }

        inline GLint convTextureMinFilter(TextureMinFilter filter)
        {
            static GLenum dat[]
            {
                GL_NEAREST,
                GL_LINEAR,

                GL_NEAREST_MIPMAP_NEAREST,
                GL_LINEAR_MIPMAP_NEAREST,

                GL_NEAREST_MIPMAP_LINEAR,
                GL_LINEAR_MIPMAP_LINEAR,
            };

            return dat[(Int32)filter];
        }

        inline GLenum convTextureWrapMode(TextureWrapMode wrapMode)
        {
            static GLenum dat[]
            {
                GL_REPEAT,
                GL_MIRRORED_REPEAT,
                GL_CLAMP_TO_EDGE,
            };

            return dat[(Int32)wrapMode];
        }

        */




        // What as this? lol
        /*

        inline GLenum convFaceMode(FaceMode face)
        {
            static GLenum dat[]
            {
                GL_FRONT,
                GL_BACK,
                GL_FRONT_AND_BACK
            };

            return dat[(Int32)face];
        }
        */

    }
}
