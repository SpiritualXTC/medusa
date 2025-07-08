#include "logging.h"


#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"


void spdlog::initialise()
{
    static bool done = false;

    if (done)
    {
        spdlog::warn("Logging already initialised");
        return;
    }

    auto console = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console->set_level(spdlog::level::debug);

    auto file = std::make_shared<spdlog::sinks::basic_file_sink_mt>("medusa.log", true);
    file->set_level(spdlog::level::debug);

    done = true;

    spdlog::logger logger("root", { console, file });

    auto l = std::make_shared<spdlog::logger>(logger);
    l->set_level(logging::level::debug);

    spdlog::set_default_logger(l);

    spdlog::info("Logging initialised");

}

std::shared_ptr<spdlog::logger> spdlog::clone_default(const std::string& name)
{
    std::shared_ptr<spdlog::logger> logger = spdlog::default_logger()->clone(name.c_str());

    return logger;
}
