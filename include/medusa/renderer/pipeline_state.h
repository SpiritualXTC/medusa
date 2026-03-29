#pragma once

#include <medusa/medusa.h>
#include <medusa/graphics/enum.h>

#include <unordered_set>


namespace medusa
{
    // -----------------------------------------------------------------------
    // State aggregates
    // -----------------------------------------------------------------------

    struct BlendState
    {
        BlendFunction   srcRGB = BlendFunction::One;
        BlendFunction   dstRGB = BlendFunction::Zero;
        BlendFunction   srcAlpha = BlendFunction::One;
        BlendFunction   dstAlpha = BlendFunction::Zero;
        BlendEquation   equation = BlendEquation::Add;
        float           colorR = 0.0f;
        float           colorG = 0.0f;
        float           colorB = 0.0f;
        float           colorA = 0.0f;

        bool operator==(const BlendState& o) const
        {
            return srcRGB == o.srcRGB && dstRGB == o.dstRGB &&
                srcAlpha == o.srcAlpha && dstAlpha == o.dstAlpha &&
                equation == o.equation &&
                colorR == o.colorR && colorG == o.colorG &&
                colorB == o.colorB && colorA == o.colorA;
        }

        bool operator!=(const BlendState& o) const { return !(*this == o); }
    };


    struct DepthState
    {
        Function    func = Function::Less;
        double      depthNear = 0.0;
        double      depthFar = 1.0;

        bool operator==(const DepthState& o) const
        {
            return func == o.func && depthNear == o.depthNear && depthFar == o.depthFar;
        }

        bool operator!=(const DepthState& o) const { return !(*this == o); }
    };


    struct StencilFaceState
    {
        Function            func = Function::Always;
        int32_t             ref = 0;
        uint32_t            readMask = 0xFFFFFFFF;
        uint32_t            writeMask = 0xFFFFFFFF;
        StencilOperation    sfail = StencilOperation::Keep;
        StencilOperation    dpfail = StencilOperation::Keep;
        StencilOperation    dppass = StencilOperation::Keep;

        bool operator==(const StencilFaceState& o) const
        {
            return func == o.func && ref == o.ref &&
                readMask == o.readMask && writeMask == o.writeMask &&
                sfail == o.sfail && dpfail == o.dpfail &&
                dppass == o.dppass;
        }

        bool operator!=(const StencilFaceState& o) const { return !(*this == o); }
    };


    struct StencilState
    {
        StencilFaceState    front;
        StencilFaceState    back;

        bool operator==(const StencilState& o) const
        {
            return front == o.front && back == o.back;
        }

        bool operator!=(const StencilState& o) const { return !(*this == o); }
    };


    struct RasterState
    {
        CullMode        cullMode = CullMode::Back;
        FaceDirection   frontFace = FaceDirection::CounterClockWise;
        PolygonMode     polygonMode = PolygonMode::Fill;
        float           polygonOffsetFactor = 0.0f;
        float           polygonOffsetUnits = 0.0f;
        float           lineWidth = 1.0f;
        float           pointSize = 1.0f;

        bool operator==(const RasterState& o) const
        {
            return cullMode == o.cullMode &&
                frontFace == o.frontFace &&
                polygonMode == o.polygonMode &&
                polygonOffsetFactor == o.polygonOffsetFactor &&
                polygonOffsetUnits == o.polygonOffsetUnits &&
                lineWidth == o.lineWidth &&
                pointSize == o.pointSize;
        }

        bool operator!=(const RasterState& o) const { return !(*this == o); }
    };


    // -----------------------------------------------------------------------
    // PipelineState - pure data, no GL calls
    // -----------------------------------------------------------------------

    /// <summary>
    /// Stores the full logical pipeline state as plain C++ values.
    /// No graphics API calls are made here. Setters return *this for chaining.
    /// Getters are overloaded: the zero-argument form returns the current value,
    /// the one-argument form assigns and returns *this.
    /// </summary>
    class PipelineState
    {
    public:
        PipelineState() {}
        ~PipelineState() {}


        // -------------------------------------------------------------------
        // Capability
        // -------------------------------------------------------------------

        /// <summary>
        /// Returns true if the capability is currently enabled.
        /// </summary>
        inline bool enabled(Capability cap) const
        {
            return _capabilities.count(cap) > 0;
        }

