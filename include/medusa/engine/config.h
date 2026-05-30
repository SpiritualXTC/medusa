#pragma once

#include <medusa/medusa.h>
#include <boost/property_tree/ptree.hpp>

namespace medusa
{
    class IConfig
    {
    public:
        typedef boost::property_tree::ptree PTree;

    public:
        IConfig() {}
        virtual ~IConfig() {}


        virtual const PTree& root() const = 0;


        const PTree& node(const std::string& key)
        {
            return node(key, root());
        }
        const PTree& node(const std::string& key, const PTree& parentNode)
        {
            return parentNode.get_child(key);
        }

        template<typename T>
        const T value(const std::string& key, T defaultValue)
        {
            return value<T>(key, root(), defaultValue);
        }

        template<typename T>
        const T value(const std::string& key, const PTree& parentNode, T defaultValue)
        {
            boost::optional<const PTree&> n = parentNode.get_child_optional(key);

            if (!n)
                return defaultValue;

            return n->get_value<T>();
        }

    };
}
