//
// Created by esamson on 8/2/19.
//

#ifndef MDFH_MESSAGE_HPP
#define MDFH_MESSAGE_HPP


#include <cstddef>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <algorithm>

#ifdef __linux__
#include <arpa/inet.h>
#endif

class message {
public:
    enum { seq_no_len = 4 };
    enum { msg_count_len = 2 };
    enum { header_len = seq_no_len + msg_count_len };
    enum { max_body_len = 1024 };
    enum { max_message_len = header_len + max_body_len };

    message()
    : data_(""), seq_no_(0), msg_count_(0)
    {
    }

    const char* data() const
    {
        return data_;
    }

    char* data()
    {
        return data_;
    }

    const char* body() const
    {
        return data_ + header_len;
    }

    char* body()
    {
        return data_ + header_len;
    }

    size_t seq_no() const
    {
        return seq_no_;
    }

    void seq_no(size_t new_seq_no)
    {
        seq_no_ = new_seq_no;
    }

    size_t msg_count() const
    {
        return msg_count_;
    }

    void msg_count(size_t new_msg_count)
    {
        msg_count_ = new_msg_count;
    }

    void decode_header()
    {
        uint32_t seq_no;
        std::copy(&data_[0], &data_[4], reinterpret_cast<char*>(&seq_no));
        seq_no_ = ntohl(seq_no);

        uint16_t msg_count;
        std::copy(&data_[4], &data_[6], reinterpret_cast<char*>(&msg_count));
        msg_count_ = ntohs(msg_count);
    }

    void encode_header()
    {
        uint32_t seq_no = htonl(seq_no_);
        std::copy(
                reinterpret_cast<const char*>(&seq_no),
                reinterpret_cast<const char*>(&seq_no) + sizeof(uint32_t),
                &data_[0]
        );

        uint16_t msg_count = htons(msg_count_);
        std::copy(
                reinterpret_cast<const char*>(&msg_count),
                reinterpret_cast<const char*>(&msg_count) + sizeof(uint16_t),
                &data_[4]
        );
    }

private:
    char data_[header_len + max_body_len];
    uint32_t seq_no_;
    uint16_t msg_count_;
};


#endif //MDFH_MESSAGE_HPP
