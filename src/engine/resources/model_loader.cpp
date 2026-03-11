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
#include <engine/graphics/model.h>

#include <engine/resources/texture_manager.h>

using namespace medusa;
using namespace medusa::loaders;


//
ModelLoader::ModelLoader(std::shared_ptr<IContext> context)
    : _context(context)
{

}


//
ModelLoader::~ModelLoader()
{

}


//
std::shared_ptr<Model> ModelLoader::load(const std::string& filename, std::shared_ptr<GenericMap<Material>> materials, std::shared_ptr<TextureManager> textures)
{
    Assimp::Importer importer;

    auto context = _context.lock();

    // Vertex Data
    std::vector<glm::vec3> position;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> textureCoords;
    std::vector<uint32_t> materialIndices;

    // Face Data
    std::vector<uint32_t> indices;

    // Mesh Data
    std::vector<Indirect> submeshes;
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

        aiString matTextureDiffuse;
        if (mat->GetTexture(aiTextureType_DIFFUSE, 0, &matTextureDiffuse) != AI_SUCCESS)
            logging::warn("Unable to read TextureBlend Diffuse from material {}, idx={}, {}", matName, matIdx, matTextureDiffuse.C_Str());

        logging::info(std::format("Material `{}` has {} textures, tex0=`{}`", matName, mat->GetTextureCount(aiTextureType_DIFFUSE), matTextureDiffuse.C_Str()));

        // TODO: Get Texture Filenames and Load Textures into material

        // Set Material Data
        material.ambient(ambient.r, ambient.g, ambient.b);
        material.diffuse(diffuse.r, diffuse.g, diffuse.b, opacity);
        material.specular(specular.r, specular.g, specular.b);

        // Set & Load Textures from Resource Database into material


        // Load Texture
        if (textures)
        {
            // At this point we NEED to have the index in the Texture Handle Buffer for AZDO
            size_t textureIndex = 0;
            std::shared_ptr<ITexture> tex = textures->loadTexture(matTextureDiffuse.C_Str());
            material.diffuseTexture(tex->handle());
        }

        // Add material
        if (materials)
        {
            auto idx = materials->insert(matName, material);
            materialIndex.push_back(idx);

            logging::debug(fmt::format("Material: name={}, index={}->{}", matName, matIdx, idx));
        }
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
                    texCoord[coordIdx] = glm::vec2(tc.x, 1.0 - tc.y); // why the 1.0 - y? :( lol
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

            std::vector<uint32_t> submeshIndices(submesh->mNumFaces * 3);
            uint32_t* ptr = submeshIndices.data();

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
            }
            indices.insert(indices.end(), submeshIndices.begin(), submeshIndices.end());

            // Add submesh data to model
            Indirect* smdLast = submeshes.size() == 0 ? nullptr : &submeshes[submeshes.size() - 1];

            Indirect smd;
            smd.baseInstance = 0;
            smd.baseVertex = vertexOffset;
            smd.firstIndex = smdLast == nullptr ? 0 : smdLast->firstIndex + smdLast->count;
            smd.instanceCount = 0;
            smd.count = submeshIndices.size();

            logging::info(fmt::format("Submesh: v={}, f={}, matIdx={} -> {}", submesh->mNumVertices, submesh->mNumFaces, submesh->mMaterialIndex, materialIndex[submesh->mMaterialIndex]));

            submeshes.push_back(smd);

            // Increase the vertex offset
            vertexOffset += submesh->mNumVertices;
        }
    }

    std::shared_ptr<GenericArray<Indirect>> sm = context->createArray<Indirect>(BufferType::DrawIndirect, BufferUsage::StaticDraw);
    std::shared_ptr<VertexBuffer> vb = context->createVertexBuffer(BufferUsage::StaticDraw);
    std::shared_ptr<IndexBuffer> ib = context->createIndexBuffer(BufferUsage::StaticDraw);
    std::shared_ptr<IDescriptor> desc = context->createDescriptor();

    std::vector<Vertex> vertices(position.size());

    Geometry geometry(context);


    // Bind Buffers to Descriptors
    desc->bind();
    vb->bind();

    if (indices.size())
        ib->bind();

    // Add geometry to model and to descriptor
    if (position.size())
    {
        geometry.addVertexData(position.data(), position.size(), AttributeLocation::Position);
        desc->addDescription(types::FloatV3, vb->stride(), AttributeLocation::Position);
    }
    if (normals.size())
    {
        geometry.addVertexData(normals.data(), normals.size(), AttributeLocation::Normal);
        desc->addDescription(types::FloatV3, vb->stride(), AttributeLocation::Normal);
    }

    if (textureCoords.size())
    {
        geometry.addVertexData(textureCoords.data(), textureCoords.size(), AttributeLocation::TextureDiffuse);
        desc->addDescription(types::FloatV2, vb->stride(), AttributeLocation::TextureDiffuse);
    }

    desc->addDescription(types::Int, vb->stride(), AttributeLocation::MaterialIndex);
    geometry.addVertexData(materialIndices.data(), materialIndices.size(), 1, AttributeLocation::MaterialIndex);

    desc->unbind();

    // VertexBuffer requires staying bound while the descriptors are setup. Unbind after everything is setup
    if (indices.size())
        ib->unbind();
    vb->unbind();

    logging::info(fmt::format("Loaded Mesh - Copying to Buffers: v={}, i={}, s={}", vertices.size(), indices.size(), submeshes.size()));

    // Allocate Vertex/Index Buffers
    geometry.interleave((uint8_t*)vertices.data(), vb->stride());
    vb->allocate(vertices.data(), vertices.size());
    if (indices.size())
        ib->allocate(indices.data(), indices.size());

    sm->allocate(submeshes.data(), submeshes.size());

    // Construct model
    std::shared_ptr<Model> model = std::make_shared<Model>(desc, vb, ib, sm);
    return model;
}
