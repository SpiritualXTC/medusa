#include "pipeline_state_gl.h"

#include <core/utilities/logging.h>

#include <opengl/gl.h>

using namespace medusa;
using namespace medusa::opengl;


PipelineStateGL::PipelineStateGL()
{
    // Reasonable defaults
    _current = PipelineState();
}


PipelineStateGL::~PipelineStateGL()
{
}


// ---------------------------------------------------------------------------
// Stack
// ---------------------------------------------------------------------------

void PipelineStateGL::push()
{
    _stack.push(_current);
}


void PipelineStateGL::pop()
{
    if (_stack.empty())
        throw MedusaError("PipelineStateGL::pop() called on an empty stack");

    apply(_stack.top());
    _stack.pop();
}


// ---------------------------------------------------------------------------
// Public
// ---------------------------------------------------------------------------

void PipelineStateGL::apply(const PipelineState& next)
{
    applyCapabilities(next);
    applyBlend(next.blend());
    applyDepth(next.depth());
    applyStencil(next.stencil());
    applyRaster(next.raster());
    applyLogicOp(next.logicOp());

    _current = next;
}


// ---------------------------------------------------------------------------
// Capability
// ---------------------------------------------------------------------------

void PipelineStateGL::applyCapabilities(const PipelineState& next)
{
    static constexpr Capability kAllCapabilities[] =
    {
        Capability::Blend,
        Capability::ColorLogicOp,
        Capability::DepthTest,
        Capability::StencilTest,
        Capability::ScissorTest,
        Capability::AlphaTest,
        Capability::CullFace,
        Capability::PolygonOffsetFill,
        Capability::PolygonOffsetLine,
        Capability::PolygonOffsetPoint,
        Capability::LineSmooth,
        Capability::PolygonSmooth,
        Capability::Multisample,
        Capability::SampleAlphaToCoverage,
        Capability::SampleAlphaToOne,
        Capability::SampleCoverage,
        Capability::SampleShading,
        Capability::PrimitiveRestart,
        Capability::PrimitiveRestartFixedIndex,
        Capability::RasterizerDiscard,
        Capability::FramebufferSRGB,
        Capability::Dither,
        Capability::DepthClamp,
        Capability::TextureCubeMapSeamless,
        Capability::ProgramPointSize,
        Capability::DebugOutput,
        Capability::DebugOutputSynchronous,
    };

    for (Capability cap : kAllCapabilities)
    {
        const bool wasEnabled = _current.enabled(cap);
        const bool nowEnabled = next.enabled(cap);

        if (wasEnabled == nowEnabled)
            continue;

        if (nowEnabled)
        {
            glEnable(mapEnum(cap));
            getCheckError("glEnable");
        }
        else
        {
            glDisable(mapEnum(cap));
            getCheckError("glDisable");
        }
    }
}


// ---------------------------------------------------------------------------
// Blend
// ---------------------------------------------------------------------------

void PipelineStateGL::applyBlend(const BlendState& next)
{
    const BlendState& cur = _current.blend();

    if (cur.srcRGB != next.srcRGB || cur.dstRGB != next.dstRGB ||
        cur.srcAlpha != next.srcAlpha || cur.dstAlpha != next.dstAlpha)
    {
        glBlendFuncSeparate(mapEnum(next.srcRGB), mapEnum(next.dstRGB),
            mapEnum(next.srcAlpha), mapEnum(next.dstAlpha));
        getCheckError("glBlendFuncSeparate");
    }

    if (cur.equation != next.equation)
    {
        glBlendEquation(mapEnum(next.equation));
        getCheckError("glBlendEquation");
    }

    if (cur.colorR != next.colorR || cur.colorG != next.colorG ||
        cur.colorB != next.colorB || cur.colorA != next.colorA)
    {
        glBlendColor(next.colorR, next.colorG, next.colorB, next.colorA);
        getCheckError("glBlendColor");
    }
}


// ---------------------------------------------------------------------------
// Depth
// ---------------------------------------------------------------------------

