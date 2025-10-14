#pragma once

#include <medusa/medusa.h>
#include <medusa/engine/state.h>
#include <medusa/graphics_fwd.h>

#include <medusa/graphics/memory.h>
#include <medusa/graphics/buffer.h>
#include <medusa/graphics/containers.h>


namespace medusa
{
    class IWindow;
    class IRenderer;

    class IContext : public IState
    {
    public:
        IContext() {}
        virtual ~IContext() {}

        virtual std::shared_ptr<IWindow> window() = 0;
        virtual std::shared_ptr<IRenderer> renderer() = 0;

        virtual bool next() = 0;
        virtual bool present() = 0;

        // Factory
        virtual std::shared_ptr<IShader> createShader() = 0;

        virtual std::shared_ptr<IDescriptor> createDescriptor() = 0;
        virtual std::shared_ptr<ITexture> createTexture() = 0;

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

    private:

    };
}
