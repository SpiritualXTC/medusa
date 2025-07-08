#include "config.h"

#include <iostream>

#include <boost/algorithm/string.hpp>
#include <boost/predef.h>

#include "core/utilities/logging.h"

using namespace medusa;


Config::Config(const std::string& file)
{
    // TODO: Needs to combine MULTIPLE config files into a single global config.
    //  - Application / User configs (~/./game.yaml)
    //  - Engine (Embedded Resource)

    _config = YAML::LoadFile(file);
    for (YAML::const_iterator it = _config.begin(); it != _config.end(); ++it)
    {
        /*
        std::cout << "YAML `" << it->first.as<std::string>() << "`" << std::endl;
        std::cout << "- IsMap: " << it->first.IsMap() << std::endl;
        std::cout << "- IsScaler: " << it->first.IsScalar() << std::endl;
        std::cout << "- IsSequence: " << it->first.IsSequence() << std::endl;

        std::cout << "- IsMap: " << it->second.IsMap() << std::endl;
        std::cout << "- IsScaler: " << it->second.IsScalar() << std::endl;
        std::cout << "- IsSequence: " << it->second.IsSequence() << std::endl;
        */
    }



}

Config::~Config()
{

}


std::string Config::makePlatformKey(std::string& key)
{
#ifdef BOOST_OS_WINDOWS
    std::string platform = "windows";
#endif

    std::vector<std::string> result;
    boost::split(result, key, boost::is_any_of("."));

    result.insert(result.begin() + 1, platform);

    return boost::join(result, ".");
}



bool Config::getNode(const std::string& key, YAML::Node& node)
{
    YAML::Node* n = &_config;

    std::vector<std::string> keys;
    boost::split(keys, key, boost::is_any_of("."));

    for (auto& k : keys)
    {
        if (!n->IsMap())
            return false;

        YAML::Node n2 = (*n)[k].as<YAML::Node>();

        n = &n2;// ((*n)[k]);

        if (!n->IsDefined())
            return false;
    }

    node = *n;

    return true;
}


bool Config::getValues(const std::string& key, std::vector<std::string>& values)
{
    YAML::Node node;
    if (!getNode(key, node))
    {
        logging::warn("Node `{0}` doesn't exist in YAML", key);
        return false;
    }

    if (node.IsSequence())
    {
        for (auto it = node.begin(); it != node.end(); ++it)
        {
            const std::string ext = it->as<std::string>();
            values.push_back(ext);
        }
    }
    else if (node.IsScalar())
    {
        const std::string ext = node.as<std::string>();
        values.push_back(ext);
    }

    return true;
}

/*
bool Config::getValue(const std::string& key, std::string& value)
{
    YAML::Node node;
    if (!getNode(key, node))
    {
        logging::warn("Node `{0}` doesn't exist in YAML", key);
        return false;
    }

    if (node.IsSequence())
    {
        logging::warn("Node `{0}` is not a scalar", key);
        return false;
    }

    if (node.IsScalar())
    {
        const std::string ext = node.as<std::string>();
        value = ext;
    }
}
*/
