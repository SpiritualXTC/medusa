#pragma once

#include <medusa/medusa.h>
#include <medusa/graphics/mesh.h>


namespace medusa
{
    class IContext;

    class IVertexBuffer;
    class IIndexBuffer;
    class IDescriptor;

    /*

    */
    class Mesh : public IMesh
    {
    public:
        Mesh(std::shared_ptr<IContext> context);
        virtual ~Mesh();

        //
        bool render(size_t instances = 1) override;

        //
        const inline std::shared_ptr<IVertexBuffer> vertexBuffer() override { return _vertexBuffer; }
        const inline std::shared_ptr<IIndexBuffer> indexBuffer() override { return _indexBuffer; }
        const inline std::shared_ptr<IDescriptor> descriptor() override { return _descriptor; }


    private:
        std::weak_ptr<IContext> _context;

        std::shared_ptr<IVertexBuffer> _vertexBuffer = nullptr;
        std::shared_ptr<IIndexBuffer> _indexBuffer = nullptr;
        std::shared_ptr<IDescriptor> _descriptor = nullptr;
    };


}
