#include "server.hpp"

bool createFolder() {
    bool result = true;
    if (!std::filesystem::exists(srv_sock->getFolderName())) {
        if (std::filesystem::create_directory(srv_sock->getFolderName())) {
            std::cout << "Folder created successfully." << std::endl;
        } else {
            std::cerr << "Failed to create folder." << std::endl;
            result = false;
        }
    }
    return result;
}

void handle_client(tcp::socket socket) {
    try {        
        boost::asio::streambuf streambuffer;

        size_t len_read = boost::asio::read_until(socket, streambuffer, '?');
        std::istream streamstr(&streambuffer);

        std::string file_name;
        std::getline(streamstr, file_name, '?');
        // std::cout << "len_read: " << len_read << std::endl;
        std::cout << "File name: " << file_name << std::endl;

        len_read = boost::asio::read_until(socket, streambuffer, '?');
        std::string file_size;
        std::getline(streamstr, file_size, '?');
        // std::cout << "len_read: " << len_read << std::endl;
        std::cout << "File size: " << file_size << std::endl;
        size_t size = std::stoi(file_size);

        createFolder();

        std::string name = srv_sock->getFolderName() + "/";
        name += file_name;
        std::ofstream file(name, std::ios::binary);

        size_t recieved_size = 0;
        char buffer[1024];

        while (recieved_size < size) {
            size_t count_bytes = socket.read_some(boost::asio::buffer(buffer));
            // std::cout << "count_bytes: " << count_bytes << std::endl;
            file.write(buffer, count_bytes);
            recieved_size += count_bytes;
        }

        // std::cout << "recieved_size = " << recieved_size << std::endl;
        std::string status = (recieved_size != size) ? "ERROR" : "OK";

        // if (recieved_size != size) status = "ERROR";
        // else status = "OK";

        boost::asio::write(socket, boost::asio::buffer(status, status.length()));
        std::cout << "File received." << std::endl;
    }
    catch (std::exception& e) {
        std::cerr << "error: " << e.what() << std::endl;
    }
}

std::string& Server::getFolderName() {
    return folderName;
}

Server::Server(boost::asio::io_context& io_context, unsigned short p, std::string f_name) : port(p), acceptor_(io_context), folderName(f_name) {
    endpoint = tcp::endpoint(tcp::v4(), p);
    openSocket();
    bindSocket();
    listenSocket();
}

void Server::openSocket() {
    acceptor_.open(endpoint.protocol());
}

void Server::bindSocket() {
    acceptor_.bind(endpoint);            
}

tcp::endpoint Server::getEndPoint() const {
    return endpoint;
}

void Server::listenSocket() {
    acceptor_.listen(5);
}

void Server::acceptSocket(tcp::socket& sock) {
    acceptor_.accept(sock);
}

void Server::run(boost::asio::io_context& io_context) {
    // boost::asio::io_context io_context;
    while(1) {
        tcp::socket clt_sock(io_context);
        acceptSocket(clt_sock);
        std::cout << "Added new client..." << std::endl;
        std::thread(handle_client, std::move(clt_sock)).detach();
    }
}

Server::~Server() {
    acceptor_.close();
}