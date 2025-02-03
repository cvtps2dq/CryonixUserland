//
// Created by cv2 on 03.02.2025.
//

#include <iostream>
#include <chrono>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/ip_icmp.h>
#include <netdb.h>
#include <unistd.h>

constexpr int PACKET_SIZE = 64;
constexpr int TIMEOUT_SEC = 1;

uint16_t checksum(void* data, int len) {
    uint32_t sum = 0;
    uint16_t* ptr = static_cast<uint16_t*>(data);
    while (len > 1) {
        sum += *ptr++;
        len -= 2;
    }
    if (len == 1) {
        sum += *reinterpret_cast<uint8_t*>(ptr);
    }
    sum = (sum & 0xFFFF) + (sum >> 16);
    sum += (sum >> 16);
    return ~static_cast<uint16_t>(sum);
}

void send_ping(const std::string& target) {
    int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sockfd < 0) {
        perror("Socket error");
        exit(EXIT_FAILURE);
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    hostent* host = gethostbyname(target.c_str());
    if (!host) {
        std::cerr << "Error: Unknown host " << target << "\n";
        exit(EXIT_FAILURE);
    }
    std::memcpy(&addr.sin_addr, host->h_addr, host->h_length);

    icmphdr icmp_req{};
    icmp_req.type = ICMP_ECHO;
    icmp_req.code = 0;
    icmp_req.un.echo.id = getpid() & 0xFFFF;
    icmp_req.un.echo.sequence = 1;
    icmp_req.checksum = checksum(&icmp_req, sizeof(icmp_req));

    auto start_time = std::chrono::high_resolution_clock::now();
    if (sendto(sockfd, &icmp_req, sizeof(icmp_req), 0, (sockaddr*)&addr, sizeof(addr)) <= 0) {
        perror("Send error");
        exit(EXIT_FAILURE);
    }

    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(sockfd, &readfds);
    timeval timeout{TIMEOUT_SEC, 0};
    if (select(sockfd + 1, &readfds, nullptr, nullptr, &timeout) > 0) {
        char buffer[PACKET_SIZE];
        sockaddr_in response_addr;
        socklen_t addr_len = sizeof(response_addr);
        if (recvfrom(sockfd, buffer, sizeof(buffer), 0, (sockaddr*)&response_addr, &addr_len) > 0) {
            auto end_time = std::chrono::high_resolution_clock::now();
            double rtt = std::chrono::duration<double, std::milli>(end_time - start_time).count();
            std::cout << "Reply from " << target << ": time=" << rtt << " ms\n";
        }
    } else {
        std::cout << "Request timed out.\n";
    }
    close(sockfd);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <hostname/IP>\n";
        return EXIT_FAILURE;
    }
    send_ping(argv[1]);
    return EXIT_SUCCESS;
}