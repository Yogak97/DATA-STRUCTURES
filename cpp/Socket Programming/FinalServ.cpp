/*
Author : ykanhed
server program
*/

#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <thread>

#define MAX_CLIENTS 10
#define TCP_PORT 1234
#define UDP_PORT 5678
#define MULTICAST_ADDR "225.0.0.37"
#define BUFFER_SIZE 1024

// function to handle a new connection from client
void handle_client(int TCP_CLIENT_SOCKET, struct sockaddr_in client_addr)
{
    std::cout << "New connection, sock fd is " << TCP_CLIENT_SOCKET << " ip/port is : " << inet_ntoa(client_addr.sin_addr) << "/" << ntohs(client_addr.sin_port) << std::endl;

    char buffer[1024] = {0};
    // char ACK[] = "ACK";
    int receivedBytes, sentBytes;

    // multicast message to all connected clients
    struct sockaddr_in multicast_addr;
    memset(&multicast_addr, 0, sizeof(multicast_addr));
    multicast_addr.sin_family = AF_INET;
    multicast_addr.sin_addr.s_addr = inet_addr(MULTICAST_ADDR);
    multicast_addr.sin_port = htons(UDP_PORT);

    int UDP_SOCK;
    if ((UDP_SOCK = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        std::cout << "ERROR: Udp socket creation failed " << std::endl;
    }

    while (true)
    {

        /*sentBytes = send(TCP_CLIENT_SOCKET, ACK, sizeof(ACK), 0);
        if (sentBytes == 0)
        {
            std::cout << "Client disconnected, socket fd is " << TCP_CLIENT_SOCKET << " ip/port : " << inet_ntoa(client_addr.sin_addr) << "/" << ntohs(client_addr.sin_port) << std::endl;
            break;
        }
        else if (sentBytes < 0)
        {
            std::cout << "ERROR sending" << std::endl;
            break;
        } */

        // receive msgs from clients
        receivedBytes = read(TCP_CLIENT_SOCKET, buffer, BUFFER_SIZE);
        if (receivedBytes == 0)
        {
            std::cout << "Client disconnected, socket fd is " << TCP_CLIENT_SOCKET << " ip/port : " << inet_ntoa(client_addr.sin_addr) << "/" << ntohs(client_addr.sin_port) << std::endl;
            break;
        }
        else if (receivedBytes < 0)
        {
            std::cout << "ERROR reading" << std::endl;
            break;
        }

        std::cout << "Msg received from " << inet_ntoa(client_addr.sin_addr) << "/" << ntohs(client_addr.sin_port) << ":" << buffer << std::endl;

        // semd udp multicast msg to the clients
        sentBytes = sendto(UDP_SOCK, (const char *)buffer, strlen(buffer), 0, (const struct sockaddr *)&multicast_addr, sizeof(multicast_addr));
        if (sentBytes < 0)
        {
            std::cout << "ERROR: sending udp multicast msg " << std::endl;
            break;
        }
        memset(buffer, 0, sizeof(buffer));
    }

    close(UDP_SOCK);
    close(TCP_CLIENT_SOCKET);
}

int main(int argc, char *argv[])
{
    int TCP_LISTEN_SOCK, TCP_CLIENT_SOCK;
    TCP_LISTEN_SOCK = socket(AF_INET, SOCK_STREAM, 0);
    if (TCP_LISTEN_SOCK == -1)
    {
        std::cout << "ERROR : Failed to create tcp socket." << std::endl;
        return -1;
    }

    int opt = 1;
    if (setsockopt(TCP_LISTEN_SOCK, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        std::cout << "ERROR : setsockopt" << std::endl;
        return -1;
    }

    struct sockaddr_in server_address, client_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(TCP_PORT);

    if (bind(TCP_LISTEN_SOCK, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        std::cout << "ERROR : bind() failed" << std::endl;
        return -1;
    }

    if (listen(TCP_LISTEN_SOCK, MAX_CLIENTS) < 0)
    {
        std::cout << "ERROR : listen() failed" << std::endl;
        return -1;
    }

    size_t client_address_length = sizeof(client_address);
    std::cout << "Server is initiated. Listening for clients! " << std::endl;
    while (true)
    {
        if ((TCP_CLIENT_SOCK = accept(TCP_LISTEN_SOCK, (struct sockaddr *)&client_address, (socklen_t *)&client_address_length)) < 0)
        {
            std::cout << "ERROR : accept() failed" << std::endl;
            return -1;
        }

        // launch new thread for every client connection
        std::thread thread_handle_new_client(handle_client, TCP_CLIENT_SOCK, client_address);
        thread_handle_new_client.detach();
    }

    return true;
}