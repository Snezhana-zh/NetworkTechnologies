#ifndef CLIENT_H
#define CLIENT_H
#include <iostream>
#include <boost/asio.hpp>
#include <fstream>
#include <filesystem>

#define BUFFER_SIZE 1500
#define STATUS_SIZE 6

using boost::asio::ip::tcp;

void run_client(std::string file_name, std::string ip, std::string port);

#endif