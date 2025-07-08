#include "model.h"

#include <medusa/graphics/descriptor.h>
#include <opengl/gl.h> /// TODO: NOOOOO

#include <core/utilities/logging.h>

using namespace medusa;

//
Model::Model(std::shared_ptr<IContext> context)
    : Mesh(context)
{

}


//
Model::~Model()
{

}


//
void Model::addMaterial(Material& material)
{
    _materials.push_back(material);
}


//
void Model::addSubmeshData(uint32_t vertices, uint32_t indices, uint32_t materialIndex)
{
    SubmeshData* last = _submeshes.size() == 0 ? nullptr : &_submeshes[_submeshes.size() - 1];

    SubmeshData submesh;
    submesh.vertexOffset = last == nullptr ? 0 : last->vertexOffset + last->vertexCount;
    submesh.vertexCount = vertices;
    submesh.indexOffset = last == nullptr ? 0 : last->indexOffset + last->indexCount;
    submesh.indexCount = indices;

    submesh.materialIndex = materialIndex;

    _submeshes.push_back(submesh);
}


//
bool Model::render(size_t instances)
{
    descriptor()->bind();


    std::vector<size_t> indicesOffset(_submeshes.size());
    std::vector<GLsizei> indicesCount(_submeshes.size());

    for (uint32_t idx = 0; idx < _submeshes.size(); ++idx)
    {
        indicesOffset[idx] = _submeshes[idx].indexOffset * 4;
        indicesCount[idx] = _submeshes[idx].indexCount;


    }

    glMultiDrawElements(GL_TRIANGLES, (GLsizei*)indicesCount.data(), GL_UNSIGNED_INT, (void**)indicesOffset.data(), _submeshes.size());

    descriptor()->unbind();

    //bool result = Mesh::render(instances);

    return true;
}
