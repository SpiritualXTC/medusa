#include "geometry_factory.h"


using namespace medusa;


//
std::shared_ptr<Cube> GeometryFactory::createCube(float width, float height, float depth)
{
    return std::make_shared<Cube>(width, height, depth);
}


//
std::shared_ptr<Cylinder> GeometryFactory::createCylinder(float radius, float height, uint32_t segments, uint32_t stacks)
{
    return std::make_shared<Cylinder>(radius, height, segments, stacks);
}


//
std::shared_ptr<Tube> GeometryFactory::createTube(float outerRadius, float innerRadius, float height, uint32_t segments, uint32_t stacks)
{
    return std::make_shared<Tube>(outerRadius, innerRadius, height, segments, stacks);
}


//
std::shared_ptr<Icosphere> GeometryFactory::createIcosphere(float radius, uint32_t subdivisions)
{
    return std::make_shared<Icosphere>(radius, subdivisions);
}


//
std::shared_ptr<Torus> GeometryFactory::createTorus(float majorRadius, float minorRadius, uint32_t majorSegments, uint32_t minorSegments)
{
    return std::make_shared<Torus>(majorRadius, minorRadius, majorSegments, minorSegments);
}


//
std::shared_ptr<Teapot> GeometryFactory::createTeapot(uint32_t resolution, float scale)
{
    return std::make_shared<Teapot>(resolution, scale);
}
