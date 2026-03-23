#pragma once

#include <medusa/medusa.h>
#include <medusa/engine_fwd.h>
#include <medusa/graphics_fwd.h>

#include <medusa/graphics/containers.h>


namespace medusa
{
    class IRenderer;

    class IContext
    {
    public:
        IContext() {}
        virtual ~IContext() {}

        virtual std::shared_ptr<IWindow> window() = 0;

        /// <summary>
        ///
        /// </summary>
        /// <returns></returns>
        virtual bool next() = 0;


        /// <summary>
        ///
        /// </summary>
        /// <returns></returns>
        virtual bool present() = 0;



        virtual void setClearColour(float r, float g, float b, float a) = 0;
        virtual void setClearDepth(float f) = 0;
        virtual void setClearStencil(int32_t i) = 0;

        /// <summary>
        ///
        /// </summary>
        /// <param name="x"></param>
        /// <param name="y"></param>
        /// <param name="width"></param>
        /// <param name="height"></param>
        virtual void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;


        /// <summary>
        ///
        /// </summary>
        /// <returns></returns>
        virtual std::shared_ptr<IShader> createShader() = 0;


        /// <summary>
        ///
        /// </summary>
        /// <returns></returns>
        virtual std::shared_ptr<IDescriptor> createDescriptor() = 0;



        /// <summary>
        ///
        /// </summary>
        /// <param name="bufferType"></param>
        /// <param name="usage"></param>
        /// <returns></returns>
        virtual std::shared_ptr<IMemory> createMemory(BufferType bufferType, BufferUsage usage) = 0;


        /// <summary>
        ///
        /// </summary>
        /// <typeparam name="S"></typeparam>
        /// <param name="type"></param>
        /// <param name="usage"></param>
        /// <param name="ptr"></param>
        /// <returns></returns>
        template <typename S>
        std::shared_ptr<GenericBuffer<S>> createBuffer(BufferType type, BufferUsage usage, const S* ptr)
        {
            auto memory = createMemory(type, usage);

            return std::make_shared<GenericBuffer<S>>(memory, ptr);
        }


        /// <summary>
        ///
        /// </summary>
        /// <typeparam name="S"></typeparam>
        /// <param name="type"></param>
        /// <param name="usage"></param>
        /// <param name="ptr"></param>
        /// <param name="count"></param>
        /// <returns></returns>
        template <typename S>
        std::shared_ptr<GenericArray<S>> createArray(BufferType type, BufferUsage usage, const S* ptr = nullptr, size_t count = 0)
        {
            auto memory = createMemory(type, usage);

            return std::make_shared<GenericArray<S>>(memory, ptr, count);
        }


        /// <summary>
        ///
        /// </summary>
        /// <typeparam name="S"></typeparam>
        /// <param name="type"></param>
        /// <param name="usage"></param>
        /// <returns></returns>
        template <typename S>
        std::shared_ptr<GenericMap<S>> createMap(BufferType type, BufferUsage usage)
        {
            auto memory = createMemory(type, usage);

            return std::make_shared<GenericMap<S>>(memory);
        }


        /// <summary>
        ///
        /// </summary>
        /// <param name="usage"></param>
        /// <returns></returns>
        std::shared_ptr<VertexBuffer> createVertexBuffer(BufferUsage usage)
        {
            return std::make_shared<VertexBuffer>(createMemory(BufferType::Array, usage));
        }


        /// <summary>
        ///
        /// </summary>
        /// <param name="usage"></param>
        /// <returns></returns>
        std::shared_ptr<IndexBuffer> createIndexBuffer(BufferUsage usage)
        {
            return std::make_shared<IndexBuffer>(createMemory(BufferType::ElementArray, usage));
        }


        /// <summary>
        ///
        /// </summary>
        /// <returns></returns>
        virtual std::shared_ptr<ITexture> createTexture() = 0;

    private:

    };
}
