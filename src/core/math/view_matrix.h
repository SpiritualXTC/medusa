#pragma once

#include "matrix.h"

namespace medusa
{
    /// <summary>
    ///
    /// </summary>
    class ViewMatrix : public IMatrix
    {
    public:
        enum _LockFlags
        {
            Unlock = 0x00,

            //TranslateX = 0x0001,
            //TranslateY = 0x0002,
            //TranslateZ = 0x0004,
            //Translation = 0x000F,

            RotatePitch = 0x0010,
            RotateYaw = 0x0020,
            RotateRoll = 0x0040,
            Rotation = 0x00F0,

            Distance = 0x0100,

            Lock = 0xFFFF,
        };

    public:
        ViewMatrix() {}
        virtual ~ViewMatrix() {}


        // Rotation
        inline float pitch() const { return _rotation.x; }
        inline float yaw() const { return _rotation.y; }
        inline float roll() const { return _rotation.z; }

        // Distance
        inline float distance() const { return _distance; }

        // Is any part of the camera locked ??
        inline bool isLocked() const { return _lockFlags != Unlock; }


        // Is any of the Specified Flags Locked ??
        inline bool isLocked(uint32_t flags) const { return (_lockFlags & flags) != Unlock; }


        // Rotate the Camera
        inline ViewMatrix& yaw(float yaw)
        {
            if (!isLocked(RotateYaw))
                _rotation.y = yaw;

            mark_dirty();

            return *this;
        }
        inline ViewMatrix& pitch(float pitch)
        {
            if (!isLocked(RotatePitch))
                _rotation.x = pitch;

            mark_dirty();

            return *this;
        }
        inline ViewMatrix& roll(float roll)
        {
            if (!isLocked(RotateRoll))
                _rotation.z = roll;

            mark_dirty();

            return *this;
        }

        // Distance
        inline ViewMatrix& distance(float distance)
        {
            if (!isLocked(Distance))
                _distance = distance;

            mark_dirty();

            return *this;
        }

        // Locks the Camera
        inline ViewMatrix& lock() { _lockFlags = Lock; }

        // Unlocks the Camera
        inline ViewMatrix& unlock() { _lockFlags = Unlock; }

    protected:
        glm::mat4 calculate() override
        {
            glm::mat4 matrix(1.0f);

            // Distance camera is from view point
            matrix = glm::translate(matrix, glm::vec3(0, 0, -distance()));

            // Camera Rotation
            matrix = glm::rotate(matrix, -pitch(), glm::vec3(1.0f, 0.0f, 0.0f));
            matrix = glm::rotate(matrix, -yaw(), glm::vec3(0.0f, 1.0f, 0.0f));
            matrix = glm::rotate(matrix, -roll(), glm::vec3(0.0f, 0.0f, 1.0f));

            return matrix;
        }

    private:
        // Locks camera movement
        uint32_t _lockFlags = 0;

        float _distance = 1.0f;
        glm::vec3 _rotation{ 0.0f, 0.0f, 0.0f };
    };

}
