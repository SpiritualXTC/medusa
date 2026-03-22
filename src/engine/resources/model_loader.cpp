#include "model_loader.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/material.h>

#include <medusa/engine/context.h>
#include <medusa/graphics.h>
#include <medusa/graphics/containers.h>

#include <core/utilities/logging.h>

#include <engine/geometry/geometry.h>

#include <engine/resources/texture_loader.h>


using namespace medusa;
using namespace medusa::loaders;


//
std::shared_ptr<Geometry> ModelLoader::loadModel(std::shared_ptr<IContext> context, const std::string& filename)
{
    Assimp::Importer importer;

    // Model Data
    std::shared_ptr<Geometry> model = std::make_shared<Geometry>();

    // Vertex Data
    std::vector<glm::vec3> position;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> textureCoords;
    std::vector<uint32_t> materialIndices;

    // Face Data
    std::vector<uint32_t> indices;

    // Mesh Data
    ModelData modelData{ 0, 0, 0, 0, 0 };
    std::vector<size_t> materialIndex;

    uint32_t vertexOffset = 0;

    // Load *.obj file
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

        // Set Material Data
        material.ambient(ambient.r, ambient.g, ambient.b);
        material.diffuse(diffuse.r, diffuse.g, diffuse.b, opacity);
        material.specular(specular.r, specular.g, specular.b);

        // Load Textures [Currently only Diffuse Texturing is enabled]
        aiString matTextureDiffuse;
        if (mat->GetTexture(aiTextureType_DIFFUSE, 0, &matTextureDiffuse) != AI_SUCCESS)
            logging::warn("Unable to read TextureBlend Diffuse from material {}, idx={}, {}", matName, matIdx, matTextureDiffuse.C_Str());
        else
        {
            std::string name = matTextureDiffuse.C_Str();

            std::shared_ptr<ITexture> tex = loaders::TextureLoader::loadTexture2D(context, name);

            uint64_t handle = model->addTexture(name, tex);

            material.diffuseTexture(handle);

            // Load the texture, assign the handle to the material
            logging::info(std::format("Material `{}` has {} textures, tex0=`{}`", matName, mat->GetTextureCount(aiTextureType_DIFFUSE), matTextureDiffuse.C_Str()));
        }

        // Add material
        auto idx = model->addMaterial(material);

        // Map the material name -> index
        materialIndex.push_back(idx);

        logging::debug(fmt::format("Material: name={}, index={}->{}", matName, matIdx, idx));
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
        uint32_t textureCoordCount = 0;
        for (uint32_t texIdx = 0; texIdx < submesh->GetNumUVChannels(); ++texIdx)
        {

            if (submesh->HasTextureCoords(texIdx))
            {
                // Only supporting one texture index for now
                if (textureCoordCount >= 1)
                {
                    logging::warn(std::format("Only the first set of 2D texture coordinates are being used `{}`", submeshName));
                    break;
                }
                uint32_t channels = submesh->mNumUVComponents[texIdx];

                // Only supporting 2D texture for now
                if (channels != 2)
                {
                    logging::warn(std::format("Only supporting 2D textures: `{}`", submeshName));
                    continue;
                }

                logging::info(std::format("Loading textures coordinates from UV index {}: `{}`", texIdx, submeshName));

                // Convert 3D vectors to 2D vectors
                std::vector<glm::vec2> texCoord(submesh->mNumVertices);

                for (size_t coordIdx = 0; coordIdx < submesh->mNumVertices; ++coordIdx)
                {
                    aiVector3D& tc = submesh->mTextureCoords[texIdx][coordIdx];
                    texCoord[coordIdx] = glm::vec2(tc.x, 1.0 - tc.y); // Invert the Y-Coordinate [Only an Open GL requirement?]
                }

                textureCoords.insert(textureCoords.end(), texCoord.begin(), texCoord.begin() + submesh->mNumVertices);

                ++textureCoordCount;
            }
        }

        // Per-Vertex Material Index
        size_t start = materialIndices.size();
        size_t count = submesh->mNumVertices;
        materialIndices.resize(materialIndices.size() + count);

        std::fill(materialIndices.begin() + start, materialIndices.end(), materialIndex[submesh->mMaterialIndex]);

        // Faces
        if (submesh->HasFaces() && submesh->mNumFaces >= 1)
        {
            std::vector<uint32_t> submeshIndexData(submesh->mNumFaces * 3);
            uint32_t* ptr = submeshIndexData.data();

            size_t submeshFaces = 0;
            size_t submeshVertices = 0;

            for (uint32_t faceIdx = 0; faceIdx < submesh->mNumFaces; ++faceIdx)
            {
                const auto& face = submesh->mFaces[faceIdx];

                if (face.mNumIndices != 3)
                {
                    logging::error(fmt::format("Unsupported number of indices found in face (idx={}), of mesh: `{}`, expected 3, got {}", faceIdx, submeshName, face.mNumIndices));
                    //for (uint32_t idx = 0; idx < face.mNumIndices; idx++)
                        //logging::error(fmt::format("- {}", face.mIndices[idx]));
                    continue;
                }

                // Copy Indices. Only supporting 3 indices in a face
                *ptr++ = face.mIndices[0];
                *ptr++ = face.mIndices[1];
                *ptr++ = face.mIndices[2];
                submeshFaces += 1;
            }
            indices.insert(indices.end(), submeshIndexData.begin(), submeshIndexData.end());

            // Add submesh data to model
            modelData.indexStart += modelData.indices;
            modelData.vertexStart += modelData.vertices;

            modelData.vertices = submeshVertices;
            modelData.indices = submeshFaces * 3;

            modelData.materialIndex = submesh->mMaterialIndex;

            model->addModelData(modelData);

            // Increase the vertex offset
            vertexOffset += submesh->mNumVertices;
        }
    }

    // Position
    if (position.size())
    {
        // Scaling :: TODO: Move this to part of the yaml file
        for (auto& p : position)
        {
            p *= 0.1f;
        }

        model->addVertexData(position.data(), position.size(), AttributeLocation::Position);
    }

    // Normals
    if (normals.size())
        model->addVertexData(normals.data(), normals.size(), AttributeLocation::Normal);

    // Texture Coords
    if (textureCoords.size())
        model->addVertexData(textureCoords.data(), textureCoords.size(), AttributeLocation::TextureDiffuse);

    // Material Indexing
    model->addVertexData(materialIndices.data(), materialIndices.size(), 1, AttributeLocation::MaterialIndex);

    // Face indices
    if (indices.size())
        model->addIndexData(indices.data(), indices.size());

    return model;
}
