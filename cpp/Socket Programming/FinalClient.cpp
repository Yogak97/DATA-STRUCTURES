#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <thread>
#define TCP_PORT 1234
#define UDP_MULTICAST_GROUP "225.0.0.37"
#define UDP_MULTICAST_PORT 5678
#define BUFFER_SIZE 1024

void receiveMulticastMsgUDP(int udp_sock)
{
    // std::cout << "Thread Launched" << std::endl;
    struct sockaddr_in sender_addr;
    socklen_t sender_addr_len = sizeof(sender_addr);
    char buffer[BUFFER_SIZE] = {0};
    while (true)
    {
        int bytes_received = recvfrom(udp_sock, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&sender_addr, &sender_addr_len);
        if (bytes_received == -1)
        {
            std::cerr << "Failed to receive multicast message" << std::endl;
            // return -1;
            break;
        }

        std::cout << "\nReceived multicast message from " << inet_ntoa(sender_addr.sin_addr) << ": " << buffer << std::endl;
        memset(buffer, 0, BUFFER_SIZE);
    }

    close(udp_sock);
}

int main(int argc, char const *argv[])
{
    // Create a tcp socket

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        std::cerr << "Failed to create socket." << std::endl;
        return 1;
    }

    // Configure the server address and port for tcp
    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(TCP_PORT);
    if (inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr) <= 0)
    {
        std::cerr << "Invalid address." << std::endl;
        return 1;
    }

    // Create UDP socket
    int udp_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (udp_sock == -1)
    {
        std::cerr << "Failed to create UDP socket" << std::endl;
        return -1;
    }

    // allow multiple sockets to use the same PORT number
    //
    u_int yes = 1;
    if (setsockopt(udp_sock, SOL_SOCKET, SO_REUSEADDR, (char *)&yes, sizeof(yes)) < 0)
    {
        perror("Reusing ADDR failed");
        return 1;
    }

    // Join multicast group on UDP socket
    struct ip_mreq mreq;
    mreq.imr_multiaddr.s_addr = inet_addr(UDP_MULTICAST_GROUP);
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);
    if (setsockopt(udp_sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&mreq, sizeof(mreq)) == -1)
    {
        std::cerr << "Failed to join multicast group" << std::endl;
        return -1;
    }

    // Bind UDP socket to port
    struct sockaddr_in udp_addr;
    udp_addr.sin_family = AF_INET;
    udp_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    udp_addr.sin_port = htons(UDP_MULTICAST_PORT);
    if (bind(udp_sock, (struct sockaddr *)&udp_addr, sizeof(udp_addr)) == -1)
    {
        std::cerr << "Failed to bind UDP socket" << std::endl;
        return -1;
    }

    // Connect to the server
    if (connect(sock, (sockaddr *)&server_address, sizeof(server_address)) == -1)
    {
        std::cerr << "Failed to connect to server." << std::endl;
        return 1;
    }

    std::thread t1(receiveMulticastMsgUDP, udp_sock);
    t1.detach();

    // Send a message to the tcp server

    while (true)
    {
        char buffer[BUFFER_SIZE] = {0};
        // std::this_thread::sleep_for(std::chrono::seconds(1));
        // sleep(1);
        // std::cout << "client " << sock << ":";
        std::cin.getline(buffer, BUFFER_SIZE);
        if (send(sock, buffer, strlen(buffer), 0) == -1)
        {
            std::cerr << "Failed to send message." << std::endl;
            return 1;
        }
    }
    // // Receive a response from the server
    // char buffer[1024] = {0};
    // if (recv(sock, buffer, 1024, 0) == -1) {
    //     std::cerr << "Failed to receive message." << std::endl;
    //     return 1;
    // }
    // Receive multicast messages on UDP socket

    // struct sockaddr_in sender_addr;
    // socklen_t sender_addr_len = sizeof(sender_addr);

    // while (true)
    // {
    //     int bytes_received = recvfrom(udp_sock, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&sender_addr, &sender_addr_len);
    //     if (bytes_received == -1)
    //     {
    //         std::cerr << "Failed to receive multicast message" << std::endl;
    //         return -1;
    //     }

    //     std::cout << "Received multicast message from " << inet_ntoa(sender_addr.sin_addr) << ": " << buffer << std::endl;
    // }

    // Close the socket
    // close(udp_sock);
    close(sock);

    return 0;
}
