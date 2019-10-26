#include <iostream>
#include "udp_multicast.hpp"

int main(int argc, char* argv[])
{
    std::cout << "Hello server!\n";

    try
    {
        if (argc != 2)
        {
            std::cerr << "Usage: sender <multicast_address>\n";
            std::cerr << "  For IPv4, try:\n";
            std::cerr << "    server 239.255.0.1\n";
            std::cerr << "  For IPv6, try:\n";
            std::cerr << "    server ff31::8000:1234\n";
            return 1;
        }

        io_context io_context;
        mdfh::udp_multicast_sender s(io_context, ip::make_address(argv[1]));
        io_context.run();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}