        /// <summary>
        /// Marks a capability as enabled.
        /// </summary>
        inline PipelineState& enable(Capability cap)
        {
            _capabilities.insert(cap);
            return *this;
        }

        /// <summary>
        /// Marks a capability as disabled.
        /// </summary>
        inline PipelineState& disable(Capability cap)
        {
            _capabilities.erase(cap);
            return *this;
        }


        // -------------------------------------------------------------------
        // Blend
        // -------------------------------------------------------------------

        /// <summary>
        /// Returns the current blend state.
        /// </summary>
        inline const BlendState& blend() const { return _blend; }

        /// <summary>
        /// Replaces the entire blend state.
        /// </summary>
        inline PipelineState& blend(const BlendState& state) { _blend = state; return *this; }

        /// <summary>
        /// Sets source and destination blend factors for both colour and alpha channels.
        /// </summary>
        inline PipelineState& blendFunc(BlendFunction src, BlendFunction dst)
        {
            _blend.srcRGB = _blend.srcAlpha = src;
            _blend.dstRGB = _blend.dstAlpha = dst;
            return *this;
        }

        /// <summary>
        /// Sets source and destination blend factors separately for colour and alpha channels.
        /// </summary>
        inline PipelineState& blendFuncSeparate(BlendFunction srcRGB, BlendFunction dstRGB,
            BlendFunction srcAlpha, BlendFunction dstAlpha)
        {
            _blend.srcRGB = srcRGB;
            _blend.dstRGB = dstRGB;
            _blend.srcAlpha = srcAlpha;
            _blend.dstAlpha = dstAlpha;
            return *this;
        }

        /// <summary>
        /// Sets the blend equation for all draw buffers.
        /// </summary>
        inline PipelineState& blendEquation(BlendEquation equation)
        {
            _blend.equation = equation;
            return *this;
        }

        /// <summary>
        /// Sets the constant blend colour. Components are clamped to [0, 1].
        /// </summary>
        inline PipelineState& blendColor(float r, float g, float b, float a)
        {
            _blend.colorR = r;
            _blend.colorG = g;
            _blend.colorB = b;
            _blend.colorA = a;
            return *this;
        }


        // -------------------------------------------------------------------
        // Depth
        // -------------------------------------------------------------------

        /// <summary>
        /// Returns the current depth state.
        /// </summary>
        inline const DepthState& depth() const { return _depth; }

        /// <summary>
        /// Replaces the entire depth state.
        /// </summary>
        inline PipelineState& depth(const DepthState& state) { _depth = state; return *this; }

        /// <summary>
        /// Sets the depth comparison function.
        /// </summary>
        inline PipelineState& depthFunc(Function func)
        {
            _depth.func = func;
            return *this;
        }

        /// <summary>
        /// Maps normalised device depth values to window-space depth values.
        /// near and far are clamped to [0, 1].
        /// </summary>
        inline PipelineState& depthRange(double near, double depthFar)
        {
            _depth.depthNear = near;
            _depth.depthFar = depthFar;
            return *this;
        }


        // -------------------------------------------------------------------
        // Stencil
        // -------------------------------------------------------------------

        /// <summary>
        /// Returns the current stencil state for both faces.
        /// </summary>
        inline const StencilState& stencil() const { return _stencil; }

        /// <summary>
        /// Replaces the entire stencil state.
        /// </summary>
        inline PipelineState& stencil(const StencilState& state) { _stencil = state; return *this; }

        /// <summary>
        /// Sets the stencil comparison function, reference value, and read mask for both faces.
        /// </summary>
        inline PipelineState& stencilFunc(Function func, int32_t ref, uint32_t mask)
        {
            _stencil.front.func = _stencil.back.func = func;
            _stencil.front.ref = _stencil.back.ref = ref;
            _stencil.front.readMask = _stencil.back.readMask = mask;
            return *this;
        }

        /// <summary>
        /// Sets the stencil comparison function, reference value, and read mask
        /// for the specified face(s).
        /// </summary>
        inline PipelineState& stencilFuncSeparate(FaceSelector face, Function func,
            int32_t ref, uint32_t mask)
        {
            auto apply = [&](StencilFaceState& f) {
                f.func = func;
                f.ref = ref;
                f.readMask = mask;
                };
            if (face == FaceSelector::Front || face == FaceSelector::FrontAndBack) apply(_stencil.front);
            if (face == FaceSelector::Back || face == FaceSelector::FrontAndBack) apply(_stencil.back);
            return *this;
        }

