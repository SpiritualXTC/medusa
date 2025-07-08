#pragma once

#include "matrix.h"

namespace medusa
{
    class ProjectionMatrix : public IMatrix
    {
    public:
        //
        ProjectionMatrix() {}

        //
        virtual ~ProjectionMatrix() {}


        // Set Near Clipping Plane
        inline ProjectionMatrix& zNear(float zN)
        {
            _near = zN;
            mark_dirty();
            return *this;
        }

        // Set Far Clipping Plane
        inline ProjectionMatrix& zFar(float zF)
        {
            _far = zF;
            mark_dirty();
            return *this;
        }

        inline ProjectionMatrix& resize(float w, float h)
        {
            _width = w;
            _height = h;
            _aspect = w / h;
            mark_dirty();
            return *this;
        }


        // Get Near Clipping Plane
        inline float width() const { return _width; }

        // Get Far Clipping Plane
        inline float height() const { return _height; }

        // Get Aspect Ratio
        inline float aspectRatio() const { return _width / _height; }

        // Get Near Clipping Plane
        inline float zNear() const { return _near; }

        // Get Far Clipping Plane
        inline float zFar() const { return _far; }


    private:

        // Target
        float _width = 1.0f;
        float _height = 1.0f;
        float _aspect = 1.0f;

        // Clipping Planes
        float _near = 0.1f;
        float _far = 1000.0f;

        float _size = 1.0f;
    };


    /// <summary>
    ///
    /// </summary>
    class PerspectiveMatrix : public ProjectionMatrix
    {
    public:
        // Set Field of Vision [Fov]
        inline ProjectionMatrix& fov(float fov)
        {
            _fov = fov;
            mark_dirty();
            return *this;
        }

        // Set Zoom
        inline ProjectionMatrix& zoom(float z)
        {
            _zoom = z;
            mark_dirty();
            return *this;
        }

        inline float fov() const { return _fov; }

        // Gets the Zoom
        inline float zoom() const { return _zoom; }


    protected:
        glm::mat4 calculate() override
        {
            glm::mat4 proj = glm::perspectiveFov(fov(), width(), height(), zNear(), zFar());

            glm::mat4 m(1.0f);

            m = glm::scale(m, glm::vec3(zoom(), zoom(), 1.0f));

            return proj * m;
        }

    private:
        float _fov = 1.0f;
        float _zoom = 1.0f;

    };


    /// <summary>
    ///
    /// </summary>
    class OrthogonalMatrix : public ProjectionMatrix
    {
    public:

        // Set Orthogonal Matrix Size :: The Size is a multiplier as the orthogonal matrix is bounded [-aspect, aspect, -1, 1]
        inline OrthogonalMatrix& size(float size)
        {
            _size = size;
            mark_dirty();
            return *this;
        }

        inline float size() { return _size; }

    protected:
        // Calculates the Matrix
        glm::mat4 calculate() override
        {
            float x = aspectRatio() * size();
            float y = size();

            glm::mat4 m = glm::ortho(-x, x, -y, y, zNear(), zFar());
            return m;
        }

    private:
        float _size = 1.0f;
    };
}
