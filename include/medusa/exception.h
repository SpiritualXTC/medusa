#pragma once

#include <string>
#include <stdexcept>

namespace medusa
{
    class MedusaError : public std::runtime_error
    {
    public:
        explicit MedusaError(const std::string& msg) throw()
            : std::runtime_error(msg.c_str()),
            _message(msg)
        {
            //
        }

        virtual char const* what() const throw()
        {
            return _message.c_str();
        }

    private:
        std::string _message;
    };


}  // namespace medusa