        /// <summary>
        /// Sets the stencil operations for both faces.
        /// </summary>
        inline PipelineState& stencilOp(StencilOperation sfail,
            StencilOperation dpfail,
            StencilOperation dppass)
        {
            _stencil.front.sfail = _stencil.back.sfail = sfail;
            _stencil.front.dpfail = _stencil.back.dpfail = dpfail;
            _stencil.front.dppass = _stencil.back.dppass = dppass;
            return *this;
        }

        /// <summary>
        /// Sets the stencil operations for the specified face(s).
        /// </summary>
        inline PipelineState& stencilOpSeparate(FaceSelector face,
            StencilOperation sfail,
            StencilOperation dpfail,
            StencilOperation dppass)
        {
            auto apply = [&](StencilFaceState& f) {
                f.sfail = sfail;
                f.dpfail = dpfail;
                f.dppass = dppass;
                };
            if (face == FaceSelector::Front || face == FaceSelector::FrontAndBack) apply(_stencil.front);
            if (face == FaceSelector::Back || face == FaceSelector::FrontAndBack) apply(_stencil.back);
            return *this;
        }

        /// <summary>
        /// Sets the stencil write mask for both faces.
        /// </summary>
        inline PipelineState& stencilMask(uint32_t mask)
        {
            _stencil.front.writeMask = _stencil.back.writeMask = mask;
            return *this;
        }

        /// <summary>
        /// Sets the stencil write mask for the specified face(s).
        /// </summary>
        inline PipelineState& stencilMaskSeparate(FaceSelector face, uint32_t mask)
        {
            if (face == FaceSelector::Front || face == FaceSelector::FrontAndBack) _stencil.front.writeMask = mask;
            if (face == FaceSelector::Back || face == FaceSelector::FrontAndBack) _stencil.back.writeMask = mask;
            return *this;
        }


        // -------------------------------------------------------------------
        // Rasterisation
        // -------------------------------------------------------------------

        /// <summary>
        /// Returns the current rasterisation state.
        /// </summary>
        inline const RasterState& raster() const { return _raster; }

        /// <summary>
        /// Replaces the entire rasterisation state.
        /// </summary>
        inline PipelineState& raster(const RasterState& state) { _raster = state; return *this; }

        /// <summary>
        /// Sets the face culling mode.
        /// </summary>
        inline PipelineState& cullFace(CullMode mode)
        {
            _raster.cullMode = mode;
            return *this;
        }

        /// <summary>
        /// Sets the vertex winding order considered front-facing.
        /// </summary>
        inline PipelineState& frontFace(FaceDirection direction)
        {
            _raster.frontFace = direction;
            return *this;
        }

        /// <summary>
        /// Sets the rasterisation mode. Applies to both faces.
        /// </summary>
        inline PipelineState& polygonMode(PolygonMode mode)
        {
            _raster.polygonMode = mode;
            return *this;
        }

        /// <summary>
        /// Sets the scale and bias applied to fragment depth values.
        /// </summary>
        inline PipelineState& polygonOffset(float factor, float units)
        {
            _raster.polygonOffsetFactor = factor;
            _raster.polygonOffsetUnits = units;
            return *this;
        }

        /// <summary>
        /// Sets the width of rasterised lines in pixels.
        /// </summary>
        inline PipelineState& lineWidth(float width)
        {
            _raster.lineWidth = width;
            return *this;
        }

        /// <summary>
        /// Sets the diameter of rasterised points in pixels.
        /// </summary>
        inline PipelineState& pointSize(float size)
        {
            _raster.pointSize = size;
            return *this;
        }


        // -------------------------------------------------------------------
        // Logic operation
        // -------------------------------------------------------------------

        /// <summary>
        /// Returns the current logic operation.
        /// </summary>
        inline LogicOperation logicOp() const { return _logicOp; }

        /// <summary>
        /// Sets the logical operation applied to colour fragments.
        /// </summary>
        inline PipelineState& logicOp(LogicOperation op)
        {
            _logicOp = op;
            return *this;
        }

    private:
        std::unordered_set<Capability>  _capabilities;
        BlendState                      _blend;
        DepthState                      _depth;
        StencilState                    _stencil;
        RasterState                     _raster;
        LogicOperation                  _logicOp = LogicOperation::Copy;
    };

}  // namespace medusa
