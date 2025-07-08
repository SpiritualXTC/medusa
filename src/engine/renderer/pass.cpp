#include "pass.h"

#include <core/utilities/logging.h>

using namespace medusa;


//
Pass::Pass(std::shared_ptr<IShader> shader, std::shared_ptr<IRenderableContainer> container)
    : IPass(shader, container)
{

}


//
Pass::~Pass()
{

}


//
bool Pass::render()
{
    auto ptr = static_pointer_cast<Pass>(this->shared_from_this());

    auto s = shader();

    container()->render(ptr);

    return true;
}
