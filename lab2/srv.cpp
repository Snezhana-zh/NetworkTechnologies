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
            openSocket();
            bindSocket();
            listenSocket();
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

Server* srv_sock;

void handle_client(tcp::socket socket) {
    try {        
        boost::asio::streambuf streambuffer;

        size_t len_read = boost::asio::read_until(socket, streambuffer, '?');
        std::istream streamstr(&streambuffer);

        std::string file_name;
        std::getline(streamstr, file_name, '?');

        std::cout << "len_read: " << len_read << std::endl;
        std::cout << "File name: " << file_name << std::endl;


        len_read = boost::asio::read_until(socket, streambuffer, '?');
        std::string file_size;
        std::getline(streamstr, file_size, '?');

        std::cout << "len_read: " << len_read << std::endl;
        std::cout << "File size: " << file_size << std::endl;

        size_t size = std::stoi(file_size);

        std::string folderName = "uploads";

        if (!std::filesystem::exists(folderName)) {
            if (std::filesystem::create_directory(folderName)) {
                std::cout << "Folder created successfully." << std::endl;
            } else {
                std::cerr << "Failed to create folder." << std::endl;
                return;
            }
        }

        std::string name = "uploads/";

        name += file_name;

        std::ofstream file(name, std::ios::binary);

        size_t recieved_size = 0;
        char buffer[1024];

        while (recieved_size < size) {
            size_t count_bytes = socket.read_some(boost::asio::buffer(buffer));
            // std::cout << "count_bytes: " << count_bytes << std::endl;
            file.write(buffer, count_bytes);
            recieved_size += count_bytes;
        }

        std::string status;
        std::cout << "recieved_size = " << recieved_size << std::endl;

        if (recieved_size != size) status = "ERROR";
        else status = "OK";

        boost::asio::write(socket, boost::asio::buffer(status, status.length()));
        std::cout << "File received." << std::endl;
    }
    catch (std::exception& e) {
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
        srv_sock = new Server(io_context, std::stoi(argv[1]));
        while(1) {
            tcp::socket clt_sock(io_context);
            srv_sock->acceptSocket(clt_sock);
            std::cout << "Added new client..." << std::endl;
            std::thread(handle_client, std::move(clt_sock)).detach();
        }
    }
    catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}