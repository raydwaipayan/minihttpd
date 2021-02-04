#include <sys/types.h>
#include <sys/socket.h>

/**
 * readline() - Read a line from the socket
 * @socket: socket to read data
 * @buf: character buffer to write bytes into
 * @size: maximim size of buffer
 * 
 * Return: number of bytes read
 */
int readline(int socket, char *buf, size_t size);