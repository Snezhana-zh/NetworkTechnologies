#include "client.hpp"

void run_client(std::string file_name, std::string file_size, std::string ip, std::string port) {
    try {
        boost::asio::io_context io_context;
        tcp::socket socket(io_context);

        tcp::resolver resolver(io_context);
        tcp::resolver::results_type endpoints = resolver.resolve(ip, port);

        boost::asio::connect(socket, endpoints);

        std::string name = file_name + "?";
        size_t len_write = boost::asio::write(socket, boost::asio::buffer(name, name.length()));
        std::cout << "Sended message: " << name << std::endl;
        // std::cout << "len_write: " << len_write << std::endl;

        std::string size = file_size + "?";
        size_t len_size = boost::asio::write(socket, boost::asio::buffer(size, size.length()));
        std::cout << "Sended message: " << file_size << std::endl;
        // std::cout << "len_size: " << len_size << std::endl;

        std::ifstream file(file_name, std::ios::binary);
        file.seekg(0, std::ios::beg);

        char buffer[1024];

        char ready[6] = "";
        boost::system::error_code error1;
        socket.read_some(boost::asio::buffer(ready), error1);

        while (!file.eof()) {
            size_t count = file.read(buffer, sizeof(buffer)).gcount();
            boost::asio::write(socket, boost::asio::buffer(buffer, count));
        }

        std::cout << "End of sending data." << std::endl;

        char status[6] = "";
        socket.read_some(boost::asio::buffer(status));
        std::cout << "STATUS: " << status << std::endl;

    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
}