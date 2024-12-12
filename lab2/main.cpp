#include "server.hpp"
#include "client.hpp"

int main(int argc, char* argv[]) {
    if (argc != 2 && argc != 4) {
        std::cerr << "Invalid args!" << std::endl;
        return 1;
    }
    try {
        boost::asio::io_context io_context;
        if (argc == 2) {
            Server* server = Server::getServer(io_context, std::stoi(argv[1]));
            server->run();
        }
        else {
            Client client(io_context, argv[2], argv[3]);
            client.run(argv[1]);
        }
    }
    catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}