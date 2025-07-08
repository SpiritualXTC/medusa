#include "pipeline.h"

#include <medusa/renderer/pass.h>

#include <medusa/graphics/shader.h>

using namespace medusa;

Pipeline::Pipeline(std::shared_ptr<IPass> pass)
    : _pass(pass)
{
    if (!_pass)
        throw MedusaError("Invalid Pass");

}

Pipeline::~Pipeline()
{

}


bool Pipeline::render()
{
    if (_pass == nullptr)
        return false;

    auto& shader = _pass->shader();

    shader->bind();

    // Render this pass
    _pass->render();

    shader->unbind();

    return true;
}
