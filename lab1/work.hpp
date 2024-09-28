#ifndef WORK_H
#define WORK_H
#include "socket.hpp"

void startWork(udp::endpoint multicast_endpoint, Socket* socket_recv, Socket* socket_send);

void checkCopies(std::chrono::_V2::steady_clock::time_point current_time, udp::endpoint sender_endpoint);

#endif