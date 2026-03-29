// src/opengl/pipeline_state_gl.h
#pragma once

#include <medusa/medusa.h>
#include <medusa/renderer/pipeline_state.h>


namespace medusa
{
    namespace opengl
    {
        /// <summary>
        /// Owns the authoritative GL pipeline state for a single OpenGL context.
        /// Holds a cached PipelineState snapshot; apply() diffs the incoming
        /// state against the cache and issues only the gl* calls required
        /// to bring the driver in sync.
        ///
        /// push() saves the current state onto an internal stack.
        /// pop() restores the most recently pushed state and applies it.
        /// </summary>
        class PipelineStateGL
        {
        public:
            PipelineStateGL();
            ~PipelineStateGL();

            /// <summary>
            /// Diffs state against the internal cache and issues the minimum
            /// set of gl* calls required to bring the driver in sync.
            /// </summary>
            void apply(const PipelineState& state);

            /// <summary>
            /// Returns the last state successfully applied to the driver.
            /// </summary>
            inline const PipelineState& current() const { return _current; }

            /// <summary>
            /// Pushes a copy of the current state onto the stack.
            /// </summary>
            void push();

            /// <summary>
            /// Restores the most recently pushed state and applies it to the driver.
            /// Throws if the stack is empty.
            /// </summary>
            void pop();

            /// <summary>
            /// Returns the number of states currently on the stack.
            /// </summary>
            inline size_t depth() const { return _stack.size(); }

        private:
            void applyCapabilities(const PipelineState& next);
            void applyBlend(const BlendState& next);
            void applyDepth(const DepthState& next);
            void applyStencil(const StencilState& next);
            void applyRaster(const RasterState& next);
            void applyLogicOp(LogicOperation next);

            PipelineState               _current;
            std::stack<PipelineState>   _stack;
        };

    }  // namespace opengl
}  // namespace medusa
