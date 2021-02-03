#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>

#define SRVSTR "Server: minihttpd1.0\r\n"

/*
 * Write http headers to socket
 * for a successful response
 */
void http_ok(int client, char *filename);

/*
 * 404 not found error
 */
void http_not_found(int client);

/*
 * Write file contents to response body
 */
void serve_file(int client, char *filename);

/*
 * Write http headers for unimplmented
 * methods
 */
void http_unimplemented(int client);