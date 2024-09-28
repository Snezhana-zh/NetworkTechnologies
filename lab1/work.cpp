#include "work.hpp"

std::unordered_map<unsigned short, std::chrono::steady_clock::time_point> copies;

void printCopies() {
    std::cout << "Live copies list: [ ";
    for (auto it = copies.begin(); it != copies.end(); ++it) {
        std::cout << it->first << " ";
    }
    std::cout << "]" << std::endl;
}

void checkCopies(std::chrono::_V2::steady_clock::time_point current_time, udp::endpoint sender_endpoint) {
    unsigned short sender_port = sender_endpoint.port();
    bool was_update = false;

    if (copies.find(sender_port) == copies.end()) {                    
        std::cout << "NEW copy port: " << sender_port << std::endl;
        was_update = true;
    }

    copies[sender_port] = current_time;
    
    for (auto it = copies.begin(); it != copies.end();) {
        auto last_msg_time = it->second;
        if (std::chrono::duration_cast<std::chrono::milliseconds>(current_time - last_msg_time).count() > TTL) {
            std::cout << "DISCONNECTED copy port: " << it->first << std::endl;
            it = copies.erase(it);
            was_update = true;
        }
        else {
            ++it;
        }
    }
    if (was_update) printCopies();
}

void startWork(udp::endpoint multicast_endpoint, Socket* socket_recv, Socket* socket_send) {
    std::vector<char> recv_buf(256);
    std::vector<char> send_buf(MESSAGE.begin(), MESSAGE.end());

    auto last_ping_time = std::chrono::steady_clock::now();

    while (true) {
        auto current_time = std::chrono::steady_clock::now();
        auto time_since_last_ping = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - last_ping_time).count();

        if (time_since_last_ping > UPDATE) {
            boost::system::error_code error;

            size_t len = socket_send->send(multicast_endpoint, error, send_buf);
            if (error) {
                std::cerr << "Error sending data: " << error.message() << " (code: " << error.value() << ")" << std::endl;
            }
            last_ping_time = current_time;
        }

        udp::endpoint sender_endpoint;
        boost::system::error_code error;

        socket_recv->receive(sender_endpoint, error, recv_buf);
        if (!error) {
            checkCopies(current_time, sender_endpoint);
        }
    }
}