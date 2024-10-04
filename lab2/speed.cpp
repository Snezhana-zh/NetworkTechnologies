#include "speed.hpp"

// std::mutex mtx;
void calculate_speed() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(3));

        std::lock_guard<std::mutex> lock(mtx);
        std::cout << "\n=== Average Speed Statistics ===" << std::endl;
        
        // Рассчитываем и выводим скорость для сервера
        auto working_time_server = std::chrono::duration_cast<std::chrono::seconds>(
                std::chrono::steady_clock::now() - srv_sock->getTime()
            ).count();

        double avg_speed_server = working_time_server > 0 ? (static_cast<double>(srv_sock->getTotalBytes()) / 1024) /working_time_server : 0;

        std::cout << "Server Average Sent Speed: " << avg_speed_server << " Kbytes/sec" << std::endl;
        // std::cout << "Server Total Bytes Sent: " << srv_sock->getTotalBytes() << " bytes" << std::endl;

        for (const auto& [client_id, data] : srv_sock->getClientsMap()) {
            auto working_time = std::chrono::duration_cast<std::chrono::seconds>(
                std::chrono::steady_clock::now() - data.start_time
            ).count();
            
            double avg_speed = working_time > 0 ? (static_cast<double>(data.bytes_sent) / 1024) / working_time : 0;

            std::cout << "Client " << client_id << " Average Sent Speed: " << avg_speed << " Kbytes/sec" << std::endl;
        }
        std::cout << "=============================\n" << std::endl;
    }
}