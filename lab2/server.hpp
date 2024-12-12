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

#define BUFFER_SIZE_SERVER 1500

#define COUNT_BYTES_IN_KB 1024
#define COUNT_KB_IN_MB 1024

#define COUNT_MLSEC_IN_SEC 1000

using boost::asio::ip::address;
using boost::asio::ip::tcp;

struct StatisticsData {
    size_t bytes_sent = 0;
    double speed;
    std::chrono::steady_clock::time_point start_time;
};

struct Clients {
    std::unordered_map<int, StatisticsData> clients_map;
    std::mutex mtx;
};

class Server {
    public:
        static Server* getServer(boost::asio::io_context& io_context, unsigned short p = 0, std::string f_name = "uploads");

        Server(Server& srv) = delete;

        void operator=(const Server&) = delete;

        void run();

        std::chrono::steady_clock::time_point& getTime();

        void openSocket();

        void bindSocket();

        void listenSocket();

        void acceptSocket(tcp::socket& sock);

        void handleClient(tcp::socket socket, int client_id);

        ~Server();
    private:
        Server(boost::asio::io_context& io_context, unsigned short p, std::string f_name);
        static Server* server;

        Clients clients;
        std::string folder_name;
        tcp::acceptor acceptor_;
        tcp::endpoint endpoint;
        boost::asio::io_context& io_context;
        unsigned short port;
        std::chrono::steady_clock::time_point start_time_server;

        class Speed {
        public:
            void calculateSpeed(Server& server);
        private:
            double avg_speed;
        };
};

#endif