#include "timing.h"

using namespace medusa;


//
Timing::Timing()
{
    _last = CHRONO::now();
}


//
Timing::~Timing()
{

}


//
void Timing::update()
{
    CHRONO::time_point time = CHRONO::now();

    _frametime = (time - _last).count() / (double)CHRONO::period::den;

    _last = time;

    _frame++;
    _timing += _frametime;
    if (_timing > 1.0)
    {
        _fps = _frame;

        _frame = 0;
        _timing -= 1.0;

        onSecond(_fps);
    }
}
