#include <poll.h>
#include <sys/socket.h>
#include "src/network/Socket.hpp"

int main( void ) {
    network::Socket s(18000);

    struct pollfd   fds[200];
    int             timeout = 5000;
    int             ret, current_size, nb_fd;

    fds[0].fd = s.get_id();
    fds[0].events = POLLIN;
    current_size = nb_fd = 1;


    for (;;) {
        ret = poll(fds, current_size, timeout);
        if (ret < 0) {
            std::cerr << "Error: Poll() failed" << std::endl;
            break ;
        }
        if (ret == 0) {
            std::cerr << "Timeout" << std::endl;
            break ;
        }

        current_size = nb_fd;
        for (int i = 0; i < current_size; i++) {
            if (fds[i].revents == 0)
                continue;
        }
        
    }

    return (0);
}