#include <iostream>
#include <boost/asio.hpp>
#include "udp_receiver.hpp"

int main(int argc, char* argv[])
{
    try
    {
        if (argc != 3)
        {
            std::cerr << "Usage: receiver <listen_address> <multicast_address>\n";
            std::cerr << "  For IPv4, try:\n";
            std::cerr << "    client 0.0.0.0 239.255.0.1\n";
            std::cerr << "  For IPv6, try:\n";
            std::cerr << "    client 0::0 ff31::8000:1234\n";
            return 1;
        }

        boost::asio::io_context io_context;
        mdfh::client::udp_multicast_receiver r(io_context,
                   boost::asio::ip::make_address(argv[1]),
                   boost::asio::ip::make_address(argv[2]));
        io_context.run();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
