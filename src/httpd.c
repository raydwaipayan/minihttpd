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
#include "request.h"

int PORT = 8000;
char path[1024];

/*
 * Hypertext Transfer Protocol 1.1 semantics
 * are taken from:
 * https://tools.ietf.org/html/rfc7231
 * 
 * accept_request(int) processes the request
 * from the client
 * 
 * A typical header format is:
 * GET /path/file.html HTTP/1.1
 * Host: www.host1.com:80
 * [blank line here]
 */
void accept_request(int client)
{
    char buf[1024];
    char url[255];
    char method[255];
    char filepath[255];
    char *query;

    int ex = 0;
    int n = 0;
    
    n = readline(client, buf, sizeof(buf));

    size_t i = 0, j = 0;
    while(i < n && buf[i] != ' ') {
        method[j++] = buf[i++];
    }
    method[j] = 0;
    j = 0;
    i++;

    if (strcmp(method, "GET") && strcmp(method, "POST")) {
        http_bad_request(client);
        close(client);
        return;
    }

    while(i < n && buf[i] != ' ') {
        url[j++] = buf[i++];
    }
    url[j] = 0;
    j = 0;

    if (!strcmp(method, "GET")) {
        query = url;

        while((*query) != '?' && (*query) != '\0') {
            query++;
        }
        if ((*query) == '?') {
            ex = 1;
            *query = '\0';
            query++;
        }
    }

    sprintf(filepath, "%s%s", path, url);
    if (filepath[strlen(filepath) - 1] == '/') {
        strcat(filepath, "index.html");
    }

    printf("%s: %s\n", method, filepath);
    if (!ex) {
        serve_file(client, filepath);
    }

    /*
     * o The proper way to close the client is to call
     *   shutdown to trigger a FIN
     * o After that call recv() and wait until it is anything
     *   other than EAGAIN, EWOULDBLOCK
     * o Close the client
     */
    shutdown(client, SHUT_WR);
    char c;
    do{
        recv(client, &c, 1, 0);
    } while (c == EAGAIN || c == EWOULDBLOCK);
    close(client);
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("Usage:\n./minihttpd [server_dir] [PORT]\n\n");
        exit(1);
    }
    strcpy(path, argv[1]);

    if (argc > 2) {
        PORT = atoi(argv[2]);
    }

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