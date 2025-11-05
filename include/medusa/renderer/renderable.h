#pragma once

#include <medusa/medusa.h>

#include <core/utilities/logging.h>

namespace medusa
{
    class IRenderable;
    class IPass;


    class IRenderable
    {
    public:
        IRenderable() {}
        virtual ~IRenderable() {}

        virtual inline uint64_t resource_id() = 0;

        virtual bool render() = 0;

        // Getters
        inline virtual const glm::mat4& transform() = 0;
    };



    /// <summary>
    ///
    /// </summary>
    class IRenderableContainer
    {
    public:
        IRenderableContainer() {}
        virtual ~IRenderableContainer() {}

        /// <summary>
        /// Add an item to the iterable
        /// </summary>
        /// <param name="renderable"></param>
        /// <returns></returns>
        virtual bool add(std::shared_ptr<IRenderable> renderable) = 0;


        /// <summary>
        /// Clear the iterable
        /// </summary>
        /// <returns></returns>
        virtual bool clear() = 0;


        /// <summary>
        /// Render all the items in the iterable
        /// </summary>
        /// <returns></returns>
        virtual bool render() = 0;
    };
}
