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

void handle_client(tcp::socket socket, int client_id) {
    try {
        {
            std::lock_guard<std::mutex> lock(mtx);
            (srv_sock->getClientsMap())[client_id].start_time = std::chrono::steady_clock::now();
        }

        size_t& client_received_bytes = srv_sock->getClientsMap()[client_id].bytes_sent;
        double& speed = srv_sock->getClientsMap()[client_id].speed;

        boost::asio::streambuf streambuffer;

        size_t len_read = boost::asio::read_until(socket, streambuffer, '?');

        std::istream streamstr(&streambuffer);

        std::string file_name;
        std::getline(streamstr, file_name, '?');

        len_read = boost::asio::read_until(socket, streambuffer, '?');

        std::string file_size;
        std::getline(streamstr, file_size, '?');
        std::cout << "File size: " << file_size << std::endl;
        size_t size = std::stoi(file_size);

        createFolder();

        std::string name = srv_sock->getFolderName() + "/";

        if (std::filesystem::exists(name + file_name)) {
            std::string copyName = file_name.substr(0, file_name.find("."));
            copyName += std::to_string(client_id);
            copyName += file_name.substr(file_name.find("."));
            name += copyName;
        }
        else {
            name += file_name;
        }

        std::cout << "File name: " << name << std::endl;

        std::ofstream file(name, std::ios::binary);

        size_t recieved_size = 0;
        char buffer[BUFFER_SIZE_SERVER];

        std::string ready = "READY";
        boost::asio::write(socket, boost::asio::buffer(ready, ready.length()));

        while (recieved_size < size) {
            std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
            
            size_t count_bytes = socket.read_some(boost::asio::buffer(buffer));
            {
                std::lock_guard<std::mutex> lock(mtx);
                client_received_bytes += count_bytes;
            }
            file.write(buffer, count_bytes);
            recieved_size += count_bytes;

            const std::chrono::duration<double, std::milli> elapsed = std::chrono::high_resolution_clock::now() - start;

            speed = count_bytes / elapsed.count();
        }

        std::string status = (recieved_size != size) ? "ERROR" : "OK";

        boost::asio::write(socket, boost::asio::buffer(status, status.length()));
        std::cout << "File received." << std::endl;
    }
    catch (std::exception& e) {
        std::cerr << "error with client_id " << client_id << ": " << e.what() << std::endl;
    }
    {
        std::lock_guard<std::mutex> lock(mtx);
        srv_sock->getClientsMap().erase(client_id);
    }
}

std::string& Server::getFolderName() {
    return folderName;
}

std::unordered_map<int, StatisticsData>& Server::getClientsMap() {
    return clients;
}

Server::Server(boost::asio::io_context& io_context, unsigned short p, std::string f_name) : port(p), acceptor_(io_context), folderName(f_name) {
    endpoint = tcp::endpoint(tcp::v4(), p);
    openSocket();
    bindSocket();
    listenSocket();
    start_time_server = std::chrono::steady_clock::now();
}

void Server::openSocket() {
    acceptor_.open(endpoint.protocol());
}

std::chrono::steady_clock::time_point& Server::getTime() {
    return start_time_server;
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
    int client_id = 1;
    while(true) {
        tcp::socket clt_sock(io_context);
        acceptSocket(clt_sock);
        std::cout << "Added new client..." << std::endl;
        std::thread(handle_client, std::move(clt_sock), client_id++).detach();
    }
}

Server::~Server() {
    acceptor_.close();
}