#include "pipeline_state_parser.h"

#include <core/utilities/logging.h>

#include <algorithm>
#include <stdexcept>


using namespace medusa;


// ---------------------------------------------------------------------------
// Internal helpers
// ---------------------------------------------------------------------------


bool PipelineStateParser::parseBool(const std::string& v, bool& out)
{
    const std::string l = lower(v);
    if (l == "true" || l == "1" || l == "yes") { out = true;  return true; }
    if (l == "false" || l == "0" || l == "no") { out = false; return true; }
    return false;
}


bool PipelineStateParser::parseFloat(const std::string& v, float& out)
{
    try { out = std::stof(v); return true; }
    catch (...) { return false; }
}


bool PipelineStateParser::parseDouble(const std::string& v, double& out)
{
    try { out = std::stod(v); return true; }
    catch (...) { return false; }
}


bool PipelineStateParser::parseInt(const std::string& v, int32_t& out)
{
    try { out = std::stoi(v); return true; }
    catch (...) { return false; }
}


bool PipelineStateParser::parseUInt(const std::string& v, uint32_t& out)
{
    try { out = static_cast<uint32_t>(std::stoul(v)); return true; }
    catch (...) { return false; }
}


// ---------------------------------------------------------------------------
// fromString - enum conversions
// ---------------------------------------------------------------------------

