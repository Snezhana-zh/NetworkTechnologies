#include "server.hpp"
#include "client.hpp"

Server* srv_sock;

int main(int argc, char* argv[]) {
    if (argc != 2 && argc != 5) {
        std::cerr << "Invalid args!" << std::endl;
        return 1;
    }
    try {
        if (argc == 2) {
            boost::asio::io_context io_context;
            srv_sock = new Server(io_context, std::stoi(argv[1]));
            srv_sock->run(io_context);
        }
        else {
            run_client(argv[1], argv[2], argv[3], argv[4]);
        }
    }
    catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}