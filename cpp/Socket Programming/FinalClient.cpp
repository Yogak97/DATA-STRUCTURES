/*
Author : ykanhed
client program
*/

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

void receiveMulticastMsgUDP(int UDP_SOCKET)
{
    struct sockaddr_in sender_addr;
    socklen_t sender_addr_len = sizeof(sender_addr);
    char buffer[BUFFER_SIZE] = {0};
    while (true)
    {
        int bytes_received = recvfrom(UDP_SOCKET, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&sender_addr, &sender_addr_len);
        if (bytes_received == -1)
        {
            std::cout << "EROOR : Failed to receive multicast message" << std::endl;
            break;
        }
        std::cout << "Received multicast message from " << inet_ntoa(sender_addr.sin_addr) << ": " << buffer << std::endl;
        memset(buffer, 0, BUFFER_SIZE);
    }
    close(UDP_SOCKET);
}

int main(int argc, char const *argv[])
{
    // Create UDP socket
    int UDP_SOCK = socket(AF_INET, SOCK_DGRAM, 0);
    if (UDP_SOCK == -1)
    {
        std::cout << "Failed to create UDP socket" << std::endl;
        return -1;
    }

    // allow multiple sockets to use the same PORT number
    u_int yes = 1;
    if (setsockopt(UDP_SOCK, SOL_SOCKET, SO_REUSEADDR, (char *)&yes, sizeof(yes)) < 0)
    {
        std::cout << "Reusing ADDR failed" << std::endl;
        return -1;
    }

    // Join multicast group on UDP socket
    struct ip_mreq mreq;
    mreq.imr_multiaddr.s_addr = inet_addr(UDP_MULTICAST_GROUP);
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);
    if (setsockopt(UDP_SOCK, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&mreq, sizeof(mreq)) == -1)
    {
        std::cout << "Failed to join multicast group" << std::endl;
        return -1;
    }

    // Bind UDP socket to port
    struct sockaddr_in udp_addr;
    udp_addr.sin_family = AF_INET;
    udp_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    udp_addr.sin_port = htons(UDP_MULTICAST_PORT);
    if (bind(UDP_SOCK, (struct sockaddr *)&udp_addr, sizeof(udp_addr)) == -1)
    {
        std::cout << "Failed to bind UDP socket" << std::endl;
        return -1;
    }

    // launch a thread to received udp multicast msgs
    std::thread t1(receiveMulticastMsgUDP, UDP_SOCK);
    t1.detach();

    while (true)
    {
        // Create a tcp socket
        int TCP_SOCK = socket(AF_INET, SOCK_STREAM, 0);
        if (TCP_SOCK == -1)
        {
            std::cout << "ERROR : Failed to create tcp socket." << std::endl;
            return 1;
        }

        // Configure the server address and port for tcp
        sockaddr_in server_address;
        server_address.sin_family = AF_INET;
        server_address.sin_port = htons(TCP_PORT);
        if (inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr) <= 0)
        {
            std::cout << "Invalid address." << std::endl;
            return -1;
        }

        // Connect to the server
        if (connect(TCP_SOCK, (sockaddr *)&server_address, sizeof(server_address)) == -1)
        {
            std::cout << "ERROR : Trying to connect to the server... " << std::endl;
            close(TCP_SOCK);
            sleep(1);
            continue;
        }

        std::cout << "Success : Connected to the server. " << std::endl;

        // Send a message to the tcp server
        char buffer[BUFFER_SIZE] = {0};
        while (true)
        {

            /*ssize_t receivedBytes = read(TCP_SOCK, buffer, sizeof(buffer) - 1);
            if (receivedBytes < 0)
            {
                std::cout << "ERROR : reading failed from server" << std::endl;
                break;
            }
            else if (receivedBytes == 0)
            {
                std::cout << "ERROR : Connection closed by server" << std::endl;
                break;
            } */
            memset(buffer, 0, sizeof(buffer));
            std::cin.getline(buffer, BUFFER_SIZE);
            ssize_t sentBytes = send(TCP_SOCK, buffer, strlen(buffer), 0);
            if (sentBytes == -1)
            {
                std::cout << "ERROR : Failed to send message." << std::endl;
                break;
            }
            else if (sentBytes == 0)
            {
                std::cout << "ERROR : Connection closed by server" << std::endl;
                break;
            }
        }

        close(TCP_SOCK);
        sleep(3);
    }

    return 0;
}
