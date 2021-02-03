/*
 * Mini http server in c
 * 
 * Create socket with socket() call
 * bind() this to an IP and port where it can
 * listen() for connections, then
 * accept() connection and send() or receive() data 
 * to/from connected sockets
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

/*
 * Socket Headers
 */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <arpa/inet.h>
#include <pthread.h>
#include <sys/wait.h>

/*
 * Helper Libraries
 */
#include "response.h"

#define PORT 8000

void accept_request(int client)
{
    serve_file(client, "test.html");
}

int main(int argc, char *argv[])
{
    int server_sock = -1;
    int client_sock = -1;

    struct sockaddr_in client;
    struct sockaddr_in server;
    int client_len=sizeof(client);

    pthread_t serve_thread;

    server_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (server_sock == -1) {
        perror("could not create socket");
        exit(1);
    }

    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(server_sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("coult not bind to socket");
        exit(1);
    }
    
    if (listen(server_sock, 5) < 0) {
        perror("could not listen on socket");
        exit(1);
    }

    printf("Mini httpd server running on port %d\n", PORT);

    while(1) {
        client_sock = accept(
            server_sock,
            (struct sockaddr *)&client,
            &client_len
        );

        if (client_sock == -1) {
            perror("Could not accept connections");
            exit(1);
        }

        if (pthread_create(&serve_thread, NULL, accept_request, client_sock) < 0) {
            perror("Error creating thread");
            exit(1);
        }
    }

    close(server_sock);
    return 0;
}