//
// Created by enrique on 31/07/2019.
//

#include <iostream>
#include <boost/asio.hpp>
#include "udp_multicast.hpp"
#include "common/system_message.hpp"
#include "common/message.hpp"
#include "common/market_data/order.hpp"

using namespace boost::asio;

namespace mdfh {

    udp_multicast_sender::udp_multicast_sender(io_context &io_context, const ip::address &multicast_address)
    : endpoint_(multicast_address, multicast_port), socket_(io_context, endpoint_.protocol()),
      timer_(io_context), message_count_(0), status_(system_message::ev_code::init), work_(make_work_guard(io_context))
    {
        set_midnight_timepoint();
        send_open_message();
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

    void udp_multicast_sender::send_open_message()
    {
        system_clock::time_point tp_now = system_clock::now();
        auto diff = duration_cast<milliseconds>(tp_now - tp_mn_);
        system_message sm(diff.count(), system_message::ev_code::open_messages);

        message msg_packet;
        msg_packet.seq_no(message_count_ + 1);
        msg_packet.msg_count(1);
        msg_packet.encode_header();

        system_message::buffer_type p = sm.message();
        std::copy(p.begin(), p.end(), msg_packet.body());
        message_.assign(msg_packet.data(), msg_packet.header_len + sm.message().size());
        socket_.async_send_to(
                buffer(message_), endpoint_,
                boost::bind(&udp_multicast_sender::handle_send_open, this,
                            placeholders::error));
    }

    void udp_multicast_sender::handle_send_open(const boost::system::error_code& error)
    {
        if (!error)
        {
            message_count_++;
            status_ = system_message::ev_code::open_messages;
            std::cout << "sent open_messages event\n";

            timer_.expires_after(seconds(1));
            timer_.async_wait(boost::bind(&udp_multicast_sender::send_start_trade_message, this));
        }
        else
        {
            std::cerr << "failed to send start_messages event\n";
        }
    }

    void udp_multicast_sender::send_start_trade_message()
    {
        system_clock::time_point tp_now = system_clock::now();
        auto diff = duration_cast<milliseconds>(tp_now - tp_mn_);
        system_message sm(diff.count(), system_message::ev_code::start_trading);

        message msg_packet;
        msg_packet.seq_no(message_count_ + 1);
        msg_packet.msg_count(1);
        msg_packet.encode_header();

        system_message::buffer_type p = sm.message();
        std::copy(p.begin(), p.end(), msg_packet.body());
        message_.assign(msg_packet.data(), msg_packet.header_len + sm.message().size());
        socket_.async_send_to(
                buffer(message_), endpoint_,
                boost::bind(&udp_multicast_sender::handle_send_start_trade, this,
                            placeholders::error));
    }

    void udp_multicast_sender::handle_send_start_trade(const boost::system::error_code& error)
    {
        if (!error)
        {
            message_count_++;
            status_ = system_message::ev_code::start_trading;
            std::cout << "sent start_trading event\n";

            start_order_sending();

            timer_.expires_after(seconds(10));
            timer_.async_wait(boost::bind(&udp_multicast_sender::send_end_trade_message, this));
        }
    }

    void udp_multicast_sender::send_end_trade_message()
    {
        system_clock::time_point tp_now = system_clock::now();
        auto diff = duration_cast<milliseconds>(tp_now - tp_mn_);
        system_message sm(diff.count(), system_message::ev_code::end_trading);

        message msg_packet;
        msg_packet.seq_no(message_count_ + 1);
        msg_packet.msg_count(1);
        msg_packet.encode_header();

        system_message::buffer_type p = sm.message();
        std::copy(p.begin(), p.end(), msg_packet.body());
        message_.assign(msg_packet.data(), msg_packet.header_len + sm.message().size());
        socket_.async_send_to(
                buffer(message_), endpoint_,
                boost::bind(&udp_multicast_sender::handle_send_end_trade, this,
                            placeholders::error));
    }

    void udp_multicast_sender::handle_send_end_trade(const boost::system::error_code& error)
    {
        if (!error)
        {
            message_count_++;
            status_ = system_message::ev_code::end_trading;
            std::cout << "sent end_trading event\n";

            timer_.expires_after(seconds(1));
            timer_.async_wait(boost::bind(&udp_multicast_sender::send_close_message, this));
        }
    }

    void udp_multicast_sender::send_close_message()
    {
        system_clock::time_point tp_now = system_clock::now();
        auto diff = duration_cast<milliseconds>(tp_now - tp_mn_);
        system_message sm(diff.count(), system_message::ev_code::close_messages);

        message msg_packet;
        msg_packet.seq_no(message_count_ + 1);
        msg_packet.msg_count(1);
        msg_packet.encode_header();

        system_message::buffer_type p = sm.message();
        std::copy(p.begin(), p.end(), msg_packet.body());
        message_.assign(msg_packet.data(), msg_packet.header_len + sm.message().size());
        socket_.async_send_to(
                buffer(message_), endpoint_,
                boost::bind(&udp_multicast_sender::handle_send_close, this,
                            placeholders::error));
    }

    void udp_multicast_sender::handle_send_close(const boost::system::error_code& error)
    {
        if (!error)
        {
            message_count_++;
            status_ = system_message::ev_code::close_messages;
            std::cout << "sent close_messages event\n";
            work_.reset();
        }
    }

    void udp_multicast_sender::start_order_sending()
    {
        std::string csv_from_input_file[] = {
                "00000000,A,123456789,B,002000,   IBM,0002000000,Y",
                "00000000,A,123456790,S,006000,   IBM,0002010000,Y",
        };

        for (const auto& csv: csv_from_input_file)
        {
            auto order = mdfh::common::market_data::from_csv(csv);

            system_clock::time_point tp_now = system_clock::now();
            auto diff = duration_cast<milliseconds>(tp_now - tp_mn_);
            order->ts = std::to_string(diff.count());
            message msg_packet;
            msg_packet.seq_no(message_count_ + 1);
            msg_packet.msg_count(1);
            msg_packet.encode_header();

            std::string o = order->str();
            std::cout << "order " << o.c_str() << "\n";
            auto len = static_cast<uint16_t>(o.size());
            constexpr int buf_len = mdfh::common::market_data::order::total_data_len + 2;
            char buf[buf_len];
            len = htons(len);

            std::copy(reinterpret_cast<const char*>(&len),
                      reinterpret_cast<const char*>(&len) + sizeof(uint16_t),
                      &buf[0]);
            std::copy(o.begin(), o.end(), &buf[0]+2);
            std::copy(&buf[0], &buf[0]+ (2+o.size()), msg_packet.body());
            message_.assign(msg_packet.data(), msg_packet.header_len + (2+o.size()));
            socket_.async_send_to(
                    buffer(message_), endpoint_,
                    boost::bind(&udp_multicast_sender::handle_send_order, this,
                                placeholders::error, 1));
        }
    }

    void udp_multicast_sender::handle_send_order(const boost::system::error_code& error, int count)
    {
        if (!error)
        {
            message_count_ += count;
            std::cout << "Sent " << count << " data messages" << "\n";
        }
    }

} // ns mdfh
