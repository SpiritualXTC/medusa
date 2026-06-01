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
        Config(const std::string& file); // TODO: Remove
        Config();
        virtual ~Config();

        bool loadFromFile(const std::string& file);
        bool loadFromString(const std::string& s);

        const PTree& root() const override { return _tree; }

    private:

        PTree _tree;
    };
}
