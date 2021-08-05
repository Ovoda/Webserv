#include "Poll.hpp"

#define READFL 0x0002
#define WRITEFL 0x0001

namespace network {
/*
 * Constructors and destructor
 */
Poll::Poll(std::vector<network::Socket> s) : _size(0), _sockets(s) {}
Poll::~Poll() {}

/*
 * Getters and Setters
 */
int Poll::get_size(void) const { return _size; }
std::vector<network::Request> &Poll::get_requests() { return _requests; }

int Poll::get_nb_socket(void) const { return _nb_socket; }

/*
 * Member functions
 */

void Poll::init_sockets(void) {
    for (std::vector<network::Socket>::iterator it = _sockets.begin();
         it != _sockets.end(); it++) {
        add(it->get_id(), READFL);
    }
    _nb_socket = _size;
}

void Poll::init_requests(void) {
    for (std::vector<network::Request>::iterator it = _requests.begin();
         it != _requests.end(); it++) {
        add(it->get_fd(), READFL | WRITEFL);
    }
    _nb_socket = _size;
}

void Poll::add(int fd, int flags) {
    int i;
    if (fd > 0) {
        // if (flags & READFL) std::cout << "READFL" << std::endl;
        // if (flags & WRITEFL) std::cout << "WRITEFL" << std::endl;
        if (flags & READFL) FD_SET(fd, &_read_set);
        if (flags & WRITEFL) FD_SET(fd, &_write_set);
        for (i = 0; i < _requests.size(); i++) {
            if (_requests[i].get_fd() == fd) break;
        }
        if (i < _requests.size()) {
            network::Request tmp(fd);
            std::cout << tmp.get_fd() << std::endl;
            _requests.push_back(tmp);
        }
        std::vector<network::Request>::iterator it;
        it = std::max_element(_requests.begin(), _requests.end());
        _max_fd = it->get_fd();
        _size++;
        // std::cout << fd << " added, max_fd : " << _max_fd << std::endl;
    }
}

void Poll::remove(int fd, int flags) {
    if (fd > 0) {
        if (flags & READFL) FD_CLR(fd, &_read_set);
        if (flags & WRITEFL) FD_CLR(fd, &_write_set);
        std::vector<network::Request>::iterator it;
        it = std::find(_requests.begin(), _requests.end(), fd);
        _requests.erase(it);
        it = std::max_element(_requests.begin(), _requests.end());
        _max_fd = it->get_fd();
        _size--;
    }
}

int Poll::do_poll(void) {
    int ret;

    FD_ZERO(&_read_set);
    FD_ZERO(&_write_set);
    init_sockets();
    init_requests();
    ret = select(_max_fd + 1, &_read_set, NULL, NULL, NULL);
    if (ret < 0) {
        std::cerr << "Error: select()" << std::endl;
        return (-1);
    }
    return (0);
}

void Poll::check_sockets(void) {
    for (std::vector<network::Socket>::iterator it = _sockets.begin();
         it != _sockets.end(); it++) {
        if (FD_ISSET(it->get_id(), &_read_set)) {
            int tmp = 0;
            // while (tmp != -1) {
            tmp = it->do_accept();
            if (tmp < 0) {
                perror("Accept: ");
                std::cerr << "Error on socket accept" << std::endl;
                break;
            }
            add(tmp, READFL | WRITEFL);
            _max_fd = tmp;
            _nb_requests++;
            std::cout << _nb_requests << std::endl;
            // }
        }
    }
}

void Poll::check_requests(void) {
    if (_ready > 0) {
        for (std::vector<network::Request>::iterator it = _requests.begin();
             it != _requests.end(); it++) {
            // put this in a thread maybe ?
            if (FD_ISSET(it->get_fd(), &_read_set)) {
                std::cout << it->get_fd() << " is available for reading"
                          << std::endl;
                //     char buffer[3000];
                //     std::cout << "sending" << std::endl;
                //     int ret = 0;
                //     ret = recv(it->get_fd(), buffer, 3000, MSG_DONTWAIT);
                //     std::cout << ret << std::endl;
                //     if (ret == 0) {
                //         std::cout << "Connection closed by client" <<
                //         std::endl; close(it->get_fd());
                //     }
                //     if (ret < 0) {
                //         std::cerr << "Error while reading on socket" <<
                //         std::endl; close(it->get_fd()); break;
                //     }
                // }
                // //                    _buffers[i] += buffer;
                // // Result<Request> res =
                // // handler.update(full_request.c_str(),
                // // full_request.length());
                // // brian
                // // refresh return events
                // if (FD_ISSET(it->get_fd(), &_write_set)) {
                //     std::cout << "sending" << std::endl;

                //     int rets = send(it->get_fd(), "HTTP/1.1 OK 200\n\nok",
                //     19, 0); if (rets < 0) std::cout << "error sending" <<
                //     std::endl;

                //     // check if message is sent fully and if so close()
                //     if (rets >= 19) {
                //         close(it->get_fd());
                //     }
                // } else {
                //     std::cout << "Error: not ready yet " << std::endl;
            }
        }
    }
}

void Poll::run_servers(std::vector<network::Socket> s) {
    for (;;) {
        do_poll();
        check_sockets();
        check_requests();
    }
}
}  // namespace network

std::ostream &operator<<(std::ostream &o, network::Poll &p) {
    o << "Poll : " << std::endl;
    std::cout << "Socket fds:" << std::endl;
    for (int i = 0; i < p.get_size(); i++) {
        if (i == p.get_nb_socket()) o << "Accepted fds :" << std::endl;
        o << i << ": " << p.get_requests()[i].get_fd() << std::endl;
    }
    return (o);
}