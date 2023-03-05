#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>
#define MAX_CLIENTS 10
#define PORT 1234
#define UDP_PORT 5678
#define MULTICAST_ADDR "225.0.0.37"

// function to handle client connection
void handle_client(int client_socket)
{
    char buffer[1024] = {0};
    while (true)
    {
        int valread = read(client_socket, buffer, 1024);
        if (valread == 0)
        {
            // client has disconnected
            printf("Client disconnected, socket fd is %d\n", client_socket);
            break;
        }
        printf("Message received from client (socket fd %d): %s\n", client_socket, buffer);

        // multicast message to all connected clients
        struct sockaddr_in multicast_addr;
        memset(&multicast_addr, 0, sizeof(multicast_addr));
        multicast_addr.sin_family = AF_INET;
        multicast_addr.sin_addr.s_addr = inet_addr(MULTICAST_ADDR);
        multicast_addr.sin_port = htons(UDP_PORT);
        int udp_fd;
        if ((udp_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
        {
            perror("socket creation failed");
            exit(EXIT_FAILURE);
        }
        int nbytes = sendto(udp_fd, (const char *)buffer, strlen(buffer), 0, (const struct sockaddr *)&multicast_addr, sizeof(multicast_addr));
        if (nbytes < 0)
        {
            perror("sendto");
            // return 1;
        }
        close(udp_fd);

        // send(client_socket, buffer, strlen(buffer), 0); tcp
        memset(buffer, 0, sizeof(buffer));
    }
    close(client_socket);
    std::cout << "Function finshed" << std::endl;
}

int main(int argc, char *argv[])
{
    // set up the server socket
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, MAX_CLIENTS) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    // accept incoming connections and handle data in separate threads
    while (true)
    {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        printf("New connection , socket fd is %d , ip is : %s , port : %d\n", new_socket, inet_ntoa(address.sin_addr), ntohs(address.sin_port));

        // create new thread to handle client connection
        std::thread t(handle_client, new_socket);
        t.detach(); // allow thread to run independently
    }

    return true;
}