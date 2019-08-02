#ifndef MDFH_UDP_RECEIVER_HPP
#define MDFH_UDP_RECEIVER_HPP

#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>


namespace mdfh
{
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
        enum { max_length = 1024, multicast_port = 54321 };
        char data_[max_length];
    };

} // ns mdfh

#endif //MDFH_UDP_RECEIVER_HPP
