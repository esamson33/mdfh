#ifndef UDP_MULTICAST_HPP
#define UDP_MULTICAST_HPP

#include <ctime>
#include <chrono>
#include <sstream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include "common/system_message.hpp"

using namespace boost::asio;
using namespace std::chrono;

namespace mdfh
{
    class udp_multicast_sender
    {
    public:
        typedef executor_work_guard<io_context::executor_type> work_guard;

        const int multicast_port = 54321;
#ifdef MDFH_DEV
        static const int max_message_count = 10;
#else
        static const int max_message_count = -1;
#endif
        udp_multicast_sender(io_context&, const ip::address&);

    private:
        void set_midnight_timepoint();
        
        void send_open_message();

        void handle_send_open(const boost::system::error_code&);

        void send_start_trade_message();

        void handle_send_start_trade(const boost::system::error_code&);

        void send_end_trade_message();

        void handle_send_end_trade(const boost::system::error_code&);

        void send_close_message();

        void handle_send_close(const boost::system::error_code&);

        void start_order_sending();

        void handle_send_order(const boost::system::error_code&, int);

    private:
        ip::udp::endpoint endpoint_;
        ip::udp::socket socket_;
        steady_timer timer_;
        uint32_t message_count_;
        std::string message_;
        system_clock::time_point tp_mn_;
        system_clock::time_point tp_last_;
        system_message::ev_code status_;
        executor_work_guard<io_context::executor_type> work_;
    };
}

#endif
