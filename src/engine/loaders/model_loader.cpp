#include "model_loader.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <medusa/engine/context.h>
#include <medusa/graphics.h>
#include <medusa/graphics/containers.h>

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


std::shared_ptr<IMesh> ModelLoader::load(const std::string& filename, std::shared_ptr<GenericMap<Material>> materials)
{
    Assimp::Importer importer;

    auto context = _context.lock();

    std::vector<glm::vec3> position;
    std::vector<glm::vec3> normals;
    // TODO: Texture Coordinates

    std::vector<uint32_t> indices;

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

        // TODO: Read Texture Filenames


        // Set Material Data
        material.ambient(ambient.r, ambient.g, ambient.b);
        material.diffuse(diffuse.r, diffuse.g, diffuse.b, opacity);
        material.specular(specular.r, specular.g, specular.b);

        // Set & Load Textures from Resource Database into material

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
            smd.baseInstance = submesh->mMaterialIndex >= materialIndex.size() ? 0 : materialIndex[submesh->mMaterialIndex];
            smd.baseVertex = vertexOffset;
            smd.firstIndex = smdLast == nullptr ? 0 : smdLast->firstIndex + smdLast->count;
            smd.instanceCount = 1; // TODO: Should be zero to start with, this will be increased during scene update
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
        desc->addDescription(types::FloatV3, sizeof(Vertex), AttributeLocation::Position);
    }
    if (normals.size())
    {
        geometry.addVertexData(normals.data(), normals.size(), AttributeLocation::Normal);
        desc->addDescription(types::FloatV3, sizeof(Vertex), AttributeLocation::Normal);
    }

    geometry.interleave((uint8_t*)vertices.data(), sizeof(Vertex));

    desc->unbind();

    // VertexBuffer requires staying bound while the descriptors are setup
    if (indices.size())
        ib->unbind();
    vb->unbind();


    logging::info(fmt::format("Loaded Mesh - Copying to Buffers: v={}, i={}, s={}", vertices.size(), indices.size(), submeshes.size()));

    // Allocate Vertex/Index Buffers
    vb->allocate(vertices.data(), vertices.size());
    if (indices.size())
        ib->allocate(indices.data(), indices.size());

    sm->allocate(submeshes.data(), submeshes.size());

    // Construct model
    std::shared_ptr<Model> model = std::make_shared<Model>(desc, vb, ib, sm);
    return model;
}
