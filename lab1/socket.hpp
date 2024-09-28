#ifndef SOCKET_H
#define SOCKET_H

#include <iostream>
#include <boost/asio.hpp>
#include <unordered_map>
#include <vector>
#include <chrono>
#include <thread>
#include <numeric>

using boost::asio::ip::udp;
using boost::asio::ip::address;

const int MULTICAST_PORT = 1024;
const int LISTEN_PORT = 1025;
const long TTL = 5000L;
const long UPDATE = 1000L; // 1сек
const std::string MESSAGE = "Hello";

class Socket {
    public:
        Socket(boost::asio::io_context& io_context, bool is_ipv6, unsigned short p = 0);

        void openSocket();

        void bindSocket();
        
        udp::endpoint getEndPoint() const;

        void joinMulticastGroup(udp::endpoint multicast_endpoint);

        size_t receive(udp::endpoint& sender_endpoint, boost::system::error_code& error, std::vector<char>& recv_buf);

        size_t send(udp::endpoint& endpoint, boost::system::error_code& error, std::vector<char>& send_buf);

        ~Socket();
    private:
        udp::socket socket_;
        udp::endpoint endpoint;
        unsigned short port;
        bool use_ipv6;
};

const std::string tostring(const std::vector<char>& v);

void checkIpV(char* ip, bool& isv6);

#endif