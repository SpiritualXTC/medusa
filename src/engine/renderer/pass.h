#pragma once

#include <medusa/renderer/pass.h>
#include <medusa/renderer/renderable.h>


namespace medusa
{
    class Pass : public IPass
    {
    public:
        Pass(std::shared_ptr<IShader> shader, std::shared_ptr<IRenderableContainer> container);
        virtual ~Pass();

        bool render() override;



    private:

    };


    class MultiPass : public IPass
    {
    public:

    };
}
