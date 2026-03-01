#include "pass.h"

#include <core/utilities/logging.h>

#include <medusa/graphics/shader.h>

using namespace medusa;


//
Pass::Pass(std::shared_ptr<IShader> shader)
    : _shader(shader)
{

}


//
Pass::~Pass()
{

}
