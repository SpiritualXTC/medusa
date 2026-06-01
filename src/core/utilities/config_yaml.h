#pragma once

#include <medusa/engine/config.h>


namespace medusa
{
    /*
        YAML Wrapper
    */

    class ConfigYAML : public IConfig
    {
    public:
        static std::shared_ptr<IConfig> loadFromFile(const std::string& file);
        static std::shared_ptr<IConfig> loadFromString(const std::string& s);

    public:
        ConfigYAML(const IConfig::PTree& config);
        virtual ~ConfigYAML();

        const PTree& root() const override { return _config; }

    private:

        PTree _config;
    };
}
