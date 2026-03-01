#pragma once

#include <medusa/graphics/containers.h>
#include <medusa/math.h>

namespace medusa
{
    typedef GenericArray<glm::mat4> TransformBuffer;


    class ITransform
    {
    public:
        /// <summary>
        /// Get the index in the transform buffer
        /// </summary>
        /// <returns></returns>
        virtual inline uint32_t transformIndex() = 0;
    };


    /// <summary>
    /// Manages interactions with the transform buffer
    /// </summary>
    class Transform : public ITransform
    {
    public:
        Transform(const glm::mat4& matrix, std::shared_ptr<TransformBuffer> transformBuffer)
            : _transformBuffer(transformBuffer)
        {
            if (!transformBuffer)
                throw MedusaError("No transform buffer");

            _transformIndex = transformBuffer->insert(matrix);
        }
        virtual ~Transform()
        {
            if (_transformIndex != -1)
            {
                // TODO: Delete
                auto transformBuffer = _transformBuffer.lock();
                transformBuffer->erase(_transformIndex);
            }
        }

        /// <summary>
        /// Get the index in the transform buffer
        /// </summary>
        /// <returns></returns>
        inline uint32_t transformIndex() override { return _transformIndex; }


        /// <summary>
        /// Updates the transform buffer with a new matrix
        /// </summary>
        /// <param name="matrix"></param>
        void pushTransform(const glm::mat4& matrix)
        {
            if (_transformIndex != -1)
            {
                auto tb = _transformBuffer.lock();
                auto& m = tb->data(_transformIndex);
                m = matrix;
            }
        }

    private:
        uint32_t _transformIndex = -1;

        std::weak_ptr<TransformBuffer> _transformBuffer;
    };

}
