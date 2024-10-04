#ifndef SERVER_H
#define SERVER_H
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
        Server(boost::asio::io_context& io_context, unsigned short p = 0, std::string f_name = "uploads");

        void run(boost::asio::io_context& io_context);

        void openSocket();

        void bindSocket();
        
        tcp::endpoint getEndPoint() const;

        void listenSocket();

        std::string& getFolderName();

        void acceptSocket(tcp::socket& sock);

        ~Server();
    private:
        std::string folderName;
        tcp::acceptor acceptor_;
        tcp::endpoint endpoint;
        unsigned short port;
};

extern Server* srv_sock;

void handle_client(tcp::socket socket);

#endif