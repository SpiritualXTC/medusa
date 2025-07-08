#pragma once

#include <medusa/medusa.h>
#include <medusa/math.h>

namespace medusa
{
    class IMatrix
    {
    public:
        IMatrix() {}
        virtual ~IMatrix() {}

        // Get the Matrix
        inline const glm::mat4& matrix()
        {
            if (_dirty)
            {
                _matrix = calculate();
                _dirty = false;
            }

            return _matrix;
        }


        IMatrix& identity()
        {
            _matrix = glm::identity<glm::mat4>();
        }



        operator std::string()
        {
            std::stringstream ss;
            auto m = matrix();

            auto f = std::format("[\n\t{:4f}\t{:4f}\t{:4f}\t{:4f}\n\t{:4f}\t{:4f}\t{:4f}\t{:4f}\n\t{:4f}\t{:4f}\t{:4f}\t{:4f}\n\t{:4f}\t{:4f}\t{:4f}\t{:4f}\n]",
                m[0][0], m[0][1], m[0][2], m[0][3],
                m[1][0], m[1][1], m[1][2], m[1][3],
                m[2][0], m[2][1], m[2][2], m[2][3],
                m[3][0], m[3][1], m[3][2], m[3][3]);

            return f;
        }

    protected:
        virtual glm::mat4 calculate() = 0;
        inline void mark_dirty() { _dirty = true; }

    private:
        bool _dirty = true;
        glm::mat4 _matrix{ 1.0 };
    };
}
