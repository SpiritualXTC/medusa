#include "model_loader.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <medusa/graphics/material.h>

#include <core/utilities/logging.h>

#include <engine/geometry/geometry.h>
#include <engine/graphics/model.h>


using namespace medusa;
using namespace medusa::loaders;


ModelLoader::ModelLoader(std::shared_ptr<IContext> context)
    : _context(context)
{

}

ModelLoader::~ModelLoader()
{

}


std::shared_ptr<IMesh> ModelLoader::load(const std::string& filename)
{
    Assimp::Importer importer;

    auto context = _context.lock();

    std::shared_ptr<Model> model = std::make_shared<Model>(context);

    std::vector<glm::vec3> position;
    std::vector<glm::vec3> normals;
    // TODO: Texture Coordinates

    std::vector<uint32_t> indices;

    uint32_t vertexOffset = 0;



    const aiScene* scene = importer.ReadFile(filename.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);



    assert(scene != nullptr);


    logging::info("Model: {}", filename);
    logging::info("- meshes: {}, materials: {}, textures: {}", scene->mNumMeshes, scene->mNumMaterials, scene->mNumTextures);

    // Load Materials
    for (uint32_t matIdx = 0; matIdx < scene->mNumMaterials; ++matIdx)
    {
        auto mat = scene->mMaterials[matIdx];
        auto matName = mat->GetName().C_Str();

        Material material;

        // Read material colour data
        aiColor3D ambient;
        aiColor3D diffuse;
        aiColor3D specular;
        float opacity = 1.0f;

        if (mat->Get(AI_MATKEY_COLOR_AMBIENT, ambient) != AI_SUCCESS)
            logging::warn("Unable to read `ambient` from material {}, idx={}", matName, matIdx);
        if (mat->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse) != AI_SUCCESS)
            logging::warn("Unable to read `diffuse` from material {}, idx={}", matName, matIdx);
        if (mat->Get(AI_MATKEY_COLOR_SPECULAR, specular) != AI_SUCCESS)
            logging::warn("Unable to read `specular` from material {}, idx={}", matName, matIdx);
        if (mat->Get(AI_MATKEY_OPACITY, opacity) != AI_SUCCESS)
            logging::warn("Unable to read `opacity` from material {}, idx={}", matName, matIdx);

        // Read Texture Filenames


        logging::info(fmt::format("ambient={},{},{}", (float)ambient.r, (float)ambient.g, (float)ambient.b));

        // Set Material Data
        material.ambient(ambient.r, ambient.g, ambient.b);
        material.diffuse(diffuse.r, diffuse.g, diffuse.b, opacity);
        material.specular(specular.r, specular.g, specular.b);

        // Set & Load Textures from Resource Database into material

        // Add material
        model->addMaterial(material);
    }


    // Load Mesh Data
    for (uint32_t meshIdx = 0; meshIdx < scene->mNumMeshes; ++meshIdx)
    {
        auto submesh = scene->mMeshes[meshIdx];
        auto submeshName = submesh->mName.C_Str();

        // Position
        if (submesh->HasPositions())
        {
            position.insert(position.end(), (glm::vec3*)submesh->mVertices, (glm::vec3*)submesh->mVertices + submesh->mNumVertices);
        }

        // Normals
        if (submesh->HasNormals())
        {
            normals.insert(normals.end(), (glm::vec3*)submesh->mNormals, (glm::vec3*)submesh->mNormals + submesh->mNumVertices);
        }

        // Textures
        for (uint32_t texIdx = 0; texIdx < submesh->GetNumUVChannels(); ++texIdx)
        {
            if (submesh->HasTextureCoords(texIdx))
            {
                uint32_t channels = submesh->mNumUVComponents[texIdx];

                logging::warn(std::format("Texture UV data found in mesh `{}`, not supported yet", submeshName));
            }
        }

        // Faces
        if (submesh->HasFaces() && submesh->mNumFaces >= 1)
        {

            std::vector<uint32_t> submeshIndices(submesh->mNumFaces * 3);
            uint32_t* ptr = submeshIndices.data();

            for (uint32_t faceIdx = 0; faceIdx < submesh->mNumFaces; ++faceIdx)
            {
                const auto& face = submesh->mFaces[faceIdx];

                if (face.mNumIndices != 3)
                {
                    logging::error(fmt::format("Unsupported number of indices found in face (idx={}), of mesh: `{}`, expected 3, got {}", faceIdx, submeshName, face.mNumIndices));
                    for (uint32_t idx = 0; idx < face.mNumIndices; idx++)
                        logging::error(fmt::format("- {}", face.mIndices[idx]));
                    continue;
                }

                // Copy Indices. Only supporting 3 indices in a face
                *ptr++ = face.mIndices[0] + vertexOffset;
                *ptr++ = face.mIndices[1] + vertexOffset;
                *ptr++ = face.mIndices[2] + vertexOffset;
            }
            indices.insert(indices.end(), submeshIndices.begin(), submeshIndices.end());

            // Increase the vertex offset
            vertexOffset += submesh->mNumVertices;

            // Add submesh data to model
            model->addSubmeshData(submesh->mNumVertices, submeshIndices.size(), submesh->mMaterialIndex);
        }
    }


    Geometry geometry(context);

    // Add geometry to model
    if (position.size())
        geometry.addVertexData(position.data(), position.size(), AttributeLocation::Position);
    if (normals.size())
        geometry.addVertexData(normals.data(), normals.size(), AttributeLocation::Normal);

    geometry.addIndexData(indices.data(), indices.size());

    // Construct model
    geometry.mesh(model);

    return model;
}
