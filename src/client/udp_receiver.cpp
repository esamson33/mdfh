#include <string>
#include "udp_receiver.hpp"
#include "common/market_data/order.hpp"

namespace mdfh
{
    udp_multicast_receiver::udp_multicast_receiver(boost::asio::io_context &io_context,
            const boost::asio::ip::address &listen_address,
            const boost::asio::ip::address &multicast_address)
            : socket_(io_context),
            status_(system_message::ev_code::init)
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
                boost::asio::buffer(in_.data(), message::max_message_len), sender_endpoint_,
                boost::bind(&udp_multicast_receiver::handle_receive_from, this,
                            boost::asio::placeholders::error,
                            boost::asio::placeholders::bytes_transferred));
    }

    void udp_multicast_receiver::handle_receive_from(const boost::system::error_code &error,
                                                     size_t /*bytes_recvd*/)
    {
        if (!error) {
            in_.decode_header();
            std::cout << "seq_num " << in_.seq_no() << ", msg_count " << in_.msg_count() << "\n";

            int current_index = message::header_len;
            for (int n = 0; n < in_.msg_count(); n++)
            {
                uint16_t len;
                std::copy(&(in_.data()[current_index]),
                        &(in_.data()[current_index+2]),
                        reinterpret_cast<char*>(&len));
                len = ntohs(len);

                // +2
                char ts[9] = "";
                for (int n=0; n < 8; n++)
                {
                    ts[n] = in_.data()[current_index+2+n];
                }
                std::cout << "ts " << ts << "\n";

                //+8
                char m_type = in_.data()[current_index+10];
                std::cout << "m_type " << m_type << "\n";

                //+1, +1
                if (m_type == 'S')
                {
                    status_ = system_message::ev_code::open_messages;
                    char ev_type = in_.data()[current_index+11];
                    std::cout << "ev_type " << ev_type << "\n";
                    switch (ev_type)
                    {
                        case 'O':
                            status_ = system_message::ev_code::open_messages;
                            break;
                        case 'S':
                            status_ = system_message::ev_code::start_trading;
                            break;
                        case 'E':
                            status_ = system_message::ev_code::end_trading;
                            break;
                        case 'C':
                            status_ = system_message::ev_code::close_messages;
                            break;
                    }
                }
                else if (m_type == 'A')
                {
                    std::string in(&in_.data()[current_index+2], len);
                    mdfh::common::market_data::order o =
                            mdfh::common::market_data::from_string(in);
                    std::cout << "order\n"
                                << "\tside: " << ((o.ind == 'B') ? "BUY" : "SELL") << "\n"
                                << "\tstock: " << o.stock << "\n"
                                << "\tshares: " << o.shares << "\n"
                                << "\tprice: " << o.price << "\n";
                }

                current_index += len+1;
            }


            socket_.async_receive_from(
                    boost::asio::buffer(in_.data(), message::max_message_len), sender_endpoint_,
                    boost::bind(&udp_multicast_receiver::handle_receive_from, this,
                                boost::asio::placeholders::error,
                                boost::asio::placeholders::bytes_transferred));
        }
    }
}
