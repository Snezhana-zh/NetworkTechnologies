#ifndef CLIENT_H
#define CLIENT_H
#include <iostream>
#include <boost/asio.hpp>
#include <fstream>

#define BUFFER_SIZE 1024
#define STATUS_SIZE 6

using boost::asio::ip::tcp;

void run_client(std::string file_name, std::string file_size, std::string ip, std::string port);

#endif