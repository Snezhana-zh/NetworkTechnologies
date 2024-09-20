#include <iostream>
#include <boost/asio.hpp>
#include <unordered_map>
#include <vector>
#include <chrono>
#include <thread>
#include <numeric>

using boost::asio::ip::udp;
using boost::asio::ip::address;

const int MULTICAST_PORT = 1024;
const int LISTEN_PORT = 1025;
const long TTL = 8000L;
const long UPDATE = 1000L; // 1сек
const std::string MESSAGE = "Hello";

class Socket {
    public:
        Socket(boost::asio::io_context& io_context, bool is_ipv6, unsigned short p = 0) : use_ipv6(is_ipv6), port(p), socket_(io_context) {
            if (use_ipv6) {
                endpoint = udp::endpoint(boost::asio::ip::address_v6::any(), port);
            } else {
                endpoint = udp::endpoint(boost::asio::ip::address_v4::any(), port);
            }
        }

        void openSocket() {
            socket_.open(endpoint.protocol());
        }

        void bindSocket() {
            socket_.set_option(boost::asio::socket_base::reuse_address(true));
            socket_.set_option(boost::asio::detail::socket_option::integer<SOL_SOCKET, SO_RCVTIMEO>{ 2000 });
            socket_.bind(endpoint);            
        }
        
        udp::endpoint getEndPoint() const {
            return endpoint;
        }

        void joinMulticastGroup(udp::endpoint multicast_endpoint) {
            boost::asio::ip::multicast::join_group join_group_option(multicast_endpoint.address());
            socket_.set_option(join_group_option);
        }

        size_t receive(udp::endpoint& sender_endpoint, boost::system::error_code& error, std::vector<char>& recv_buf) {
           return socket_.receive_from(boost::asio::buffer(recv_buf), sender_endpoint, 0, error);
        }

        size_t send(udp::endpoint& endpoint, boost::system::error_code& error, std::vector<char>& send_buf) {
            return socket_.send_to(boost::asio::buffer(send_buf), endpoint, 0, error);
        }

        ~Socket() {
            socket_.close();
        }
    private:
        udp::socket socket_;
        udp::endpoint endpoint;
        unsigned short port;
        bool use_ipv6;
};

const std::string tostring(const std::vector<char>& v) {
    std::ostringstream os;
    for(auto i: v) os << i;
    return os.str();
}

static std::unordered_map<unsigned short, std::chrono::steady_clock::time_point> copies;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Invalid args!" << std::endl;
        return 1;
    }
    try {
        udp::endpoint multicast_endpoint(address::from_string(argv[1]), MULTICAST_PORT);

        bool isv6 = false;
        if (address::from_string(argv[1]).is_v6()) {
            isv6 = true;
        }
        boost::asio::io_context io_context;
        Socket* socket_recv = new Socket(io_context, isv6, MULTICAST_PORT);
        socket_recv->openSocket();
        socket_recv->bindSocket();
        socket_recv->joinMulticastGroup(multicast_endpoint);


        Socket* socket_send = new Socket(io_context, isv6);
        socket_send->openSocket();
        socket_send->bindSocket();


        std::vector<char> recv_buf(256);
        std::vector<char> send_buf(MESSAGE.begin(), MESSAGE.end());
        std::vector<unsigned short> trashBin;

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
             std::cout << "recv_buf: " << tostring(recv_buf) << "\n";

                unsigned short sender_port = sender_endpoint.port();
                std::cout << "sender_port: " << sender_port << std::endl;

                if (copies.find(sender_port) == copies.end()) {                    
                    std::cout << "New copy port " << sender_port << std::endl;
                    copies[sender_port] = current_time;
                }
            }

            for (auto it = copies.begin(); it != copies.end();) {
                auto last_msg_time = it->second;
                if (std::chrono::duration_cast<std::chrono::milliseconds>(current_time - last_msg_time).count() > TTL) {
                    std::cout << "Copy from port " << it->first << " disconnected" << std::endl;
                    it = copies.erase(it);
                }
                else {
                    ++it;
                }
            }
        }
    }
    catch (std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
