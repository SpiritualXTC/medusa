#include "config.h"

#include <iostream>

#include <boost/algorithm/string.hpp>
#include <boost/predef.h>

#include <medusa/exception.h>

#include "core/utilities/logging.h"


using namespace medusa;


// TODO: Clean this up [AI: Gemini]
void yaml_to_ptree(const YAML::Node& yaml_node, boost::property_tree::ptree& pt_node) {
    switch (yaml_node.Type()) {
    case YAML::NodeType::Scalar: {
        // Assign the scalar value directly to the current node
        pt_node.put_value(yaml_node.as<std::string>());
        break;
    }
    case YAML::NodeType::Map: {
        // Maps are represented as named child elements
        for (auto it = yaml_node.begin(); it != yaml_node.end(); ++it) {
            std::string key = it->first.as<std::string>();
            boost::property_tree::ptree child;
            yaml_to_ptree(it->second, child);
            pt_node.add_child(key, child);
        }
        break;
    }
    case YAML::NodeType::Sequence: {
        // Sequences/Arrays are represented as a list of unnamed children in PropertyTree
        for (const auto& item : yaml_node) {
            boost::property_tree::ptree child;
            yaml_to_ptree(item, child);
            pt_node.add_child("", child); // Use an empty string for array elements
        }
        break;
    }
    case YAML::NodeType::Null:
    case YAML::NodeType::Undefined:
    default:
        // Handled as empty values
        pt_node.put_value("");
        break;
    }
}



//
Config::Config()
{

}


//
bool Config::loadFromFile(const std::string& path)
{
    _config = YAML::LoadFile(path);


    toTree();

    return true;
}


//
bool Config::loadFromString(const std::string& contents)
{
    if (contents == "")
        throw MedusaError("No contents to load");
    _config = YAML::Load(contents);

    toTree();

    return true;
}


//
Config::Config(const std::string& file)
{
    if (!loadFromFile(file))
        throw MedusaError("Unable to load config file");
}


//
Config::~Config()
{

}


//
void Config::toTree()
{
    yaml_to_ptree(_config, _tree);
}


//
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


//
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

        n = &n2;

        if (!n->IsDefined())
            return false;
    }

    node = *n;

    return true;
}


//
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
