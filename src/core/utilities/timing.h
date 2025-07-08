#pragma once

#include <chrono>

#include <boost/signals2/signal.hpp>

namespace medusa
{
    class Timing
    {
    public:
        typedef std::chrono::high_resolution_clock CHRONO;
        typedef boost::signals2::signal<void(int)> OnSecond;

    public:
        //
        Timing();

        //
        virtual ~Timing();

        //
        void update();

        //
        const inline int32_t fps() const { return _fps; }

        //
        const inline double frametime() const { return _frametime; }

        //
        OnSecond onSecond;

    private:
        int32_t _fps = 0;
        double _frametime = 0.0;


        int32_t _frame = 0;
        double _timing = 0.0;

        CHRONO::time_point _last;

    };
}
