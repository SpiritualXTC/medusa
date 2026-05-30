#pragma once

#include <medusa/engine/config.h>

#include <yaml-cpp/yaml.h> // TODO: Remove this // Move to the cpp file


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


        // TODO: Deprecate/Remove all of the following
        bool getNode(const std::string& key, YAML::Node& node);
        bool getValues(const std::string& key, std::vector<std::string>& values);

        template<typename T>
        T getValue(const std::string& key, T default_value)
        {
            YAML::Node node;
            if (!getNode(key, node))
                return default_value;
            if (node.IsSequence())
                return default_value;
            return node.as<T>();
        }

    private:

        void toTree();


        std::string makePlatformKey(std::string& key);

        YAML::Node _config;
        PTree _tree;
    };
}
