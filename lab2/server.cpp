#include "server.hpp"

Server* Server::server = nullptr;

bool createFolder(std::string folder_name) {
    bool result = true;
    if (!std::filesystem::exists(folder_name)) {
        if (std::filesystem::create_directory(folder_name)) {
            std::cout << "Folder created successfully." << std::endl;
        } else {
            std::cerr << "Failed to create folder." << std::endl;
            result = false;
        }
    }
    return result;
}

std::string makeFileName(std::string folderName, std::string file_name_param, int client_id) {
    std::string name = folderName + "/";
    if (std::filesystem::exists(name + file_name_param)) {
        std::string copyName = file_name_param.substr(0, file_name_param.find("."));
        copyName += std::to_string(client_id);
        copyName += file_name_param.substr(file_name_param.find("."));
        name += copyName;
    }
    else {
        name += file_name_param;
    }
    return name;
}

void Server::handleClient(tcp::socket socket, int client_id) {
    try {
        {
            std::lock_guard<std::mutex> lock(clients.mtx);
            clients.clients_map[client_id].start_time = std::chrono::steady_clock::now();
        }

        boost::asio::streambuf streambuffer;

        size_t len_read = boost::asio::read_until(socket, streambuffer, '?');

        std::istream streamstr(&streambuffer);

        std::string file_name_param;
        std::getline(streamstr, file_name_param, '?');

        len_read = boost::asio::read_until(socket, streambuffer, '?');

        std::string size;
        std::getline(streamstr, size, '?');
        std::cout << "File size: " << size << std::endl;
        size_t file_size = std::stoi(size);

        createFolder(folder_name);

        std::string file_name = makeFileName(folder_name, file_name_param, client_id);
        std::cout << "File name: " << file_name << std::endl;

        std::ofstream file(file_name, std::ios::binary);

        size_t recieved_size = 0;
        char buffer[BUFFER_SIZE_SERVER];

        std::string ready = "READY";
        boost::asio::write(socket, boost::asio::buffer(ready, ready.length()));

        std::chrono::high_resolution_clock::time_point global_start = std::chrono::high_resolution_clock::now();

        while (recieved_size < file_size) {
            std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
            
            size_t count_bytes = socket.read_some(boost::asio::buffer(buffer));
            {
                std::lock_guard<std::mutex> lock(clients.mtx);
                clients.clients_map[client_id].bytes_sent += count_bytes;
            }
            file.write(buffer, count_bytes);
            recieved_size += count_bytes;

            const std::chrono::duration<double, std::milli> end = std::chrono::high_resolution_clock::now() - start;

            {
                std::lock_guard<std::mutex> lock(clients.mtx);
                clients.clients_map[client_id].speed = ((double)count_bytes / (end.count() / COUNT_MLSEC_IN_SEC)) / COUNT_BYTES_IN_KB;
            }
        }

        std::string status = (recieved_size != file_size) ? "ERROR" : "OK";

        boost::asio::write(socket, boost::asio::buffer(status, status.length()));
        std::cout << "File received." << std::endl;

        const std::chrono::duration<double, std::milli> global_end = std::chrono::high_resolution_clock::now() - global_start;
        if (global_end.count() < 3000) {
            std::this_thread::sleep_for(std::chrono::milliseconds(3000));
        }

        std::lock_guard<std::mutex> lock(clients.mtx);
        clients.clients_map.erase(client_id);
    }
    catch (std::exception& e) {
        std::cerr << "Error with client_id " << client_id << ": " << e.what() << std::endl;
    }
}

Server* Server::getServer(boost::asio::io_context& io_context, unsigned short p, std::string f_name) {
    if (server == nullptr) {
        server = new Server(io_context, p, f_name);
    }
    return server;
}

Server::Server(boost::asio::io_context& io_context_, unsigned short p, std::string f_name) : port(p), acceptor_(io_context_), folder_name(f_name), io_context(io_context_) {
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

void Server::listenSocket() {
    acceptor_.listen(5);
}

void Server::acceptSocket(tcp::socket& sock) {
    acceptor_.accept(sock);
}

void Server::run() {
    Speed speed_worker;
    std::thread([this, &speed_worker]() {
            speed_worker.calculateSpeed(*this);
        }).detach();

    int client_id = 0;
    while(true) {
        tcp::socket clt_sock(io_context);
        acceptSocket(clt_sock);
        std::cout << "Added new client " << ++client_id << std::endl;
        std::thread([this, socket = std::move(clt_sock), client_id]() mutable {
                handleClient(std::move(socket), client_id);
            }).detach();
    }
}

Server::~Server() {
    acceptor_.close();
}