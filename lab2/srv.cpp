#include <iostream>
#include <boost/asio.hpp>
#include <unordered_map>
#include <vector>
#include <chrono>
#include <thread>
#include <numeric>
#include <filesystem>
#include <fstream>
#include <thread>

using boost::asio::ip::address;
using boost::asio::ip::tcp;

class Server {
    public:
        Server(boost::asio::io_context& io_context, unsigned short p = 0) : port(p), acceptor_(io_context) {
            endpoint = tcp::endpoint(tcp::v4(), p);
        }

        void openSocket() {
            acceptor_.open(endpoint.protocol());
        }

        void bindSocket() {
            acceptor_.bind(endpoint);            
        }
        
        tcp::endpoint getEndPoint() const {
            return endpoint;
        }

        void listenSocket() {
            acceptor_.listen(5);
        }

        void acceptSocket(tcp::socket& sock) {
            acceptor_.accept(sock);
        }

        ~Server() {
            acceptor_.close();
        }
    private:
        tcp::acceptor acceptor_;
        tcp::endpoint endpoint;
        unsigned short port;
};

std::list<tcp::socket> sockets; // сделать у сервера как private поле, 
// сервер сделать синглтон и вызывать метод получения сокета из функции handler

boost::system::error_code error;

void handle_client() {
    try {
        if (sockets.empty()) {
            return;            
        }
        tcp::socket& socket = sockets.back(); // сделать внутри сервера мьютекс
        
        // char file_name[256];
        boost::asio::streambuf streambuffer;
        size_t len_read = boost::asio::read_until(socket, streambuffer, '?');
        std::istream streamstr(&streambuffer);

        std::string file_name;
        std::getline(streamstr, file_name, '?');
        // size_t len_read = socket.read_some(boost::asio::buffer(file_name), error);
        // if (error) {
        //     std::cerr << "read_some() failed: " << error.message() << std::endl;
        //     return;
        // }
        // file_name[len_read - 1] = '\0';
        std::cout << "len_read: " << len_read << std::endl;
        std::cout << "File name: " << file_name << std::endl;

        // char file_size[20];
        // len_read = socket.read_some(boost::asio::buffer(file_size), error);
        // if (error) {
        //     std::cerr << "read_some() failed: " << error.message() << std::endl;
        //     return;
        // }
        // file_size[len_read] = '\0';

        len_read = boost::asio::read_until(socket, streambuffer, '?');

        // std::istream streamstr(&streambuffer);

        std::string file_size;
        std::getline(streamstr, file_size, '?');

        std::cout << "len_read: " << len_read << std::endl;
        std::cout << "File size: " << file_size << std::endl;

        size_t size = std::stoi(file_size);

        std::string folderName = "uploads";

        if (std::filesystem::create_directory(folderName)) {
            std::cout << "Folder created successfully." << std::endl;
        } else {
            std::cerr << "Failed to create folder." << std::endl;
        }

        std::string name = "uploads/";

        name += file_name;

        std::ofstream file(name, std::ios::binary);

        size_t recieved_size = 0;
        char buffer[1024];

        while (recieved_size < size) {
            size_t count_bytes = socket.read_some(boost::asio::buffer(buffer));
            std::cout << "count_bytes: " << count_bytes << std::endl;
            file.write(buffer, count_bytes);
            recieved_size += count_bytes;
        } //если ошибка, написать клиенкту error

        std::string status;
        std::cout << "recieved_size = " << recieved_size << std::endl;

        if (recieved_size != size) status = "ERROR";
        else status = "OK";

        boost::asio::write(socket, boost::asio::buffer(status, status.length()));
        std::cout << "File received." << std::endl;
        //закрыть соединение

    } catch (std::exception& e) {
        std::cerr << "error: " << e.what() << std::endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Invalid args!" << std::endl;
        return 1;
    }
    try {
        boost::asio::io_context io_context;
        Server* srv_sock = new Server(io_context, std::stoi(argv[1]));
        srv_sock->openSocket();
        srv_sock->bindSocket();
        srv_sock->listenSocket();

        while(1) {
            tcp::socket clt_sock(io_context);
            srv_sock->acceptSocket(clt_sock);
            std::cout << "Added new client..." << std::endl;
            sockets.push_back(std::move(clt_sock));
            std::thread thread(handle_client);
            thread.detach();
        }
    }
    catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}