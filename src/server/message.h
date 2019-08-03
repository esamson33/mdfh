//
// Created by esamson on 8/2/19.
//

#ifndef MDFH_MESSAGE_H
#define MDFH_MESSAGE_H


#include <cstddef>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cstdint>

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
        seq_no_ = ((data_[0] >> 24) & 0xFF |
                (data_[1] >> 16) & 0xFF |
                (data_[2] >> 8) & 0xFF |
                (data_[3] >> 0) & 0xFF);
        msg_count_ = ((data_[4] >> 8) & 0xFF |
                      (data_[5] >> 0) & 0xFF);
    }

    void encode_header()
    {
        data_[0] = (seq_no_ >> 24) & 0xFF;
        data_[1] = (seq_no_ >> 16) & 0xFF;
        data_[2] = (seq_no_ >> 8) & 0xFF;
        data_[3] = (seq_no_ >> 0) & 0xFF;
        data_[4] = (msg_count_ >> 8) & 0xFF;
        data_[5] = (msg_count_ >> 0) & 0xFF;
    }

private:
    char data_[header_len + max_body_len];
    uint32_t seq_no_;
    uint16_t msg_count_;
};


#endif //MDFH_MESSAGE_H
