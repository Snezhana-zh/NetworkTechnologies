#include "socket.hpp"
#include "work.hpp"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Invalid args!" << std::endl;
        return 1;
    }
    try {
        udp::endpoint multicast_endpoint(address::from_string(argv[1]), MULTICAST_PORT);

        bool isv6 = false;
        checkIpV(argv[1], isv6);
        
        boost::asio::io_context io_context;
        Socket* socket_recv = new Socket(io_context, isv6, MULTICAST_PORT);

        socket_recv->joinMulticastGroup(multicast_endpoint);

        Socket* socket_send = new Socket(io_context, isv6);

        startWork(multicast_endpoint, socket_recv, socket_send);        
    }
    catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
