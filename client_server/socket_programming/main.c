#include "socket_functions.h"
#include <string.h>
#include <pthread.h>

#define BUFFER_SIZE 1024 //TODO
#define NUM_THREADS 10  //replace with command line arg
                       //delete if not needed

typedef struct listener_thread_arg {
    int tcp_socket,
        client_id;
    //TODO whatever needed
}listener_thread_arg;

typedef struct client{
    //TODO
    char* name;
    int client_socket;
}client_t;

void case_one() {
    //TODO 7 return type and arguments
}

void case_two() {
    //TODO 8 return type and arguments
}

void handle_message(client_t* client, char* message) {
    //TODO 9 return type and arguments
    //usually something like this:
    //char buffer[BUFFER_SIZE];
    //snprintf(buffer, sizeof(buffer), "%s: %s", client->name, message);
    //write(STDOUT_FILENO, buffer, strlen(buffer));
}

void* handle_client(void* arg) {
    client_t    *client = (client_t*)arg;
    char        buffer[BUFFER_SIZE], message[BUFFER_SIZE * 2];
    ssize_t     bytes_received;

    //INIT BUFFERS
    memset(message, 0, sizeof(message));
    memset(buffer, 0, sizeof(buffer));

    //RECEIVE MESSAGES
    while(1) {
        if((bytes_received = recv(client->client_socket, buffer, sizeof(buffer), 0)) == -1) {
            perror("recv");
            break;
        }
        buffer[bytes_received] = '\0';

        //TODO 6 check input
        if(strcmp(buffer, "/shutdown")) case_one(); //TODO
        else if(strcmp(buffer, "/kick")) case_two(); //TODO
        else handle_message(client, buffer); //TODO

    }
    //TODO 10 cleanup;
    return NULL;
}

void* accept_connections(void* arg) {
    listener_thread_arg* listenerThreadArg = (listener_thread_arg*)arg;

    struct sockaddr_in      client_address;
    socklen_t               client_socklen;
    pthread_t               client_thread;
    int                     client_socket;

    client_socklen = sizeof(client_address);
    while(1) {
        accept_safe(listenerThreadArg->tcp_socket, (struct sockaddr*)&client_address, &client_socklen);
        client_t* client = malloc(sizeof(client_t));
        if(client == NULL) {
            perror("malloc");
            exit(EXIT_FAILURE);
        }

        //TODO 5 initialize client and handle_client. Don't forget to free!
    }
}

int main(int argc, char* argv[]) {
    if(argc != 2) { //TODO change number
        fprintf(stderr, "Usage: %s <whatever>\n", argv[0]); //TODO suitable error message
        return EXIT_FAILURE;
    }

    int tcp_socket, opt;
    socklen_t tcp_socklen;
    in_port_t port;         //TODO 1 parse port
    struct sockaddr_in tcp_address;

    tcp_socket = create_socket();
    set_reuse_address(tcp_socket);
    init_server_sockaddr(&tcp_address, port);
    tcp_socklen = sizeof(tcp_address);
    bind_safe(tcp_socket, (struct sockaddr*)&tcp_address, tcp_socklen);
    listen_safe(tcp_socket, SOMAXCONN);

    //printf("Listening on port %d\n", port);

    //TODO 2 initialize datastructures as needed
    //TODO 3 accept connections; Either use threads or do it sequentially
    pthread_t threads[NUM_THREADS];
    listener_thread_arg listenerThreadArg;
    for(int i = 0; i < NUM_THREADS; i++) {
        //TODO 4 init listenerThreadArg
        pthread_create(&threads[i], NULL, accept_connections, (void*)&listenerThreadArg);
    }
    close(tcp_socket);
    return EXIT_SUCCESS;
}