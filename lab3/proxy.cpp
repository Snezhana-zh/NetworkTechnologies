#include "proxy.hpp"

Proxy::Proxy(boost::asio::io_context& io_context, unsigned short port)
        : io_context_(io_context),
          acceptor_(io_context, tcp::endpoint(tcp::v4(), port)),
          socket_(io_context) {
    start_accept();
}

void Proxy::start_accept() {
    acceptor_.async_accept(socket_, [this](const boost::system::error_code& error) {
        if (!error) {
            handle_client(error);
        } else {
            std::cerr << "Error code: " << error.value() << std::endl;
            std::cerr << "Error message: " << error.message() << std::endl;
        }
        start_accept();
    });
}

void Proxy::handle_client(const boost::system::error_code& error) {
    std::cout << "New client...\n";
    try {
        tcp::resolver resolver_(io_context_);
        tcp::socket server_(io_context_);
        auto session = std::make_shared<Session>(std::move(socket_), std::move(server_), std::move(resolver_));

        session->start_greeting();

    } catch (std::exception& e) {
        std::cerr << "Exception in handle_client: " << e.what() << std::endl;
    }
}