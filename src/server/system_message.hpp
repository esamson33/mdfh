#ifndef SYSTEM_MESSAGE_HPP
#define SYSTEM_MESSAGE_HPP

#include <string>
namespace mdfh
{

    class system_message
    {
    public:
        enum ev_code {
            init = 'I',
            open_messages = 'O',
            start_trading = 'S', 
            end_trading = 'E', 
            close_messages = 'C'};

        system_message(uint32_t ts, ev_code ev)
          : ts_(ts), ev_(ev) {}

        std::string message();

    private:
        uint32_t ts_;
        ev_code ev_;
    };

}
#endif // SYSTEM_MESSAGE_HPP
