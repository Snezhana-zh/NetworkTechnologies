#ifndef SESSION_H
#define SESSION_H

#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <boost/endian/conversion.hpp>

using boost::asio::ip::tcp;
#define MAX_LEN 8*1024
#define MIN_LENGTH_GREETING 2
#define MIN_LENGTH_REQUEST 7
#define VERTION_IN_ARRAY 0
#define METHOD_COUNT 1
#define LEN_NAME 4
#define ADDRESS_TYPE 3

enum Bytes {
    VERTION = 0x05,
    METHOD_NUM = 0x00,
    COMAND_CODE = 0X01,
    ADDRESS_TYPE_Ipv4 = 0x01,
    ADDRESS_TYPE_Domen = 0x03,
    REQUEST_GRATED = 0x00
};

class Session : public std::enable_shared_from_this<Session> {
public:
    Session(tcp::socket socket, tcp::socket srv, tcp::resolver);

    void start_greeting();

private:

    void read_client_greeting();

    void handle_client_greeting(const boost::system::error_code& error, size_t bytes_transferred);

    void handle_server_choice(const boost::system::error_code& error, size_t bytes_transferred);

    void read_client_request();

    void handle_client_request(const boost::system::error_code& error, size_t bytes_transferred);

    void handle_resolve(const boost::system::error_code& error, tcp::resolver::results_type results);

    void handle_connect(const boost::system::error_code& ec, const tcp::endpoint& endpoint);

    void start_relay(const boost::system::error_code& error, size_t bytes_transferred);

    void read_from_client();

    void handle_client_read(const boost::system::error_code& error, size_t bytes_transferred);

    void read_from_server();

    void handle_server_read(const boost::system::error_code& error, size_t bytes_transferred);

    tcp::socket client_socket_;
    tcp::socket server_socket_;
    tcp::resolver resolver_;

    std::array<unsigned char, MAX_LEN> data_;
};

#endif