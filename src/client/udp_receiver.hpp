#ifndef MDFH_UDP_RECEIVER_HPP
#define MDFH_UDP_RECEIVER_HPP

#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "common/message.hpp"
#include "common/system_message.hpp"
#include "order_library.hpp"

namespace mdfh {
namespace client {

    class udp_multicast_receiver
    {
    public:
        udp_multicast_receiver(boost::asio::io_context&,
                const boost::asio::ip::address& listen_address,
                 const boost::asio::ip::address& multicast_address);

        void handle_receive_from(const boost::system::error_code&, size_t);

    private:
        boost::asio::ip::udp::socket socket_;
        boost::asio::ip::udp::endpoint sender_endpoint_;
        enum { max_length = 1024 };
        enum { multicast_port = 54321 };
        message in_;
        system_message::ev_code status_;
        order_library library_;
    };
}
} // ns mdfh

#endif //MDFH_UDP_RECEIVER_HPP
