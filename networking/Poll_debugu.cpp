#include "Poll.hpp"

namespace network {
/*
 * Constructors and destructor
 */
Poll::Poll(void) : _timeout(10000), _capacity(0), _size(0), _nb_socket(0) {
    _fds = new struct pollfd[0];
}
Poll::Poll(int timeout, int size)
    : _timeout(timeout), _capacity(size), _size(size) {
    _fds = new struct pollfd[size];
}
Poll::Poll(int timeout, std::vector<network::Socket> s) : _timeout(timeout) {
    _fds = new struct pollfd[s.size()];
    _size = 0;
    for (std::vector<network::Socket>::iterator it = s.begin(); it != s.end();
         it++) {
        _fds[_size].fd = it->get_id();
        _fds[_size].events = POLLIN;
		_checked_socket.push_back(false);
        _size++;
    }
    _nb_socket = _size;
    _capacity = _size;
}
Poll::~Poll() { delete[] _fds; }

/*
 * Getters and Setters
 */
int Poll::get_size(void) const { return _size; }
struct pollfd *Poll::get_fds(void) const {
    return _fds;
}
int Poll::get_nb_socket(void) const { return _nb_socket; }
int Poll::get_timeout(void) const { return _timeout; }

/*
 * Member functions
 */

void Poll::add(int fd, int flags) {
	std::cout << "array of int : ";
    if (_size == _capacity) {
        if (_capacity == 0) _capacity = 1;
        struct pollfd *tmp = new struct pollfd[_capacity * 2];
        for (int i = 0; i < _size; i++) {
            tmp[i] = _fds[i];
			std::cout << tmp[i].fd << "[" << tmp[i].events << "], ";
        }
        _fds = tmp;
    }
    if (fd > 0) {
        _fds[_size].fd = fd;
        _fds[_size].events = flags;
			std::cout << _fds[_size].fd << "[" << _fds[_size].events << "]" << std::endl;
		_checked_socket.push_back(false);
        _size++;
    }
}

int Poll::do_poll(void) {
    _ready = poll(_fds, _size, _timeout);
	std::cout << _ready << " events" << std::endl;
    return (_ready);
}

void Poll::check_sockets(std::vector<network::Socket> s) {
    std::cout << "---- check sockets begin ----" << std::endl;
    if (_ready > 0) {
        for (int j = 0; j < _nb_socket; j++) {
            if (_fds[j].revents & POLLIN) {
                int tmp = 0;
                tmp = s[j].do_accept();
                std::cout << "new fd created : " << tmp << std::endl;
                add(tmp, POLLIN | POLLOUT);
                _ready--;
            }
        }
    }
    std::cout << "---- check sockets end ----" << std::endl;
}

void Poll::check_requests(void) {
    std::cout << "---- check requests begin ----" << std::endl;
    if (_ready > 0) {
        for (int i = _nb_socket; i < _size; i++) {
            if (_fds[i].revents & POLLIN) {
                std::cout << "working on fd : " << _fds[i].fd << std::endl;
                int new_fd = _fds[i].fd;
                char buffer[4096];
                int ret;
                ret = recv(_fds[i].fd, buffer, 4096, 0);
                if (ret <= 0) {
                    new_fd = -1;
                }
                _ready--;
				_checked_socket[i] = true;
            } else {
                std::cout << "Non polling work, needs : " << _fds[i].revents << std::endl;
            }
        }
    } else {
        std::cout << "nothing to be done" << std::endl;
    }

    std::cout << "---- check requests end ----" << std::endl;
}

void Poll::send_response(void) {
    std::cout << "---- send respond begin ----" << std::endl;
    for (int i = _nb_socket; i < _size; i++) {
        if (_fds[i].revents == POLLOUT &&_checked_socket[i] == true) {  // && response is ready)
            std::cout << "working on fd : " << _fds[i].fd << std::endl;
            int ret;
            ret = send(_fds[i].fd, "HTTP/1.1 OK 200\n\n500", 20, 0);
            if (ret >= 19) {
                close(_fds[i].fd);
                _fds[i].fd = -1;
            }
        }
    }
    resize_fds();
    std::cout << "---- send respond end ----" << std::endl;
}  // namespace network

void Poll::resize_fds(void) {
    for (int i = _nb_socket; i < _size; i++) {
        if (_fds[i].fd == -1) {
			_checked_socket.erase(_checked_socket.begin() + i);
            for (int j = i; j < _size; j++) {
                _fds[j].fd = _fds[j + 1].fd;
            }
            _size--;
        }
    }
}

void Poll::run_servers(std::vector<network::Socket> s) {
    for (;;) {
        do_poll();
        check_sockets(s);
        check_requests();
        send_response();
        std::cout << std::endl;
		 for (int i = 0; i < _size; i++) {
			std::cout << _fds[i].fd << "[" << _fds[i].events << " | " << std::boolalpha << _checked_socket[i] << "], ";
        }
		std::cout << std::endl;
    }
}
}  // namespace network

std::ostream &operator<<(std::ostream &o, network::Poll const &p) {
    o << "Poll : " << std::endl;
    std::cout << "Socket fds:" << std::endl;
    for (int i = 0; i < p.get_size(); i++) {
        if (i == p.get_nb_socket()) o << "Accepted fds :" << std::endl;
        o << i << ": " << p.get_fds()[i].fd << std::endl;
    }
    return (o);
}