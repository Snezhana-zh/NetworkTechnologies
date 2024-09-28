#include <iostream>
#include <boost/asio.hpp>
#include <fstream>

using boost::asio::ip::tcp;

int main(int argc, char* argv[]) {
    try {
        if (argc != 5) {
            std::cerr << "Invalid args!" << std::endl;
            return 1;
        }

        // char* file_name = argv[1];
        // char* file_size = argv[2];
        char* ip = argv[3];
        char* port = argv[4];

        std::string file_name(argv[1]);
        std::string file_size(argv[2]);

        boost::asio::io_context io_context;
        tcp::socket socket(io_context);

        tcp::resolver resolver(io_context);
        tcp::resolver::results_type endpoints = resolver.resolve(ip, port);

        boost::asio::connect(socket, endpoints);

        std::string name = file_name + "?";
        size_t len_write = boost::asio::write(socket, boost::asio::buffer(name, name.length()));
        std::cout << "Sended message: " << name << std::endl;
        std::cout << "len_write: " << len_write << std::endl;

        std::string size = file_size + "?";
        size_t len_size = boost::asio::write(socket, boost::asio::buffer(size, size.length()));
        std::cout << "Sended message: " << file_size << std::endl;
        std::cout << "len_size: " << len_size << std::endl;

        std::ifstream file(file_name, std::ios::binary);
        file.seekg(0, std::ios::beg);

        char buffer[1024];

        while (!file.eof()) {
            size_t count = file.read(buffer, sizeof(buffer)).gcount();
            std::cout << "count: " << count << std::endl;
            boost::asio::write(socket, boost::asio::buffer(buffer, count));
        }

        char status[4];
        boost::system::error_code error;
        socket.read_some(boost::asio::buffer(status), error);
        if (error) {
            std::cerr << "read_some() failed: " << error.message() << std::endl;
        } else {
            std::cout << "STATUS: " << status << std::endl;
        }
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}