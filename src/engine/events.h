#pragma once

#include <memory>
#include <vector>

#include <medusa/engine/events.h>

namespace medusa
{

    class Events
    {
    public:
        Events();
        virtual ~Events();

        bool registerHandler(std::shared_ptr<IEventHandler> handler);
        bool unregisterHandler(std::shared_ptr<IEventHandler> handler);

        bool poll();

    private:
        std::vector<std::weak_ptr<IEventHandler>> _handlers;
    };
}
