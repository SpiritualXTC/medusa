#pragma once

#include <medusa/medusa.h>
#include <medusa/graphics/buffer.h>

namespace medusa
{
    typedef uint32_t ResourceIndex;


    /// <summary>
    /// Interface for the memory View
    /// </summary>
    class IMemoryView
    {
    public:
        // Binding
        virtual bool bind() const = 0;
        virtual bool unbind() const = 0;

    };


    /// <summary>
    /// Interface for a block of memory
    /// </summary>
    class IMemory: public IMemoryView
    {
    public:
        IMemory(BufferType bufferType, BufferUsage usage, ResourceIndex index = -1)
            : _type(bufferType)
            , _usage(usage)
            , _index(index)
        {

        }

        virtual ~IMemory() {}

        const inline BufferType type() const { return _type; }
        const inline BufferUsage usage() const { return _usage; }
        const inline ResourceIndex index() const { return _index; }

        // Accessors
        virtual const inline void* handle() const = 0;
        virtual const inline size_t size() const = 0;


        /// <summary>
        /// Allocate the buffer
        /// </summary>
        /// <param name="ptr"></param>
        /// <param name="size"></param>
        /// <param name="usage"></param>
        /// <returns></returns>
        virtual bool allocate(size_t size, const void* ptr = nullptr) = 0;

        /// <summary>
        /// Write to the buffer
        /// </summary>
        /// <param name="ptr"></param>
        /// <param name="offset"></param>
        /// <param name="size"></param>
        /// <returns></returns>
        virtual bool write(size_t offset, size_t size, const void* ptr) = 0;


        /// <summary>
        ///
        /// </summary>
        /// <param name="ptr"></param>
        /// <param name="offset"></param>
        /// <param name="size"></param>
        /// <returns></returns>
        virtual bool read(size_t offset, size_t size, void* ptr) = 0;


        /// <summary>
        ///
        /// </summary>
        /// <param name="src_offset"></param>
        /// <param name="dst_offset"></param>
        /// <param name="size"></param>
        /// <param name="memory"></param>
        /// <returns></returns>
        virtual bool copy(size_t src_offset, size_t dst_offset, size_t size, std::shared_ptr<IMemory> memory) = 0;

    private:
        BufferType _type = (BufferType)0;
        BufferUsage _usage = (BufferUsage)0;

        ResourceIndex _index = -1;
    };





    /// <summary>
    /// View of a block of memory
    /// </summary>
    class MemoryView : public IMemoryView
    {
    public:
        MemoryView(std::shared_ptr<IMemory> memory)
            : MemoryView()

        {
            _memory = memory;
            assert(_memory != nullptr);
        }

        virtual ~MemoryView() {}


        inline const std::shared_ptr<IMemory> memory() const { return _memory; }


        inline const BufferType type() const { return _memory->type(); }
        inline const BufferUsage usage() const { return _memory->usage(); }
        inline const  ResourceIndex index() const { return _memory->index(); }
        inline const void* handle() const { return _memory->handle(); }



        inline const size_t size() const { return _memory->size(); }

        // TODO: Proper locking mechanism


        /// <summary>
        ///
        /// </summary>
        /// <returns></returns>
        inline bool bind() const override { return _memory->bind(); }


        /// <summary>
        ///
        /// </summary>
        /// <returns></returns>
        inline bool unbind() const override { return _memory->unbind(); }


        /// <summary>
        /// Syncs the offline data back to the buffer // primitive method :)
        /// </summary>
        /// <returns></returns>
        virtual bool sync() { return false; }

    protected:
        MemoryView()
        {
            // Seems to make the abstractions happier
        } // needed?

    private:

        std::shared_ptr<IMemory> _memory;
    };
}  // namespace medusa
