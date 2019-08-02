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
    enum { header_length = 6 };
    enum { max_body_length = 1024 };

    message()
    : body_length_(0), body_count_(0)
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

    const size_t length() const
    {
        return header_length + body_length_;
    }

    const char* body() const
    {
        return data_ + header_length;
    }

    char* body()
    {
        return data_ + header_length;
    }

    size_t body_length() const
    {
        return body_length_;
    }

    void body_length(size_t new_length)
    {
        body_length_ = new_length;
        if (body_length_ > max_body_length)
            body_length_ = max_body_length;
    }

    bool decode_header()
    {
        using namespace std; // For strncat and atoi.
        char seq_no[seq_no_len+1] = "";
        char seq_no[msg_count+1] = "";
        strncat(seq_no, data_, seq_no_len);
        strncat(msg_count, data_[seq_no_len], msg_count);
        body_length_ = atoi(header);
        if (body_length_ > max_body_length)
        {
            body_length_ = 0;
            return false;
        }
        return true;
    }

    void encode_header()
    {
        using namespace std; // For sprintf and memcpy.
        char header[header_length + 1] = "";
        sprintf(header, "%4d%2d", static_cast<int>(body_length_), static_cast<int>(body_count_));
        memcpy(data_, header, header_length);
    }

private:
    char data_[header_length + max_body_length];
    size_t body_length_;
    size_t body_count_;
};


#endif //MDFH_MESSAGE_H
