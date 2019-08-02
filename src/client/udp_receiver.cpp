#include "udp_receiver.hpp"

namespace mdfh
{
    udp_multicast_receiver::udp_multicast_receiver(boost::asio::io_context &io_context,
                                                   const boost::asio::ip::address &listen_address,
                                                   const boost::asio::ip::address &multicast_address)
                                                   : socket_(io_context)
            {
        // Create the socket so that multiple may be bound to the same address.
        boost::asio::ip::udp::endpoint listen_endpoint(listen_address, multicast_port);
        socket_.open(listen_endpoint.protocol());
        socket_.set_option(boost::asio::ip::udp::socket::reuse_address(true));
        socket_.bind(listen_endpoint);

        // Join the multicast group.
        socket_.set_option(
                boost::asio::ip::multicast::join_group(multicast_address));

        socket_.async_receive_from(
                boost::asio::buffer(data_, max_length), sender_endpoint_,
                boost::bind(&udp_multicast_receiver::handle_receive_from, this,
                            boost::asio::placeholders::error,
                            boost::asio::placeholders::bytes_transferred));
    }

    void udp_multicast_receiver::handle_receive_from(const boost::system::error_code &error,
                                                     size_t bytes_recvd)
    {
        if (!error) {
            // parse data here
            //uint32_t seq_num = *(reinterpret_cast<uint32_t*>(data_));
            //uint16_t msg_count = *(reinterpret_cast<uint32_t*>(data_+4));
            char s[] = { data_[0], 0 };
            uint32_t seq_num = std::atoi(s);
            uint16_t msg_count = 0;//std::atoi(data_[1);

            std::cout << "seq_num " << seq_num << ", msg_count " << msg_count << "\n";
            std::cout.write(data_, bytes_recvd);
            std::cout << std::endl;

            socket_.async_receive_from(
                    boost::asio::buffer(data_, max_length), sender_endpoint_,
                    boost::bind(&udp_multicast_receiver::handle_receive_from, this,
                                boost::asio::placeholders::error,
                                boost::asio::placeholders::bytes_transferred));
        }
    }
}
