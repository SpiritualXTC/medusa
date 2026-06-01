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
std::shared_ptr<IConfig> Config::loadFromFile(const std::string& file)
{
    YAML::Node yaml = YAML::LoadFile(file);

    IConfig::PTree tree;
    yaml_to_ptree(yaml, tree);

    return std::make_shared<Config>(std::move(tree));
}


//
std::shared_ptr<IConfig> Config::loadFromString(const std::string& s)
{
    YAML::Node yaml = YAML::Load(s);

    IConfig::PTree tree;
    yaml_to_ptree(yaml, tree);

    return std::make_shared<Config>(std::move(tree));
}


//
Config::Config(const IConfig::PTree& config)
    : _config(config)
{

}


//
Config::~Config()
{

}
