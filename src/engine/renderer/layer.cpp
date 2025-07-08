#include "layer.h"

#include <medusa/math/matrix.h>

#include <medusa/renderer/pipeline.h>

using namespace medusa;



//
Layer::Layer(std::shared_ptr<IPipeline> pipeline)
    : _pipeline(pipeline)
{

}


//
Layer::~Layer()
{

}


//
bool Layer::render()
{
    // Set Scissor Rect
    _pipeline->render();

    return true;
}
