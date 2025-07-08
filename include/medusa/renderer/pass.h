#pragma once

#include <medusa/medusa.h>
#include <medusa/graphics_fwd.h>

#include <medusa/renderer/renderable.h>

namespace medusa
{
    class IPass : public std::enable_shared_from_this<IPass>
    {
    public:
        IPass(std::shared_ptr<IShader> shader, std::shared_ptr<IRenderableContainer> _container)
            : _shader(shader)
            , _container(_container)
        {

        }

        virtual ~IPass() {}


        const inline std::shared_ptr<IShader> shader() { return _shader; }

        virtual bool render() = 0;

        const inline std::shared_ptr<IRenderableContainer> container() const { return _container; }

    private:
        std::shared_ptr<IShader> _shader;
        std::shared_ptr<IRenderableContainer> _container;
    };
}
