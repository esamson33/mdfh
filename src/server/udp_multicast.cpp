//
// Created by enrique on 31/07/2019.
//

#include <iostream>
#include <cstdio>
#include <boost/asio.hpp>
#include "udp_multicast.hpp"
#include "system_message.hpp"
#include "message.h"

using namespace boost::asio;

namespace mdfh {

    udp_multicast_sender::udp_multicast_sender(io_context &io_context, const ip::address &multicast_addess)
    : endpoint_(multicast_addess, multicast_port), socket_(io_context, endpoint_.protocol()),
      timer_(io_context), message_count_(0), status_(system_message::ev_code::init)
    {
        set_midnight_timepoint();
        send_start_message();
    }

    void udp_multicast_sender::set_midnight_timepoint()
    {
        time_t now;
        time(&now);
        std::tm * mn = std::localtime(&now);
        mn->tm_sec = 0;
        mn->tm_min = 0;
        mn->tm_hour = 0;
        std::time_t secs_from_mn = std::mktime(mn);
        tp_mn_ = system_clock::from_time_t(secs_from_mn);
    }

    void udp_multicast_sender::send_start_message()
    {
        system_clock::time_point tp_now = system_clock::now();
        auto diff = duration_cast<milliseconds>(tp_now - tp_mn_);
        system_message sm(diff.count(), system_message::ev_code::open_messages);

        message msg_packet;
        msg_packet.seq_no(message_count_ + 1);
        msg_packet.msg_count(1);
        msg_packet.encode_header();

        system_message::buffer_type p = sm.message();
        memcpy(msg_packet.body(), p.data(), p.size());
        message_.assign(msg_packet.data(), msg_packet.header_len + sm.message().size());
        socket_.async_send_to(
                buffer(message_), endpoint_,
                boost::bind(&udp_multicast_sender::handle_send_start, this,
                            placeholders::error));
    }

    void udp_multicast_sender::handle_send_start(const boost::system::error_code& error)
    {
        if (!error)
        {
            message_count_++;
            status_ = system_message::ev_code::open_messages;
            std::cout << "sent start_messages event\n";
        }
        else
        {
            std::cerr << "failed to send start_messages event\n";
        }
    }

} // ns mdfh
