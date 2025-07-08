#pragma once

#include <tuple>
#include <yaml-cpp/yaml.h>

namespace medusa
{
    /*
        YAML Wrapper
    */
    class Config
    {
    public:
        Config(const std::string& file);
        virtual ~Config();


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

        std::string makePlatformKey(std::string& key);

        YAML::Node _config;
    };
}
