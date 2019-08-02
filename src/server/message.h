//
// Created by esamson on 8/2/19.
//

#ifndef MDFH_MESSAGE_H
#define MDFH_MESSAGE_H


#include <cstddef>
#include <cstdio>
#include <cstring>
#include <cstdlib>

class message {
public:
    enum { seq_no_len = 4 };
    enum { msg_count_len = 2 };
    enum { header_len = seq_no_len + msg_count_len };
    enum { max_body_len = 1024 };


    message()
    : seq_no_(0), msg_count_(0)
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
        using namespace std; // For strncat and atoi.
        char seq_no[seq_no_len+1] = "";
        strncat(seq_no, data_, seq_no_len);
        char msg_count[msg_count_len+1] = "";
        strncat(msg_count, data_+seq_no_len, msg_count_len);
        seq_no_ = atoi(seq_no);
        msg_count_ = atoi(msg_count);
    }

    void encode_header()
    {
        using namespace std; // For sprintf and memcpy.
        char header[header_len + 1] = "";
        sprintf(header, "%4d%2d", static_cast<int>(seq_no_),
                static_cast<int>(msg_count_));
        memcpy(data_, header, header_len);
    }

private:
    char data_[header_len + max_body_len];
    size_t seq_no_;
    size_t msg_count_;
};


#endif //MDFH_MESSAGE_H
