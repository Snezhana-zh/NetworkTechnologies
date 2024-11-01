#include <iostream>
#include <boost/asio.hpp>
#include "proxy.hpp"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: socks_proxy <port>" << std::endl;
        return 1;
    }

    unsigned short port = std::atoi(argv[1]);

    boost::asio::io_context io_context;

    boost::shared_ptr<Proxy> proxy(new Proxy(io_context, port));

    std::cout << "Server started!\n";

    io_context.run();

    return 0;
}