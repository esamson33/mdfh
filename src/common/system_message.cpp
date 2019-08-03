#include <iomanip>
#include <iostream>
#include "system_message.hpp"

namespace mdfh
{
    system_message::buffer_type system_message::message()
    {
        if (message_.empty()) {
            std::ostringstream os;
            os << std::right << std::setw(8) << ts_;
            os << 'S';
            os << (char) ev_;
            message_.assign(os.str());
        }
        system_message::buffer_type buf;
        auto len = static_cast<uint16_t>(message_.size());
        buf[0] = (len >> 8) & 0xFF;
        buf[1] = (len >> 0) & 0xFF;
        std::copy(message_.begin(), message_.end(), buf.begin()+2);
        return buf;
    }
}