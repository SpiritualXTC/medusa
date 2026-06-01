#include "config.h"

#include <medusa/exception.h>

#include <yaml-cpp/yaml.h>


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
    YAML::Node yaml = YAML::LoadFile(path);

    yaml_to_ptree(yaml, _tree);

    return true;
}


//
bool Config::loadFromString(const std::string& contents)
{
    if (contents == "")
        throw MedusaError("No contents to load");
    YAML::Node yaml = YAML::Load(contents);

    yaml_to_ptree(yaml, _tree);

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
