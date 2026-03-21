#pragma once

#include <medusa/medusa.h>

//#include "geometry.h"
#include <engine/geometry/cube.h>
#include <engine/geometry/cylinder.h>
#include <engine/geometry/icosphere.h>
#include <engine/geometry/torus.h>
#include <engine/geometry/teapot.h>

namespace medusa
{
    class GeometryFactory
    {
    public:
        GeometryFactory() = delete;

        /// <summary>
        ///
        /// </summary>
        /// <param name="width"></param>
        /// <param name="height"></param>
        /// <param name="depth"></param>
        /// <returns></returns>
        static std::shared_ptr<Cube> createCube(float width = 1.0f, float height = 1.0f, float depth = 1.0f);

        /// <summary>
        ///
        /// </summary>
        /// <param name="radius"></param>
        /// <param name="height"></param>
        /// <param name="segments"></param>
        /// <param name="stacks"></param>
        /// <returns></returns>
        static std::shared_ptr<Cylinder> createCylinder(float radius = 1.0f, float height = 2.0f, uint32_t segments = 32, uint32_t stacks = 1);


        /// <summary>
        ///
        /// </summary>
        /// <param name="outerRadius"></param>
        /// <param name="innerRadius"></param>
        /// <param name="height"></param>
        /// <param name="segments"></param>
        /// <param name="stacks"></param>
        /// <returns></returns>
        static std::shared_ptr<Tube> createTube(float outerRadius = 1.0f, float innerRadius = 0.5f, float height = 2.0f, uint32_t segments = 32, uint32_t stacks = 1);


        /// <summary>
        ///
        /// </summary>
        /// <param name="radius"></param>
        /// <param name="subdivisions"></param>
        /// <returns></returns>
        static std::shared_ptr<Icosphere> createIcosphere(float radius = 1.0f, uint32_t subdivisions = 3);


        /// <summary>
        ///
        /// </summary>
        /// <param name="majorRadius"></param>
        /// <param name="minorRadius"></param>
        /// <param name="majorSegments"></param>
        /// <param name="minorSegments"></param>
        /// <returns></returns>
        static std::shared_ptr<Torus> createTorus(float majorRadius = 1.0f, float minorRadius = 0.25f, uint32_t majorSegments = 32, uint32_t minorSegments = 16);


        /// <summary>
        ///
        /// </summary>
        /// <param name="resolution"></param>
        /// <param name="scale"></param>
        /// <returns></returns>
        static std::shared_ptr<Teapot> createTeapot(uint32_t resolution = 8, float scale = 1.0f);
    };
}
