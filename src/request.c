#include <request.h>

int readline(int socket, char *buf, size_t size)
{
    char c = 0;
    size_t i = 0;
    int r = 0;

    while(i < size) {
        r = recv(socket, &c, 1, 0);
        if (r > 0 && c != '\n') {
            if (c == '\r') {
                r = recv(socket, &c, 1, MSG_PEEK);
                if (r > 0 && c == '\n') {
                    recv(socket, &c, 1, 0);
                    break;
                }
                c = '\r';
            }
            buf[i++] = c;
        } else {
            break;
        }
    }
    buf[i] = 0;
    return i;
}