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
                {BufferType::DrawIndirect, GL_DRAW_INDIRECT_BUFFER},
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


        GLenum mapEnum(StorageFormat storageFormat)
        {
            static const std::unordered_map<StorageFormat, GLenum> items{
                {StorageFormat::RGBA, GL_RGBA},
                {StorageFormat::DepthComponent, GL_DEPTH_COMPONENT},
                {StorageFormat::DepthStencil, GL_DEPTH_STENCIL},
            };
            return _gl_map(items, storageFormat);
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
            static const std::unordered_map<Function, GLenum> items{
                {Function::LessEqual, GL_LEQUAL},
                {Function::GreaterEqual, GL_GEQUAL},
                {Function::Less, GL_LESS},
                {Function::Greater, GL_GREATER},
                {Function::Equal, GL_EQUAL},
                {Function::NotEqual, GL_NOTEQUAL},
                {Function::Always, GL_ALWAYS},
                {Function::Never, GL_NEVER},
            };
            return _gl_map(items, function);
        }


        GLint mapEnum(TextureMagFilter magFilter)
        {
            static const std::unordered_map<TextureMagFilter, GLint> items{
                {TextureMagFilter::Nearest, GL_NEAREST},
                {TextureMagFilter::Linear, GL_LINEAR},
            };
            auto it = items.find(magFilter);
            return it == items.end() ? GL_INVALID_ENUM : it->second;
        }


        GLint mapEnum(TextureMinFilter minFilter)
        {
            static const std::unordered_map<TextureMinFilter, GLint> items{
                {TextureMinFilter::Nearest, GL_NEAREST},
                {TextureMinFilter::Linear, GL_LINEAR},
                {TextureMinFilter::NearestMipMapNearest, GL_NEAREST_MIPMAP_NEAREST},
                {TextureMinFilter::LinearMipMapNearest, GL_LINEAR_MIPMAP_NEAREST},
                {TextureMinFilter::NearestMapMapLinear, GL_NEAREST_MIPMAP_LINEAR},
                {TextureMinFilter::LinearMipMapLinear, GL_LINEAR_MIPMAP_LINEAR},
            };
            auto it = items.find(minFilter);
            return it == items.end() ? GL_INVALID_ENUM : it->second;
        }


        GLenum mapEnum(TextureWrapMode wrapMode)
        {
            static const std::unordered_map<TextureWrapMode, GLenum> items{
                {TextureWrapMode::Repeat, GL_REPEAT},
                {TextureWrapMode::MirroredRepeat, GL_MIRRORED_REPEAT},
                {TextureWrapMode::ClampToEdge, GL_CLAMP_TO_EDGE},
            };
            return _gl_map(items, wrapMode);
        }


        GLenum mapEnum(Swizzle swizzle)
        {
            static const std::unordered_map<Swizzle, GLenum> items{
                {Swizzle::Red, GL_RED},
                {Swizzle::Green, GL_GREEN},
                {Swizzle::Blue, GL_BLUE},
                {Swizzle::Alpha, GL_ALPHA},
            };
            return _gl_map(items, swizzle);
        }


        GLenum mapEnum(Capability cap)
        {
            static const std::unordered_map<Capability, GLenum> items{
                // Fragment operations
                { Capability::Blend,                    GL_BLEND                    },
                { Capability::ColorLogicOp,             GL_COLOR_LOGIC_OP           },
                { Capability::DepthTest,                GL_DEPTH_TEST               },
                { Capability::StencilTest,              GL_STENCIL_TEST             },
                { Capability::ScissorTest,              GL_SCISSOR_TEST             },
                { Capability::AlphaTest,                GL_ALPHA_TEST               },

                // Rasterisation
                { Capability::CullFace,                 GL_CULL_FACE                },
                { Capability::PolygonOffsetFill,        GL_POLYGON_OFFSET_FILL      },
                { Capability::PolygonOffsetLine,        GL_POLYGON_OFFSET_LINE      },
                { Capability::PolygonOffsetPoint,       GL_POLYGON_OFFSET_POINT     },
                { Capability::LineSmooth,               GL_LINE_SMOOTH              },
                { Capability::PolygonSmooth,            GL_POLYGON_SMOOTH           },
                { Capability::Multisample,              GL_MULTISAMPLE              },
                { Capability::SampleAlphaToCoverage,    GL_SAMPLE_ALPHA_TO_COVERAGE },
                { Capability::SampleAlphaToOne,         GL_SAMPLE_ALPHA_TO_ONE      },
                { Capability::SampleCoverage,           GL_SAMPLE_COVERAGE          },
                { Capability::SampleShading,            GL_SAMPLE_SHADING           },

                // Primitive / geometry
                { Capability::PrimitiveRestart,         GL_PRIMITIVE_RESTART        },
                { Capability::PrimitiveRestartFixedIndex, GL_PRIMITIVE_RESTART_FIXED_INDEX },
                { Capability::RasterizerDiscard,        GL_RASTERIZER_DISCARD       },

                // Framebuffer / output
                { Capability::FramebufferSRGB,          GL_FRAMEBUFFER_SRGB         },
                { Capability::Dither,                   GL_DITHER                   },

                // Miscellaneous
                { Capability::DepthClamp,               GL_DEPTH_CLAMP              },
                { Capability::TextureCubeMapSeamless,   GL_TEXTURE_CUBE_MAP_SEAMLESS },
                { Capability::ProgramPointSize,         GL_PROGRAM_POINT_SIZE       },
                { Capability::DebugOutput,              GL_DEBUG_OUTPUT             },
                { Capability::DebugOutputSynchronous,   GL_DEBUG_OUTPUT_SYNCHRONOUS },
            };
            return _gl_map(items, cap);
        }

        GLenum mapEnum(BlendEquation equation)
        {
            static const std::unordered_map<BlendEquation, GLenum> items{
                { BlendEquation::Add,             GL_FUNC_ADD              },
                { BlendEquation::Subtract,        GL_FUNC_SUBTRACT         },
                { BlendEquation::ReverseSubtract, GL_FUNC_REVERSE_SUBTRACT },
                { BlendEquation::Min,             GL_MIN                   },
                { BlendEquation::Max,             GL_MAX                   },
            };
            return _gl_map(items, equation);
        }


        GLenum mapEnum(BlendFunction function)
        {
            static const std::unordered_map<BlendFunction, GLenum> items{
                { BlendFunction::Zero,                     GL_ZERO                     },
                { BlendFunction::One,                      GL_ONE                      },
                { BlendFunction::SourceColour,             GL_SRC_COLOR                },
                { BlendFunction::OneMinusSourceColour,     GL_ONE_MINUS_SRC_COLOR      },
                { BlendFunction::DestinationColour,        GL_DST_COLOR                },
                { BlendFunction::OneMinusDestinationColour,GL_ONE_MINUS_DST_COLOR      },
                { BlendFunction::SourceAlpha,              GL_SRC_ALPHA                },
                { BlendFunction::OneMinusSourceAlpha,      GL_ONE_MINUS_SRC_ALPHA      },
                { BlendFunction::DestinationAlpha,         GL_DST_ALPHA                },
                { BlendFunction::OneMinusDestinationAlpha, GL_ONE_MINUS_DST_ALPHA      },
                { BlendFunction::ConstantColour,           GL_CONSTANT_COLOR           },
                { BlendFunction::OneMinusConstantColour,   GL_ONE_MINUS_CONSTANT_COLOR },
                { BlendFunction::ConstantAlpha,            GL_CONSTANT_ALPHA           },
                { BlendFunction::OneMinusConstantAlpha,    GL_ONE_MINUS_CONSTANT_ALPHA },
                { BlendFunction::AlphaSaturate,            GL_SRC_ALPHA_SATURATE       },
            };
            return _gl_map(items, function);
        }


        GLenum mapEnum(StencilOperation operation)
        {
            static const std::unordered_map<StencilOperation, GLenum> items{
                { StencilOperation::Zero,         GL_ZERO      },
                { StencilOperation::Keep,         GL_KEEP      },
                { StencilOperation::Replace,      GL_REPLACE   },
                { StencilOperation::Increment,    GL_INCR      },
                { StencilOperation::IncrementWrap,GL_INCR_WRAP },
                { StencilOperation::Decrement,    GL_DECR      },
                { StencilOperation::DecrementWrap,GL_DECR_WRAP },
                { StencilOperation::Invert,       GL_INVERT    },
            };
            return _gl_map(items, operation);
        }

        GLenum mapEnum(LogicOperation op)
        {
            static const std::unordered_map<LogicOperation, GLenum> items{
                { LogicOperation::Clear,        GL_CLEAR         },
                { LogicOperation::Set,          GL_SET           },
                { LogicOperation::Copy,         GL_COPY          },
                { LogicOperation::CopyInverted, GL_COPY_INVERTED },
                { LogicOperation::Noop,         GL_NOOP          },
                { LogicOperation::Invert,       GL_INVERT        },
                { LogicOperation::And,          GL_AND           },
                { LogicOperation::Nand,         GL_NAND          },
                { LogicOperation::Or,           GL_OR            },
                { LogicOperation::Nor,          GL_NOR           },
                { LogicOperation::Xor,          GL_XOR           },
                { LogicOperation::Equivalent,   GL_EQUIV         },
                { LogicOperation::AndReverse,   GL_AND_REVERSE   },
                { LogicOperation::AndInverted,  GL_AND_INVERTED  },
                { LogicOperation::OrReverse,    GL_OR_REVERSE    },
                { LogicOperation::OrInverted,   GL_OR_INVERTED   },
            };
            return _gl_map(items, op);
        }


        GLenum mapEnum(FaceSelector face)
        {
            static const std::unordered_map<FaceSelector, GLenum> items{
                { FaceSelector::Front,        GL_FRONT          },
                { FaceSelector::Back,         GL_BACK           },
                { FaceSelector::FrontAndBack, GL_FRONT_AND_BACK },
            };
            return _gl_map(items, face);
        }

    }
}
