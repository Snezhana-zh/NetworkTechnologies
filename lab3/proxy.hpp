#ifndef PROXY_H
#define PROXY_H

#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <boost/array.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "session.hpp"

class Proxy : public boost::enable_shared_from_this<Proxy> {
    public:
        Proxy(boost::asio::io_context& io_context, unsigned short p);

    private:
        void start_accept();

        void handle_client(const boost::system::error_code& error);

        tcp::acceptor acceptor_;
        tcp::socket socket_;
        boost::asio::io_context& io_context_;
};

#endif