#include "server.hpp"

void Server::Speed::calculateSpeed(Server& server) {
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(3));

        std::lock_guard<std::mutex> lock(server.clients.mtx);
        std::cout << "\n=== Speed Statistics ===" << std::endl;
    
        for (const auto& [client_id, data] : server.clients.clients_map) {
            auto working_time = std::chrono::duration_cast<std::chrono::seconds>(
                std::chrono::steady_clock::now() - data.start_time
            ).count();
            
            avg_speed = working_time > 0 ? (static_cast<double>(data.bytes_sent) / COUNT_BYTES_IN_KB) / working_time : 0;

            std::cout << "Client " << client_id << " Average Sent Speed: " << avg_speed / COUNT_KB_IN_MB << " Mb/sec" << std::endl;

            std::cout << "Client " << client_id << " Sent Speed: " << data.speed / COUNT_KB_IN_MB << " Mb/sec" << std::endl;
        }
        std::cout << "=========================\n" << std::endl;
    }
}