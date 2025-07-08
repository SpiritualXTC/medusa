#pragma once

#include <format>
#include <strstream>

#include <medusa/math/matrix.h>

namespace medusa
{

    /// <summary>
    ///
    /// </summary>
    class Matrix : virtual public IMatrix
    {
    public:
        Matrix() {}
        virtual ~Matrix() {}

        // Position
        const inline glm::vec3& position() const { return _position; }

        inline Matrix& position(const glm::vec3& v)
        {
            _position = { v.x, v.y, v.z };

            mark_dirty();
            return *this;
        }

        inline Matrix& position(float x, float y, float z)
        {
            _position = { x, y, z };

            mark_dirty();
            return *this;
        }


        // Rotation
        const inline float pitch() const { return _rotation.x; }
        const inline float yaw() const { return _rotation.y; }
        const inline float roll() const { return _rotation.z; }

        // Rotate the matrix
        inline Matrix& yaw(float yaw)
        {
            _rotation.y = yaw;

            mark_dirty();
            return *this;
        }
        inline Matrix& pitch(float pitch)
        {
            _rotation.x = pitch;

            mark_dirty();
            return *this;
        }
        inline Matrix& roll(float roll)
        {
            _rotation.z = roll;

            mark_dirty();
            return *this;
        }


    protected:
        // Calculates the Matrix
        glm::mat4 calculate() override
        {
            glm::mat4 matrix(1.0f);

            // Translation
            matrix = glm::translate(matrix, _position);

            // Rotation
            matrix = glm::rotate(matrix, yaw(), glm::vec3(0.0f, 1.0f, 0.0f));
            matrix = glm::rotate(matrix, roll(), glm::vec3(0.0f, 0.0f, 1.0f));
            matrix = glm::rotate(matrix, pitch(), glm::vec3(1.0f, 0.0f, 0.0f));

            // Scale

            return matrix;
        }
    private:
        glm::vec3 _position{ 0.0f, 0.0f, 0.0f };
        glm::vec3 _rotation{ 0.0f, 0.0f, 0.0f };
    };
}
