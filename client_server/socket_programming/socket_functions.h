#ifndef SOCKET_FUNCTIONS_H
#define SOCKET_FUNCTIONS_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int create_socket() {
    int socket_fd = socket(PF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    return socket_fd;
}

void bind_safe(int socket, struct sockaddr* address, socklen_t socklen) {
    if (bind(socket, address, socklen) == -1) {
        perror("bind");
        close(socket);
        exit(EXIT_FAILURE);
    }
}

void listen_safe(int socket, int backlog) {
    if (listen(socket, backlog) == -1) {
        perror("listen");
        close(socket);
        exit(EXIT_FAILURE);
    }
}

int accept_safe(int socket, struct sockaddr* address, socklen_t* socklen) {
    int client_socket = accept(socket, address, socklen);
    if (client_socket == -1) {
        perror("accept");
        close(socket);
        exit(EXIT_FAILURE);
    }
    return client_socket;
}

void init_server_sockaddr(struct sockaddr_in* address, in_port_t port) {
    address->sin_addr.s_addr = htonl(INADDR_ANY);
    address->sin_family = AF_INET;
    address->sin_port = htons(port);
}

void set_reuse_address(int socket) {
    int opt = 1;
    setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
}

#endif