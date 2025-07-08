#include "view.h"

#include <core/utilities/logging.h>

using namespace medusa;


//
MultiView::MultiView()
{

}


//
MultiView::~MultiView()
{

}


//
bool MultiView::addView(std::shared_ptr<IView> view)
{
    _views.push_back(view);
    return true;
}


//
bool MultiView::resize(uint32_t width, uint32_t height)
{
    bool b = true;
    for (auto& v : _views)
        b &= v->render();

    return b;
}



//
bool MultiView::render()
{
    bool b = true;

    for (auto& v : _views)
        b &= v->render();

    return b;
}
