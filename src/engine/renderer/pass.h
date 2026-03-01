#pragma once

#include <medusa/renderer/pass.h>
#include <medusa/renderer/renderable.h>


namespace medusa
{
    class Pass : public IPass
    {
    public:
        Pass(std::shared_ptr<IShader> shader);
        virtual ~Pass();

        const inline std::shared_ptr<IShader> shader() override { return _shader; }

    private:
        std::shared_ptr<IShader> _shader;
    };

}
