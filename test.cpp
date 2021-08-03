#include <poll.h>
#include <sys/socket.h>
#include <unistd.h>

#include "src/network/SimpleRequest.hpp"
#include "src/network/Socket.hpp"
#include <errno.h>
#include <stdio.h>

int main(void) {
    network::Socket s(18000);

    struct pollfd fds[200];
    int timeout = 5000;
    int ret, current_size, nb_fd;
    network::SimpleRequest tmp(s.do_accept());

    fds[0].fd = tmp.get_fd();
    fds[0].events = POLLOUT | POLLIN;
    current_size = nb_fd = 1;

    for (;;) {
        ret = poll(fds, current_size, timeout);
        if (ret < 0) {
            std::cerr << "Error: Poll() failed" << std::endl;
            break;
        }
        if (ret == 0) {
            std::cerr << "Timeout" << std::endl;
            break;
        }
        // std::cout << "POLLOUT" << std::endl;
        if (fds[0].revents & POLLIN) {
            char buffer[3000];
            ret = read(fds[0].fd, buffer, 3000);
            std::cout << buffer << std::endl;
        }
        if (fds[0].revents & POLLOUT) {
            send(fds[0].fd, "bonjour", 7, 0);
            int errorCode;
            uint len = sizeof(errorCode);
            int result =
                getsockopt(fds[0].fd, SOL_SOCKET, SO_ERROR, &errorCode, &len);
            if (errorCode == 0)
                close(fds[0].fd);
            // fds[0].events = POLLIN;
        }
        current_size = nb_fd;
    }

    return (0);
}