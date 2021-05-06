#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>

#define SRVSTR "Server: minihttpd1.0\r\n"

/**
 * http_ok() - Write http headers to socket
 *             for a successful response
 */
void http_ok(int client, char *mimetype, char *filename);

/*
 * http_not_found() - Write 404 not found error
 */
void http_not_found(int client);

/**
 * http_bad_request() - Write 400 bad request
 */
void http_bad_request(int client);

/**
 * http_server_error() - Write 500 headers in case of
 *                       an internal server error
 */
void http_server_error(int client);

/**
 * server_file() - Write file contents to response body
 */
void serve_file(int client, char *mimetype, char *filename);

/**
 * http_unimplemented() - Write http headers for unimplmented
 *                        methods
 */
void http_unimplemented(int client);