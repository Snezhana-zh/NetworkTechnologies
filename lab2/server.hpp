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
#include <mutex>

#define BUFFER_SIZE_SERVER 1024

using boost::asio::ip::address;
using boost::asio::ip::tcp;

struct StatisticsData {
    size_t bytes_sent = 0;
    double speed;
    std::chrono::steady_clock::time_point start_time;
};

class Server {
    public:
        Server(boost::asio::io_context& io_context, unsigned short p = 0, std::string f_name = "uploads");

        void run(boost::asio::io_context& io_context);

        std::chrono::steady_clock::time_point& getTime();

        void openSocket();

        void bindSocket();
        
        tcp::endpoint getEndPoint() const;

        void listenSocket();

        std::string& getFolderName();

        void acceptSocket(tcp::socket& sock);

        std::unordered_map<int, StatisticsData>& getClientsMap();

        ~Server();
    private:
        std::unordered_map<int, StatisticsData> clients;
        std::string folderName;
        tcp::acceptor acceptor_;
        tcp::endpoint endpoint;
        unsigned short port;
        std::chrono::steady_clock::time_point start_time_server;
};

extern Server* srv_sock;

extern std::mutex mtx;

void handle_client(tcp::socket socket, int client_id);

#endif