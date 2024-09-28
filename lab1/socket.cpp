#include "socket.hpp"

Socket::Socket(boost::asio::io_context& io_context, bool is_ipv6, unsigned short p) : use_ipv6(is_ipv6), port(p), socket_(io_context) {
    if (use_ipv6) {
        endpoint = udp::endpoint(boost::asio::ip::address_v6::any(), port);
    } else {
        endpoint = udp::endpoint(boost::asio::ip::address_v4::any(), port);
    }
    openSocket();
    bindSocket();
}

void Socket::openSocket() {
    socket_.open(endpoint.protocol());
}

void Socket::bindSocket() {
    socket_.set_option(boost::asio::socket_base::reuse_address(true));
    socket_.set_option(boost::asio::detail::socket_option::integer<SOL_SOCKET, SO_RCVTIMEO>{ 2000 });
    socket_.bind(endpoint);     
}

udp::endpoint Socket::getEndPoint() const {
    return endpoint;
}

void Socket::joinMulticastGroup(udp::endpoint multicast_endpoint) {
    boost::asio::ip::multicast::join_group join_group_option(multicast_endpoint.address());
    socket_.set_option(join_group_option);
}

size_t Socket:: receive(udp::endpoint& sender_endpoint, boost::system::error_code& error, std::vector<char>& recv_buf) {
    return socket_.receive_from(boost::asio::buffer(recv_buf), sender_endpoint, 0, error);
}

size_t Socket::send(udp::endpoint& endpoint, boost::system::error_code& error, std::vector<char>& send_buf) {
    return socket_.send_to(boost::asio::buffer(send_buf), endpoint, 0, error);
}

Socket::~Socket() {
    socket_.close();
}

const std::string tostring(const std::vector<char>& v) {
    std::ostringstream os;
    for(auto i: v) os << i;
    return os.str();
}

void checkIpV(char* ip, bool& isv6) {
    if (address::from_string(ip).is_v6()) {
        isv6 = true;
    }    
}