#ifndef SYSTEM_MESSAGE_HPP
#define SYSTEM_MESSAGE_HPP

#include <string>

namespace mdfh
{

    class system_message
    {
    public:
        enum { message_len = 12 };
        typedef std::array<unsigned char, message_len> buffer_type;
        enum ev_code {
            init = 'I',
            open_messages = 'O',
            start_trading = 'S', 
            end_trading = 'E', 
            close_messages = 'C'};

        system_message(uint32_t ts, ev_code ev)
          : ts_(ts), ev_(ev) {}

        buffer_type message();

    private:
        uint64_t ts_;
        ev_code ev_;
        std::string message_;
    };

}
#endif // SYSTEM_MESSAGE_HPP
