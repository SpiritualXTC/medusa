#pragma once

#include <medusa/medusa.h>

#include <core/utilities/logging.h>

namespace medusa
{
    // TODO: The Container SHOULD be an IRenderable, however the resource_id() (and the transform()) messes it up... requires an further derived class above IRenderable


    class IRenderable
    {
    public:
        IRenderable() {}
        virtual ~IRenderable() {}


        /// <summary>
        ///
        /// </summary>
        /// <returns></returns>
        virtual inline uint64_t resource_id() = 0;


        /// <summary>
        ///
        /// </summary>
        /// <returns></returns>
        virtual bool render() = 0;


        /// <summary>
        ///
        /// </summary>
        /// <returns></returns>
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
        ///
        /// </summary>
        /// <returns></returns>
        virtual bool render() = 0;
    };
}
