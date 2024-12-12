#ifndef CLIENT_H
#define CLIENT_H
#include <iostream>
#include <boost/asio.hpp>
#include <fstream>
#include <filesystem>

#define BUFFER_SIZE 1500
#define STATUS_SIZE 6

using boost::asio::ip::tcp;

class Client {
public:
    Client(boost::asio::io_context& io_context, std::string ip, std::string port);
    void run(std::string file_name);
private:
    tcp::socket socket;
};

#endif