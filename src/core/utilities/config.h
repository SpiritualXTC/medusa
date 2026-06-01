#pragma once

#include <medusa/engine/config.h>


namespace medusa
{
    /*
        YAML Wrapper
    */

    class Config : public IConfig
    {
    public:
        static std::shared_ptr<IConfig> loadFromFile(const std::string& file);
        static std::shared_ptr<IConfig> loadFromString(const std::string& s);

    public:
        Config(const IConfig::PTree& config);
        virtual ~Config();

        const PTree& root() const override { return _config; }

    private:

        PTree _config;
    };
}
