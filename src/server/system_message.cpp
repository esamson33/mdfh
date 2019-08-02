#include <sstream>
#include "system_message.hpp"

namespace mdfh
{
    std::string system_message::message()
    {
        std::ostringstream os;
        os << ts_;
        os << 'S';
        os << (char)ev_;
        return os.str();
    }
}
