#pragma once

#include <core/utilities/config.h>

namespace medusa
{
    class MedusaConfig : public Config
    {
    public:
        MedusaConfig() : Config("../config/medusa.config.yaml") {}

    };
}