void PipelineStateGL::applyDepth(const DepthState& next)
{
    const DepthState& cur = _current.depth();

    if (cur.func != next.func)
    {
        glDepthFunc(mapEnum(next.func));
        getCheckError("glDepthFunc");
    }

    if ((cur.depthNear != next.depthNear) || (cur.depthFar != next.depthFar))
    {
        glDepthRange(next.depthNear, next.depthFar);
        getCheckError("glDepthRange");
    }
}


// ---------------------------------------------------------------------------
// Stencil
// ---------------------------------------------------------------------------

void PipelineStateGL::applyStencil(const StencilState& next)
{
    const StencilState& cur = _current.stencil();

    // Front face
    if (cur.front.func != next.front.func ||
        cur.front.ref != next.front.ref ||
        cur.front.readMask != next.front.readMask)
    {
        glStencilFuncSeparate(GL_FRONT, mapEnum(next.front.func),
            next.front.ref, next.front.readMask);
        getCheckError("glStencilFuncSeparate(front)");
    }

    if (cur.front.sfail != next.front.sfail ||
        cur.front.dpfail != next.front.dpfail ||
        cur.front.dppass != next.front.dppass)
    {
        glStencilOpSeparate(GL_FRONT, mapEnum(next.front.sfail),
            mapEnum(next.front.dpfail), mapEnum(next.front.dppass));
        getCheckError("glStencilOpSeparate(front)");
    }

    if (cur.front.writeMask != next.front.writeMask)
    {
        glStencilMaskSeparate(GL_FRONT, next.front.writeMask);
        getCheckError("glStencilMaskSeparate(front)");
    }

    // Back face
    if (cur.back.func != next.back.func ||
        cur.back.ref != next.back.ref ||
        cur.back.readMask != next.back.readMask)
    {
        glStencilFuncSeparate(GL_BACK, mapEnum(next.back.func),
            next.back.ref, next.back.readMask);
        getCheckError("glStencilFuncSeparate(back)");
    }

    if (cur.back.sfail != next.back.sfail ||
        cur.back.dpfail != next.back.dpfail ||
        cur.back.dppass != next.back.dppass)
    {
        glStencilOpSeparate(GL_BACK, mapEnum(next.back.sfail),
            mapEnum(next.back.dpfail), mapEnum(next.back.dppass));
        getCheckError("glStencilOpSeparate(back)");
    }

    if (cur.back.writeMask != next.back.writeMask)
    {
        glStencilMaskSeparate(GL_BACK, next.back.writeMask);
        getCheckError("glStencilMaskSeparate(back)");
    }
}


// ---------------------------------------------------------------------------
// Rasterisation
// ---------------------------------------------------------------------------

void PipelineStateGL::applyRaster(const RasterState& next)
{
    const RasterState& cur = _current.raster();

    if (cur.cullMode != next.cullMode)
    {
        glCullFace(mapEnum(next.cullMode));
        getCheckError("glCullFace");
    }

    if (cur.frontFace != next.frontFace)
    {
        glFrontFace(mapEnum(next.frontFace));
        getCheckError("glFrontFace");
    }

    if (cur.polygonMode != next.polygonMode)
    {
        glPolygonMode(GL_FRONT_AND_BACK, mapEnum(next.polygonMode));
        getCheckError("glPolygonMode");
    }

    if (cur.polygonOffsetFactor != next.polygonOffsetFactor ||
        cur.polygonOffsetUnits != next.polygonOffsetUnits)
    {
        glPolygonOffset(next.polygonOffsetFactor, next.polygonOffsetUnits);
        getCheckError("glPolygonOffset");
    }

    if (cur.lineWidth != next.lineWidth)
    {
        glLineWidth(next.lineWidth);
        getCheckError("glLineWidth");
    }

    if (cur.pointSize != next.pointSize)
    {
        glPointSize(next.pointSize);
        getCheckError("glPointSize");
    }
}


// ---------------------------------------------------------------------------
// Logic operation
// ---------------------------------------------------------------------------

void PipelineStateGL::applyLogicOp(LogicOperation next)
{
    if (_current.logicOp() == next)
        return;

    glLogicOp(mapEnum(next));
    getCheckError("glLogicOp");
}
