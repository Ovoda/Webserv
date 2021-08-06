#include "Poll.hpp"

#define READFL 0x0002
#define WRITEFL 0x0001
#define IS_SOCKET false
#define IS_REQUEST true
#define CLOSED_CONN true
#define OPEN_CONN false

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
        add(it->get_id(), READFL, IS_SOCKET);
    }
    _nb_socket = _size;
}

void Poll::init_requests(void) {
    for (std::vector<network::Request>::iterator it = _requests.begin();
         it != _requests.end(); it++) {
        add(it->get_fd(), READFL, IS_REQUEST);
    }
    _nb_socket = _size;
}

void Poll::add(int fd, int flags, bool is_request) {
    int i;
    if (fd > 0) {
        // if (flags & READFL) std::cout << "READFL" << std::endl;
        // if (flags & WRITEFL) std::cout << "WRITEFL" << std::endl;
        if (flags & READFL) FD_SET(fd, &_read_set);
        if (is_request == true) {
            for (i = 0; i < _requests.size(); i++) {
                if (_requests[i].get_fd() == fd) break;
            }
            if (i >= _requests.size()) {
                network::Request tmp(fd);
                // std::cout << tmp.get_fd() << " added" << std::endl;
                _requests.push_back(tmp);
            }
            _size++;
        }
        set_max_fd();
    }
}

void Poll::remove(int fd) {
    if (fd > 0) {
        FD_CLR(fd, &_read_set);
    }
}

void Poll::set_max_fd(void) {
    if (_requests.size() != 0) {
        _max_fd = _requests.back().get_fd();
    } else {
        _max_fd = _sockets.back().get_id();
    }
}

int Poll::do_poll(void) {
    int ret;
	struct timeval timeout;

    FD_ZERO(&_read_set);
	bzero(&timeout, sizeof(timeout));
    init_sockets();
    init_requests();
    ret = select(_max_fd + 1, &_read_set, NULL, NULL, &timeout);
    if (ret < 0) {
        std::cerr << "Error: select()" << std::endl;
        return (-1);
    }
    return (0);
}

void Poll::check_sockets(void) {
    for (std::vector<network::Socket>::iterator it = _sockets.begin();
         it != _sockets.end(); it++) {
        int tmp = 0;
        // while (tmp != -1) {
        tmp = it->do_accept();
        if (tmp > 0) {
            fcntl(tmp, F_SETFL, O_NONBLOCK);
            if (tmp < 0) {
                perror("Accept: ");
                std::cerr << "Error on socket accept" << std::endl;
                break;
			}
            _max_fd = tmp;
            _nb_requests++;
        }
        // std::cout << _nb_requests << std::endl;
        // }
    }
}

void Poll::check_requests(void) {
    if (!_requests.empty()) {
        for (std::vector<network::Request>::iterator it = _requests.begin();
             it != _requests.end(); it++) {
            if (FD_ISSET(it->get_fd(), &_read_set)) {
				std::cout << "bonjour" << std::endl;
                char buffer[3000];
                int ret, rets = 0;
                ret = recv(it->get_fd(), buffer, 3000, MSG_DONTWAIT);
                if (ret <= 0) {
                    std::cout << "done" << std::endl;
                    it->set_connection(CLOSED_CONN);
                } else {
                    rets = send(it->get_fd(), "HTTP/1.1 OK 200\n\n0", 18, 0);
                }
            }
        }
        for (int i = _requests.size() - 1; i >= 0; i--) {
            if (_requests[i].get_connection() == CLOSED_CONN) {
                FD_CLR(_requests[i].get_fd(), &_read_set);
                close(_requests[i].get_fd());
                std::cout << "erasing " << _requests[i].get_fd()
                          << " connection" << std::endl;
                _requests.erase(_requests.begin() + i);
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