#include "layer.h"

using namespace medusa;

//
LayerGroup::LayerGroup()
{

}


//
LayerGroup::~LayerGroup()
{

}


//
bool LayerGroup::addLayer(std::shared_ptr<ILayer> layer)
{
    _layers.push_back(layer);
    return true;
}


bool LayerGroup::render()
{
    for (auto& layer : _layers)
    {
        layer->render();
    }

    return true;
}
