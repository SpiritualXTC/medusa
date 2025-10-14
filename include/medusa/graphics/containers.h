#pragma once

#include <medusa/medusa.h>
#include <medusa/graphics/memory.h>

#include <core/utilities/logging.h>

namespace medusa
{
    /// <summary>
    /// Buffer view of memory
    /// </summary>
    template <typename S>
    class GenericBuffer : public MemoryView
    {
    public:
        GenericBuffer(std::shared_ptr<IMemory> mem, const S* ptr = nullptr)
            : MemoryView(mem)
            , _data({})
        {
            if (ptr)
                _data = *ptr;
            memory()->allocate(sizeof(S), ptr);
        }

        virtual ~GenericBuffer(){}

        inline S& data() { return _data; }
        const inline S& data() const { return _data; }

        bool sync() override
        {
            return memory()->write(0, sizeof(S), (void*)&_data);
        }

    private:

        S _data;
    };


    /// <summary>
    /// Array view of memory
    /// </summary>
    template <typename S>
    class GenericArray : public MemoryView
    {
    public:
        GenericArray(std::shared_ptr<IMemory> mem, const S* ptr = nullptr, size_t count = 0)
            : MemoryView(mem)
        {
            _stride = sizeof(S);

            if (count > 0)
                allocate(ptr, count);
        }


        /// <summary>
        ///
        /// </summary>
        /// <param name="ptr"></param>
        /// <param name="count"></param>
        /// <returns></returns>
        inline const bool allocate(const S* ptr = nullptr, size_t count = 0)
        {
            reset();

            //
            if (ptr != nullptr)
            {
                // Allocate and copy
                _data.resize(count);

                for (size_t i = 0; i < count; ++i)
                    _data[i] = ptr[i];
                _next = count;
                _active = count;
            }
            else if (count != 0)
            {
                // Preallocate
                _data.reserve(count);
            }

            // Allocate
            memory()->allocate(_stride * _data.capacity(), _data.data());

            return true;
        }


        /// <summary>
        ///
        /// </summary>
        /// <param name="v"></param>
        /// <returns></returns>
        inline const bool allocate(const std::vector<S>& v)
        {
            return allocate(v.data(), v.size());
        }


        inline const size_t stride() const { return _stride; }
        inline const size_t capacity() const { return _data.capacity(); }
        inline const size_t elements() const { return _active; }


        inline const size_t size() const { return _data.size(); }
        inline const size_t next() const { return _next; }


        inline S& data(size_t index) { return _data[index]; }
        const inline S& data(size_t index) const { return _data[index]; }

        bool sync() override
        {
            return memory()->write(0, stride() * elements(), (void*)_data.data());
        }

        bool sync(size_t index)
        {
            return memory()->write(stride() * index, stride(), (void*)&_data[index]);
        }

        bool reset()
        {
            _next = 0;
            _active = 0;
            _erased.clear();
            return true;
        }


        /// <summary>
        ///
        /// </summary>
        /// <param name="value"></param>
        /// <returns></returns>
        size_t insert(const S& value)
        {
            size_t idx = -1;

            if (_erased.size() > 0)
            {
                // Override an erased item
                size_t idx = _erased.back();
                _erased.pop_back();
                _data[idx] = value;

                sync(idx);
                _active++;
            }
            else if (next() < size())
            {
                // The next index is still able to be assigned inline
                idx = _next++;
                _data[idx] = value;

                sync(idx);
                _active++;
            }
            else
            {
                // Need to push. (vector handles adjusting the capacity)
                idx = _next++;
                _data.push_back(value);

                // Allocate :: This is a lazy reallocation TODO: Resize on the GPU with staging buffer
                memory()->allocate(stride() * _data.capacity(), _data.data());
                _active++;
            }

            return idx;
        }


        const std::vector<S>& buffer() const { return _data; }

    private:
        // TODO: erase() MUST decrease _active...

        size_t _stride = 0;

        size_t _next = 0;
        size_t _active = 0;

        std::vector<S> _data;
        std::vector<size_t> _erased;
    };





    /// <summary>
    /// Array view of memory, with elements getting a name
    /// </summary>
    template <typename S>
    class GenericMap : public GenericArray<S>
    {
    public:
        GenericMap(std::shared_ptr<IMemory> mem)
            : GenericArray<S>(mem)
        {


        }

        /// <summary>
        /// Determine whether the key exists in the map
        /// </summary>
        /// <param name="name"></param>
        /// <returns></returns>
        virtual bool exists(const std::string& name) const
        {
            auto it = _map.find(name);
            return it != _map.end();
        }


        /// <summary>
        /// Gets the index of the item with the name
        /// </summary>
        /// <param name="name"></param>
        /// <returns></returns>
        size_t index(const std::string& name) const
        {
            auto it = _map.find(name);

            return it != _map.end() ? it->second : -1;
        }


        inline S& data(const std::string& name)
        {
            // TODO: These are highly unstable. lol
            auto idx = index(name);
            return GenericArray<S>::data(idx);
        }

        const inline S& data(const std::string& name) const
        {
            // TODO: These are highly unstable. lol
            auto idx = index(name);
            return GenericArray<S>::data(idx);
        }


        /// <summary>
        ///
        /// </summary>
        /// <param name="name"></param>
        /// <param name="s"></param>
        /// <returns></returns>
        size_t insert(const std::string& name, const S& s)
        {
            size_t idx = GenericArray<S>::insert(s);

            _map.insert({ name, idx });

            return idx;
        }

    private:
        std::unordered_map<std::string, size_t> _map;
    };


    /// Specialisations

    struct Vertex
    {
        glm::vec3 position;
        glm::vec3 normal;
        //glm::vec2 texture;
    };



    //template<typename V>
    // TODO: This should be able to to handle any Vertex
    //  -- The problem is the association of a vertex structure alongside the geometry construction
    class VertexBuffer : public GenericArray<Vertex>
    {
    public:
        VertexBuffer(std::shared_ptr<IMemory> mem)
            : GenericArray<Vertex>(mem)
        {

        }

        const size_t vertices() const { return this->elements(); }
    };


    class IndexBuffer : public GenericArray<uint32_t>
    {
    public:
        IndexBuffer(std::shared_ptr<IMemory> mem)
            : GenericArray<uint32_t>(mem)
        {

        }

        const size_t indices() const { return this->elements(); }
    };
} // namespace medusa
