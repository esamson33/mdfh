//
// Created by Enrique Jr. Samson on 2019-08-05.
//

#ifndef MDFH_ORDER_BOOK_HPP
#define MDFH_ORDER_BOOK_HPP

#include <queue>
#include <iostream>
#include "common/market_data/order.hpp"

namespace mdfh {
namespace server {

    class order_book {
    public:
        order_book(std::string stock)
        : stock_(stock)
        {
        }

        void receive(mdfh::common::market_data::order::order_ptr order)
        {
            if (order->ind == 'B') buy_.push(*order);
            if (order->ind == 'S') sell_.push(*order);
        }

        void show()
        {
            std::cout << "\n==========\nBook: " << stock_ << "\n\tBuy (Best):\n";
            if (buy_.empty())
            {
                std::cout << "\tOrders\n" << "\t\tNone\n";
            }
            else
            {

                common::market_data::order o = buy_.top();
                std::cout << "\t\tref: " << o.ref << "\n"
                          << "\t\tshares: " << o.shares << "\n"
                          << "\t\tprice: " << o.price << "\n";
            }

            std::cout << "\tSell (Best):\n";
            if (sell_.empty())
            {
                std::cout << "\tOrders\n" << "\t\tNone\n";
            }
            else
            {

                common::market_data::order o = sell_.top();
                std::cout << "\t\tref: " << o.ref << "\n"
                          << "\t\tshares: " << o.shares << "\n"
                          << "\t\tprice: " << o.price << "\n";
            }
            std::cout << "==========\n\n";

        }

    private:
        std::string stock_;
        std::priority_queue<
                common::market_data::order,
                std::vector<common::market_data::order>,
                std::less< std::vector <common::market_data::order>::value_type>> buy_;

        std::priority_queue<
                common::market_data::order,
                std::vector<common::market_data::order>,
                std::greater<std::vector<common::market_data::order>::value_type>> sell_;
    };

}
}
#endif //MDFH_ORDER_BOOK_HPP
