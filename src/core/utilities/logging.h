#pragma once

#include <string>

#include "spdlog/spdlog.h"


namespace spdlog
{
    void initialise();

    std::shared_ptr<spdlog::logger> clone_default(const std::string& name);
}

namespace logging = spdlog;
