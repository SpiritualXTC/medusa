#pragma once

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/random_generator.hpp>

namespace medusa
{
    class UUID
    {
    public:
        /// <summary>
        /// Generate a UUID
        /// </summary>
        UUID()
        {
            _uuid = boost::uuids::random_generator()();
        }

        /// <summary>
        /// Get the UUID as a string
        /// </summary>
        /// <returns></returns>
        std::string getUUID()
        {
            return boost::uuids::to_string(_uuid);
        }

    private:
        boost::uuids::uuid _uuid;
    };
}
