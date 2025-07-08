#pragma once

#include <medusa/medusa.h>

namespace medusa
{
    class IScene
    {
    public:
        IScene() {}
        virtual ~IScene() {}

        inline uint64_t id() { return _id; }

        virtual bool update(float ft) = 0;
        virtual bool render() = 0;

    private:
        static uint64_t generate()
        {
            static uint64_t auto_increment = 0;

            return ++auto_increment;
        }


        const uint64_t _id = generate();
    };
}
