#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <iostream>

int main(void) {
    int s;
    struct sockaddr_in  a;
    socklen_t           addr_len =  sizeof(a);

    a.sin_family = AF_INET;
    a.sin_addr.s_addr = INADDR_ANY;
    a.sin_port = htons(18003);

    s = socket(AF_INET, SOCK_STREAM, 0);
    bind(s, (struct sockaddr *)&a, addr_len);
    listen(s, 10);

    fcntl(s, F_SETFL, O_NONBLOCK);
    while (1) {
        int i = accept(s, (struct sockaddr *)&a, &addr_len);
    }
}