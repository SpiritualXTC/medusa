#include "model.h"

using namespace medusa;


//
Model::Model()
{

}


//
Model::Model(std::shared_ptr<Geometry> geometry)
{
    // Just copy everything...
    _vertices = geometry->interleave<Vertex>();
    _indices = geometry->getIndices();

    _materials = geometry->getMaterials();
    _textures = geometry->getTextures();
    _modelData = geometry->getModelData();
}


//
Model::~Model()
{

}