std::string medusa::lower(const std::string& s)
{
    std::string out = s;
    std::transform(out.begin(), out.end(), out.begin(),
        [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return out;
}

bool medusa::fromString(const std::string& s, Capability& out)
{
    static const std::unordered_map<std::string, Capability> map{
        { "blend",                      Capability::Blend                      },
        { "colorlogicop",               Capability::ColorLogicOp               },
        { "depthtest",                  Capability::DepthTest                  },
        { "stenciltest",                Capability::StencilTest                },
        { "scissortest",                Capability::ScissorTest                },
        { "alphatest",                  Capability::AlphaTest                  },
        { "cullface",                   Capability::CullFace                   },
        { "polygonoffsetfill",          Capability::PolygonOffsetFill          },
        { "polygonoffsetline",          Capability::PolygonOffsetLine          },
        { "polygonoffsetpoint",         Capability::PolygonOffsetPoint         },
        { "linesmooth",                 Capability::LineSmooth                 },
        { "polygonsmooth",              Capability::PolygonSmooth              },
        { "multisample",                Capability::Multisample                },
        { "samplealphatocoverage",      Capability::SampleAlphaToCoverage      },
        { "samplealphatoone",           Capability::SampleAlphaToOne           },
        { "samplecoverage",             Capability::SampleCoverage             },
        { "sampleshading",              Capability::SampleShading              },
        { "primitiverestart",           Capability::PrimitiveRestart           },
        { "primitiverestartfixedindex", Capability::PrimitiveRestartFixedIndex },
        { "rasterizerdiscard",          Capability::RasterizerDiscard          },
        { "framebuffersrgb",            Capability::FramebufferSRGB            },
        { "dither",                     Capability::Dither                     },
        { "depthclamp",                 Capability::DepthClamp                 },
        { "texturecubemapseamless",     Capability::TextureCubeMapSeamless     },
        { "programpointsize",           Capability::ProgramPointSize           },
        { "debugoutput",                Capability::DebugOutput                },
        { "debugoutputsynchronous",     Capability::DebugOutputSynchronous     },
    };

    auto it = map.find(lower(s));
    if (it == map.end()) return false;
    out = it->second;
    return true;
}


bool medusa::fromString(const std::string& s, BlendFunction& out)
{
    static const std::unordered_map<std::string, BlendFunction> map{
        { "zero",                     BlendFunction::Zero                     },
        { "one",                      BlendFunction::One                      },
        { "sourcecolour",             BlendFunction::SourceColour             },
        { "oneminussourcecolour",     BlendFunction::OneMinusSourceColour     },
        { "destinationcolour",        BlendFunction::DestinationColour        },
        { "oneminusdestinationcolour",BlendFunction::OneMinusDestinationColour},
        { "sourcealpha",              BlendFunction::SourceAlpha              },
        { "oneminussourcealpha",      BlendFunction::OneMinusSourceAlpha      },
        { "destinationalpha",         BlendFunction::DestinationAlpha         },
        { "oneminusdestinationalpha", BlendFunction::OneMinusDestinationAlpha },
        { "constantcolour",           BlendFunction::ConstantColour           },
        { "oneminusconstantcolour",   BlendFunction::OneMinusConstantColour   },
        { "constantalpha",            BlendFunction::ConstantAlpha            },
        { "oneminusconstantalpha",    BlendFunction::OneMinusConstantAlpha    },
        { "alphasaturate",            BlendFunction::AlphaSaturate            },
    };

    auto it = map.find(lower(s));
    if (it == map.end()) return false;
    out = it->second;
    return true;
}


bool medusa::fromString(const std::string& s, BlendEquation& out)
{
    static const std::unordered_map<std::string, BlendEquation> map{
        { "add",             BlendEquation::Add             },
        { "subtract",        BlendEquation::Subtract        },
        { "reversesubtract", BlendEquation::ReverseSubtract },
        { "min",             BlendEquation::Min             },
        { "max",             BlendEquation::Max             },
    };

    auto it = map.find(lower(s));
    if (it == map.end()) return false;
    out = it->second;
    return true;
}


bool medusa::fromString(const std::string& s, Function& out)
{
    static const std::unordered_map<std::string, Function> map{
        { "never",        Function::Never        },
        { "less",         Function::Less         },
        { "equal",        Function::Equal        },
        { "lessequal",    Function::LessEqual    },
        { "greater",      Function::Greater      },
        { "notequal",     Function::NotEqual     },
        { "greaterequal", Function::GreaterEqual },
        { "always",       Function::Always       },
    };

    auto it = map.find(lower(s));
    if (it == map.end()) return false;
    out = it->second;
    return true;
}


bool medusa::fromString(const std::string& s, StencilOperation& out)
{
    static const std::unordered_map<std::string, StencilOperation> map{
        { "zero",         StencilOperation::Zero         },
        { "keep",         StencilOperation::Keep         },
        { "replace",      StencilOperation::Replace      },
        { "increment",    StencilOperation::Increment    },
        { "incrementwrap",StencilOperation::IncrementWrap},
        { "decrement",    StencilOperation::Decrement    },
        { "decrementwrap",StencilOperation::DecrementWrap},
        { "invert",       StencilOperation::Invert       },
    };

    auto it = map.find(lower(s));
    if (it == map.end()) return false;
    out = it->second;
    return true;
}


bool medusa::fromString(const std::string& s, CullMode& out)
{
    static const std::unordered_map<std::string, CullMode> map{
        { "front",        CullMode::Front        },
        { "back",         CullMode::Back         },
        { "frontandback", CullMode::FrontAndBack },
    };

    auto it = map.find(lower(s));
    if (it == map.end()) return false;
    out = it->second;
    return true;
}


bool medusa::fromString(const std::string& s, FaceDirection& out)
{
    static const std::unordered_map<std::string, FaceDirection> map{
        { "clockwise",        FaceDirection::ClockWise        },
        { "counterclockwise", FaceDirection::CounterClockWise },
        { "cw",        FaceDirection::ClockWise},
        { "ccw", FaceDirection::CounterClockWise },
    };

    auto it = map.find(lower(s));
    if (it == map.end()) return false;
    out = it->second;
    return true;
}


bool medusa::fromString(const std::string& s, PolygonMode& out)
{
    static const std::unordered_map<std::string, PolygonMode> map{
        { "fill", PolygonMode::Fill },
        { "line", PolygonMode::Line },
    };

    auto it = map.find(lower(s));
    if (it == map.end()) return false;
    out = it->second;
    return true;
}


bool medusa::fromString(const std::string& s, LogicOperation& out)
{
    static const std::unordered_map<std::string, LogicOperation> map{
        { "clear",        LogicOperation::Clear        },
        { "set",          LogicOperation::Set          },
        { "copy",         LogicOperation::Copy         },
        { "copyinverted", LogicOperation::CopyInverted },
        { "noop",         LogicOperation::Noop         },
        { "invert",       LogicOperation::Invert       },
        { "and",          LogicOperation::And          },
        { "nand",         LogicOperation::Nand         },
        { "or",           LogicOperation::Or           },
        { "nor",          LogicOperation::Nor          },
        { "xor",          LogicOperation::Xor          },
        { "equivalent",   LogicOperation::Equivalent   },
        { "andreverse",   LogicOperation::AndReverse   },
        { "andinverted",  LogicOperation::AndInverted  },
        { "orreverse",    LogicOperation::OrReverse    },
        { "orinverted",   LogicOperation::OrInverted   },
    };

    auto it = map.find(lower(s));
    if (it == map.end()) return false;
    out = it->second;
    return true;
}


bool medusa::fromString(const std::string& s, FaceSelector& out)
{
    static const std::unordered_map<std::string, FaceSelector> map{
        { "front",        FaceSelector::Front        },
        { "back",         FaceSelector::Back         },
        { "frontandback", FaceSelector::FrontAndBack },
    };

    auto it = map.find(lower(s));
    if (it == map.end()) return false;
    out = it->second;
    return true;
}


// ---------------------------------------------------------------------------
// PipelineStateParser::apply
// ---------------------------------------------------------------------------

uint32_t PipelineStateParser::apply(PipelineState& state,
    const std::unordered_map<std::string, std::string>& pairs)
{
    uint32_t errors = 0;

    for (const auto& [rawKey, rawValue] : pairs)
    {
        const std::string key = lower(rawKey);
        const std::string val = rawValue;

        // ----------------------------------------------------------------
        // Capability toggles: enable.<capability> = true/false
        // ----------------------------------------------------------------
        if (key.substr(0, 7) == "enable.")
        {
            Capability cap{};
            bool on = false;
            if (!fromString(key.substr(7), cap) || !parseBool(val, on))
            {
                logging::warn("PipelineStateParser: unrecognised capability toggle `{}`", rawKey);
                ++errors;
                continue;
            }
            on ? state.enable(cap) : state.disable(cap);
            continue;
        }

        // ----------------------------------------------------------------
        // Blend
        // ----------------------------------------------------------------
        if (key == "blendsrcrgb")
        {
            BlendFunction f{};
            if (!fromString(val, f)) { logging::warn("PipelineStateParser: bad BlendFunction `{}`", val); ++errors; continue; }
            state.blend(BlendState{ f, state.blend().dstRGB, state.blend().srcAlpha, state.blend().dstAlpha,
                                    state.blend().equation,
                                    state.blend().colorR, state.blend().colorG, state.blend().colorB, state.blend().colorA });
            continue;
        }
        if (key == "blenddstrgb")
        {
            BlendFunction f{};
            if (!fromString(val, f)) { logging::warn("PipelineStateParser: bad BlendFunction `{}`", val); ++errors; continue; }
            state.blend(BlendState{ state.blend().srcRGB, f, state.blend().srcAlpha, state.blend().dstAlpha,
                                    state.blend().equation,
                                    state.blend().colorR, state.blend().colorG, state.blend().colorB, state.blend().colorA });
            continue;
        }
        if (key == "blendsrcalpha")
        {
            BlendFunction f{};
            if (!fromString(val, f)) { logging::warn("PipelineStateParser: bad BlendFunction `{}`", val); ++errors; continue; }
            state.blend(BlendState{ state.blend().srcRGB, state.blend().dstRGB, f, state.blend().dstAlpha,
                                    state.blend().equation,
                                    state.blend().colorR, state.blend().colorG, state.blend().colorB, state.blend().colorA });
            continue;
        }
        if (key == "blenddstalpha")
        {
            BlendFunction f{};
            if (!fromString(val, f)) { logging::warn("PipelineStateParser: bad BlendFunction `{}`", val); ++errors; continue; }
            state.blend(BlendState{ state.blend().srcRGB, state.blend().dstRGB, state.blend().srcAlpha, f,
                                    state.blend().equation,
                                    state.blend().colorR, state.blend().colorG, state.blend().colorB, state.blend().colorA });
            continue;
        }
        if (key == "blendequation")
        {
            BlendEquation e{};
            if (!fromString(val, e)) { logging::warn("PipelineStateParser: bad BlendEquation `{}`", val); ++errors; continue; }
            state.blendEquation(e);
            continue;
        }
        if (key == "blendcolorr") { float f{}; if (!parseFloat(val, f)) { ++errors; continue; } auto b = state.blend(); b.colorR = f; state.blend(b); continue; }
        if (key == "blendcolorg") { float f{}; if (!parseFloat(val, f)) { ++errors; continue; } auto b = state.blend(); b.colorG = f; state.blend(b); continue; }
        if (key == "blendcolorb") { float f{}; if (!parseFloat(val, f)) { ++errors; continue; } auto b = state.blend(); b.colorB = f; state.blend(b); continue; }
        if (key == "blendcolora") { float f{}; if (!parseFloat(val, f)) { ++errors; continue; } auto b = state.blend(); b.colorA = f; state.blend(b); continue; }

        // ----------------------------------------------------------------
        // Depth
        // ----------------------------------------------------------------
        if (key == "depthfunc")
        {
            Function f{};
            if (!fromString(val, f)) { logging::warn("PipelineStateParser: bad Function `{}`", val); ++errors; continue; }
            state.depthFunc(f);
            continue;
        }
        if (key == "depthnear") { double d{}; if (!parseDouble(val, d)) { ++errors; continue; } state.depthRange(d, state.depth().depthFar);  continue; }
        if (key == "depthfar") { double d{}; if (!parseDouble(val, d)) { ++errors; continue; } state.depthRange(state.depth().depthNear, d); continue; }

        // ----------------------------------------------------------------
        // Stencil - both faces
        // ----------------------------------------------------------------
        if (key == "stencilfunc")
        {
            Function f{};
            if (!fromString(val, f)) { logging::warn("PipelineStateParser: bad Function `{}`", val); ++errors; continue; }
            state.stencilFunc(f, state.stencil().front.ref, state.stencil().front.readMask);
            continue;
        }
        if (key == "stencilref")
        {
            int32_t i{};
            if (!parseInt(val, i)) { ++errors; continue; }
            state.stencilFunc(state.stencil().front.func, i, state.stencil().front.readMask);
            continue;
        }
        if (key == "stencilreadmask")
        {
            uint32_t u{};
            if (!parseUInt(val, u)) { ++errors; continue; }
            state.stencilFunc(state.stencil().front.func, state.stencil().front.ref, u);
            continue;
        }
        if (key == "stencilwritemask")
        {
            uint32_t u{};
            if (!parseUInt(val, u)) { ++errors; continue; }
            state.stencilMask(u);
            continue;
        }
        if (key == "stencilsfail")
        {
            StencilOperation op{};
            if (!fromString(val, op)) { logging::warn("PipelineStateParser: bad StencilOperation `{}`", val); ++errors; continue; }
            state.stencilOp(op, state.stencil().front.dpfail, state.stencil().front.dppass);
            continue;
        }
        if (key == "stencildpfail")
        {
            StencilOperation op{};
            if (!fromString(val, op)) { logging::warn("PipelineStateParser: bad StencilOperation `{}`", val); ++errors; continue; }
            state.stencilOp(state.stencil().front.sfail, op, state.stencil().front.dppass);
            continue;
        }
        if (key == "stencildppass")
        {
            StencilOperation op{};
            if (!fromString(val, op)) { logging::warn("PipelineStateParser: bad StencilOperation `{}`", val); ++errors; continue; }
            state.stencilOp(state.stencil().front.sfail, state.stencil().front.dpfail, op);
            continue;
        }

        // ----------------------------------------------------------------
        // Rasterisation
        // ----------------------------------------------------------------
        if (key == "cullmode")
        {
            CullMode m{};
            if (!fromString(val, m)) { logging::warn("PipelineStateParser: bad CullMode `{}`", val); ++errors; continue; }
            state.cullFace(m);
            continue;
        }
        if (key == "frontface")
        {
            FaceDirection d{};
            if (!fromString(val, d)) { logging::warn("PipelineStateParser: bad FaceDirection `{}`", val); ++errors; continue; }
            state.frontFace(d);
            continue;
        }
        if (key == "polygonmode")
        {
            PolygonMode m{};
            if (!fromString(val, m)) { logging::warn("PipelineStateParser: bad PolygonMode `{}`", val); ++errors; continue; }
            state.polygonMode(m);
            continue;
        }
        if (key == "polygonoffsetfactor") { float f{}; if (!parseFloat(val, f)) { ++errors; continue; } state.polygonOffset(f, state.raster().polygonOffsetUnits);  continue; }
        if (key == "polygonoffsetunits") { float f{}; if (!parseFloat(val, f)) { ++errors; continue; } state.polygonOffset(state.raster().polygonOffsetFactor, f); continue; }
        if (key == "linewidth") { float f{}; if (!parseFloat(val, f)) { ++errors; continue; } state.lineWidth(f);  continue; }
        if (key == "pointsize") { float f{}; if (!parseFloat(val, f)) { ++errors; continue; } state.pointSize(f); continue; }

        // ----------------------------------------------------------------
        // Logic operation
        // ----------------------------------------------------------------
        if (key == "logicop")
        {
            LogicOperation op{};
            if (!fromString(val, op)) { logging::warn("PipelineStateParser: bad LogicOperation `{}`", val); ++errors; continue; }
            state.logicOp(op);
            continue;
        }

        logging::warn("PipelineStateParser: unknown key `{}`", rawKey);
        ++errors;
    }

    return errors;
}
