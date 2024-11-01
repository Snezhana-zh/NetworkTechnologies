#include "session.hpp"

Session::Session(tcp::socket socket, tcp::socket srv, tcp::resolver resolver) : 
                                    client_socket_(std::move(socket)), 
                                    server_socket_(std::move(srv)), 
                                    resolver_(std::move(resolver))  {}

void Session::start_greeting() {
    read_client_greeting();
}

void Session::read_client_greeting() {
    auto self(shared_from_this());
    client_socket_.async_read_some(boost::asio::buffer(data_, MAX_LEN),
                                    boost::bind(&Session::handle_client_greeting, self,
                                                boost::asio::placeholders::error,
                                                boost::asio::placeholders::bytes_transferred));
}

void Session::handle_client_greeting(const boost::system::error_code& error, size_t bytes_transferred) {
    if (bytes_transferred < 2 || data_.at(0) != Bits::VERTION) {
        return;
    }

    uint8_t methods_count = data_.at(1);
    bool no_auth_method_found = false;
    for (uint8_t i = 0; i < methods_count; ++i) {
        if (data_.at(2 + i) == 0) {
            no_auth_method_found = true;
            break;
        }
    }

    if (!no_auth_method_found) {
        return;
    }

    uint8_t response[] = {Bits::VERTION, Bits::METHOD_NUM};
    boost::asio::async_write(client_socket_, boost::asio::buffer(response, sizeof(response)),
                                    boost::bind(&Session::handle_server_choice, shared_from_this(),
                                                boost::asio::placeholders::error, 
                                                boost::asio::placeholders::bytes_transferred));
}

void Session::handle_server_choice(const boost::system::error_code& error, size_t bytes_transferred) {
    if(!error) {
        read_client_request();
    }
    else {
        std::cerr << "Error code: " << error.value() << std::endl;
        std::cerr << "Error message: " << error.message() << std::endl;
    }
}

void Session::read_client_request() {
    client_socket_.async_read_some(boost::asio::buffer(data_, MAX_LEN),
                                    boost::bind(&Session::handle_client_request, shared_from_this(),
                                                boost::asio::placeholders::error,
                                                boost::asio::placeholders::bytes_transferred));
}

void Session::handle_client_request(const boost::system::error_code& error, size_t bytes_transferred) {
    if (error) {
        std::cerr << "Error code: " << error.value() << std::endl;
        std::cerr << "Error message: " << error.message() << std::endl;
        return;
    }
    if (bytes_transferred < 5 || data_.at(0) != Bits::VERTION || data_.at(1) != Bits::COMAND_CODE) {
        return;
    }

    std::string host;
    uint16_t port;

    if (data_.at(3) == Bits::ADDRESS_TYPE_Domen) {
        uint8_t len = data_.at(4);
        host = "";
        for (int i = 0; i < len; i++) {
            host += data_.at(5 + i);
        }
        port = (data_.at(5 + len) << 8) | data_.at(6 + len);
    } 
    else if (data_.at(3) == Bits::ADDRESS_TYPE_Ipv4) {
        host = std::to_string(data_.at(4)) + "." + std::to_string(data_.at(5)) + "." +
                std::to_string(data_.at(6)) + "." + std::to_string(data_.at(7));
        port = ntohs(*reinterpret_cast<const uint16_t*>(&data_[8]));
    } 
    else {
        return;
    }

    auto self(shared_from_this());
    resolver_.async_resolve(host, std::to_string(port),
                            [self](const boost::system::error_code& error, tcp::resolver::results_type results) {
                                self->handle_resolve(error, results);
                            });
}

void Session::handle_resolve(const boost::system::error_code& error, tcp::resolver::results_type results) {
    if (!error) {
        auto self(shared_from_this());
        boost::asio::async_connect(server_socket_, results, 
                                                [self](const boost::system::error_code& error, const tcp::endpoint& endpoint) {
                                                    self->handle_connect(error, endpoint);
                                                });
    }
    else {
        std::cerr << "Error code: " << error.value() << std::endl;
        std::cerr << "Error message: " << error.message() << std::endl;
    }
}

void Session::handle_connect(const boost::system::error_code& error, const tcp::endpoint& endpoint) {
    if (!error) {
        // Отправка ответа клиенту
        uint8_t response[] = {Bits::VERTION, Bits::REQUEST_GRATED, 0x00, Bits::ADDRESS_TYPE_Ipv4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
        *reinterpret_cast<uint32_t*>(&response[4]) = htonl(server_socket_.remote_endpoint().address().to_v4().to_ulong());
        *reinterpret_cast<uint16_t*>(&response[8]) = htons(server_socket_.remote_endpoint().port());

        boost::asio::async_write(client_socket_, boost::asio::buffer(response, sizeof(response)),
                                    boost::bind(&Session::start_relay, shared_from_this(),
                                                boost::asio::placeholders::error, 
                                                boost::asio::placeholders::bytes_transferred));
    }
    else {
        std::cerr << "Error code: " << error.value() << std::endl;
        std::cerr << "Error message: " << error.message() << std::endl;
    }
}

void Session::start_relay(const boost::system::error_code& error, size_t bytes_transferred) {
    if (!error) {
        // Начинаем передачу данных между клиентом и сервером
        read_from_client();
        read_from_server();
    }
}

void Session::read_from_client() {
    client_socket_.async_read_some(boost::asio::buffer(data_, MAX_LEN),
                                    boost::bind(&Session::handle_client_read, shared_from_this(),
                                                boost::asio::placeholders::error,
                                                boost::asio::placeholders::bytes_transferred));
}

void Session::handle_client_read(const boost::system::error_code& error, size_t bytes_transferred) {
    if (!error) {
        auto self(shared_from_this());
        boost::asio::async_write(server_socket_, boost::asio::buffer(data_, bytes_transferred),
                                    [self](const boost::system::error_code& error, size_t bytes_transferred) {
                                        self->read_from_client();
                                    });
    }
}

void Session::read_from_server() {
    server_socket_.async_read_some(boost::asio::buffer(data_, MAX_LEN),
                                    boost::bind(&Session::handle_server_read, shared_from_this(),
                                                boost::asio::placeholders::error,
                                                boost::asio::placeholders::bytes_transferred));
}

void Session::handle_server_read(const boost::system::error_code& error, size_t bytes_transferred) {
    if (!error) {
        auto self(shared_from_this());
        boost::asio::async_write(client_socket_, boost::asio::buffer(data_, bytes_transferred),
                                    [self](const boost::system::error_code& error, size_t bytes_transferred) {
                                        self->read_from_server();
                                    });
    }
}