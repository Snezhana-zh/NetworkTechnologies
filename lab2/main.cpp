#include "server.hpp"
#include "client.hpp"
#include "speed.hpp"

Server* srv_sock;

std::mutex mtx;

int main(int argc, char* argv[]) {
    if (argc != 2 && argc != 5) {
        std::cerr << "Invalid args!" << std::endl;
        return 1;
    }
    try {
        if (argc == 2) {
            std::thread(calculate_speed).detach();
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

// to do: узнать оптимальный размер буфера для передачи - сравнить скорости, узнать mtu, проверить с другим ноутом