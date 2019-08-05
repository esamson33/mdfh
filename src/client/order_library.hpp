//
// Created by Enrique Jr. Samson on 2019-08-05.
//

#ifndef MDFH_ORDER_LIBRARY_HPP
#define MDFH_ORDER_LIBRARY_HPP


#include <unordered_map>
#include <string>

#include "common/market_data/order.hpp"
#include "order_book.hpp"

namespace mdfh {
namespace client {

    class order_library
    {
    public:
        typedef std::unordered_map<std::string, mdfh::server::order_book> library;
        order_library()
        {
            library_ = {{"IBM", mdfh::server::order_book("IBM")},
                        {"AAPL", mdfh::server::order_book("IBM")}};
        }

        void add_order(mdfh::common::market_data::order::order_ptr order)
        {
            mdfh::server::order_book& ob = library_.at(order->stock);
            ob.receive(order);

            ob.show();
        }

    private:
        library library_;
        std::vector<common::market_data::order> orders_;
    };

} // server
} // mdfh

#endif //MDFH_ORDER_LIBRARY_HPP
