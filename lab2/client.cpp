#include "client.hpp"

Client::Client(boost::asio::io_context& io_context, std::string ip, std::string port) : socket(io_context) {
    tcp::resolver resolver(io_context);
    tcp::resolver::results_type endpoints = resolver.resolve(ip, port);

    boost::asio::connect(socket, endpoints);
}

void sendParams(std::string file_name, tcp::socket& socket) {
    std::string name = file_name + "?";
    size_t len_write = boost::asio::write(socket, boost::asio::buffer(name, name.length()));
    std::cout << "Sended file name: " << file_name << std::endl;

    size_t fileSize;
    std::filesystem::path filePath = std::filesystem::current_path() / file_name;
    if (std::filesystem::exists(filePath) && std::filesystem::is_regular_file(filePath)) {
        fileSize = std::filesystem::file_size(filePath);
    } else {
        std::cerr << "ERROR: The file was not found or is not a regular file." << std::endl;
        fileSize = 0;
        return;
    }

    std::string size = std::to_string(fileSize) + "?";

    size_t len_size = boost::asio::write(socket, boost::asio::buffer(size, size.length()));
    std::cout << "Sended fize size: " << fileSize << std::endl;
}

void Client::run(std::string file_name) {
    try {
        sendParams(file_name, socket);
        
        std::ifstream file(file_name, std::ios::binary);
        file.seekg(0, std::ios::beg);

        char buffer[BUFFER_SIZE];

        char ready[STATUS_SIZE] = "";
        boost::system::error_code error;
        socket.read_some(boost::asio::buffer(ready), error);

        while (!file.eof()) {
            size_t count = file.read(buffer, sizeof(buffer)).gcount();
            boost::asio::write(socket, boost::asio::buffer(buffer, count));
        }

        std::cout << "End of sending data." << std::endl;

        char status[STATUS_SIZE] = "";
        socket.read_some(boost::asio::buffer(status));
        std::cout << "STATUS: " << status << std::endl;

    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
}