#include <errno.h>
#include <poll.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

#include "src/network/SimpleRequest.hpp"
#include "src/network/Socket.hpp"

int main(void) {
    network::Socket s(18000);

    struct pollfd fds[200];
    int timeout = 5000;
    int ret, current_size, nb_fd;

    fds[0].fd = s.get_id();
    fds[0].events = POLLIN;
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

        current_size = nb_fd;
        for (int i = 0; i < current_size; i++) {
            if (fds[i].fd == s.get_id()) {
                // std::cout << "server" << std::endl;
                if (fds[i].revents != POLLIN) {
                    // std::cout << "Error: not pollin" << std::endl;
                    // break;
                } else {
                    network::SimpleRequest tmp(s.do_accept());
                    fcntl(tmp.get_fd(), F_SETFL, O_NONBLOCK);
                    fds[nb_fd].fd = tmp.get_fd();
                    fds[nb_fd].events = POLLOUT | POLLIN;
                    nb_fd++;
                    std::cout << "accept" << std::endl;
                }
            } else {
                // std::cout << "request" << std::endl;
                if (fds[i].revents & POLLIN) {
                    char buffer[3000];
                    ret = recv(fds[i].fd, buffer, 3000, 0);
                    if (ret < 0) {
                        break;
                    }
                    std::cout << "HELLO: " << buffer << std::endl;
                }
                //     std::cout << "sending" << std::endl;
                //     send(fds[i].fd, "bonjour", 7, 0);
                //     int errorCode;
                //     uint len = sizeof(errorCode);
                //     int result = getsockopt(fds[i].fd, SOL_SOCKET, SO_ERROR,
                //                             &errorCode, &len);
                //     if (errorCode == 0) {
                //         close(fds[i].fd);
                //         fds[i].fd = -1;
                //         for (int k = 0; k < nb_fd; k++) {
                //             if (fds[k].fd == -1) {
                //                 for (int j = k; j < nb_fd; j++) {
                //                     fds[j].fd = fds[j + 1].fd;
                //                 }
                //                 k--;
                //                 nb_fd--;
                //             }
                //         }
                //     }
                //     // fds[0].events = POLLIN;
                // }
            }
        }
        current_size = nb_fd;
    }

    return (0);
}