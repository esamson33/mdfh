//
// Created by Enrique Jr. Samson on 2019-08-04.
//

#ifndef MDFH_ORDER_HPP
#define MDFH_ORDER_HPP

#include <memory>
#include <sstream>
#include <iomanip>

namespace mdfh {
namespace common {
namespace market_data {

    struct order
    {
        static constexpr int total_data_len = 42;

        std::string ts;         // len is 8
        char m_type;            // "A", "a" long form
        std::string ref;        // len 9
        char ind;               // "B", "S"
        std::string shares;     // len 6
        std::string stock;      // len 6
        std::string price;      // len 10
        char display;           // "Y"

        std::string str()
        {
            std::ostringstream os;
            os << std::right << std::setw(8) << ts
                << m_type
                << ref
                << ind
                << shares
                << std::left << std::setw(6) << stock
                <<  price
                << display;
            return os.str();
        }
    };

    static order from_csv(const std::string& csv)
    {
        order o;
        o.ts = csv.substr(0, 8);
        o.m_type = csv.substr(9, 1)[0];
        switch (o.m_type) {
            case 'A':
                o.ref = csv.substr(11, 9);
                o.ind = csv.substr(21, 1)[0];
                o.shares = csv.substr(23, 6);
                o.stock = csv.substr(30, 6);
                o.price = csv.substr(37, 10);
                o.display = csv.substr(48, 1)[0];
        }
        return o;
    }

    static order from_string(const std::string& str)
    {
        order o;
        o.ts = str.substr(0, 8);
        o.m_type = str.substr(8, 1)[0];
        switch (o.m_type) {
            case 'A':
                o.ref = str.substr(9, 9);
                o.ind = str.substr(18, 1)[0];
                o.shares = str.substr(19, 6);
                o.stock = str.substr(25, 6);
                o.price = str.substr(31, 10);
                o.display = str.substr(41, 1)[0];
        }
        return o;
    }

} // market_data
} // common
} // mdfh


#endif //MDFH_ORDER_HPP
