//
// Created by enrique on 31/07/2019.
//

#include <iostream>
#include <cstdio>
#include <boost/asio.hpp>
#include "udp_multicast.hpp"
#include "system_message.hpp"

using namespace boost::asio;

namespace mdfh {

    udp_multicast_sender::udp_multicast_sender(io_context &io_context, const ip::address &multicast_addess)
            : endpoint_(multicast_addess, multicast_port), socket_(io_context, endpoint_.protocol()),
              timer_(io_context), message_count_(0), status_(system_message::ev_code::init) {
        set_midnight_timepoint();
        send_start_message();

        system_clock::time_point tp_now = system_clock::now();
        time_point<system_clock, seconds> secs =
                time_point_cast<seconds>(tp_now);
        time_point<system_clock, milliseconds> millis =
                time_point_cast<milliseconds>(tp_now);

        auto rep_secs = secs.time_since_epoch().count();
        auto rep_millis = millis.time_since_epoch().count();
        auto diff = rep_millis - (rep_secs * 1000);

        timer_.expires_after(milliseconds(diff));
        timer_.async_wait(
                boost::bind(&udp_multicast_sender::send_second_message, this,
                            placeholders::error));
    }

    void udp_multicast_sender::send_second_message(const boost::system::error_code &error)
    {
        if (!error && (max_message_count < 0 || message_count_ < max_message_count)) {
            tp_last_ = system_clock::now();
            auto diff = duration_cast<seconds>(tp_last_ - tp_mn_);

            /*
            uint16_t count{1};
            char msg_type = 'T';
            */

            uint32_t secs = static_cast<uint32_t>(diff.count());

            /*
            char b[1024];
            size_t len;
            len = std::snprintf(b, 1024, "%u%u%u%c", message_count_+1, count, secs, msg_type);
            message_ = os.str().data();
            message_.assign(b, len);
            socket_.async_send_to(
                    buffer(message_), endpoint_,
                    boost::bind(&udp_multicast_sender::handle_send_to, this,
                                placeholders::error));
            */

            second_message m(secs);
            message_.assign(m.data(), m.size());
            socket_.async_send_to(
                    buffer(message_), endpoint_,
                    boost::bind(&udp_multicast_sender::handle_send_to, this,
                                placeholders::error));
        }
    }

    void udp_multicast_sender::handle_send_to(const boost::system::error_code& error)
    {
        if (!error && (max_message_count < 0 || message_count_ < max_message_count))
        {
            message_count_++;
            if (status_ == system_message::ev_code::init)
            {
                send_start_message();
            }
            
            timer_.expires_after(std::chrono::seconds(1));
            timer_.async_wait(
                    boost::bind(&udp_multicast_sender::send_second_message, this,
                                placeholders::error));
        }
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
        auto diff = duration_cast<nanoseconds>(tp_now - tp_last_);
        system_message m(diff.count(), system_message::ev_code::open_messages);

        uint16_t count{1};
        std::ostringstream os;
        os << message_count_+1
            << count
            << m.message();
        message_ = os.str();
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
