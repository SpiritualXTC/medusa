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
    container()->render();

    return true;
}
