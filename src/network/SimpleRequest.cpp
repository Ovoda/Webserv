#include "SimpleRequest.hpp"
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

namespace network {
SimpleRequest::SimpleRequest( void ) {}

SimpleRequest::SimpleRequest(int fd) : _fd(fd) {
	if (_fd < 0) return;
	unblock();
	_data = extract();
}

// HMMMM: not sure about what to return when error occurs

int	SimpleRequest::extract(void) {
	char buffer[3000] = {0};
	int ret;

	if (_fd < 0) {
		_data = "";
		return -1;
	}
	if (( ret = recv(_fd, buffer, 3000, 0)) < 0) {
		// std::cout << strerror(errno) << std::endl;
		// std::cerr << "Error: RECV()" << std::endl;
		// _data = "";
		return ret;
	}
	_data = buffer;
	return (ret);
}

SimpleRequest::~SimpleRequest() {}

void 		SimpleRequest::set_fd(int fd) { _fd = fd; }
int 		SimpleRequest::get_fd(void) const { return _fd; }
std::string SimpleRequest::get_data(void) const { return _data; }

void	SimpleRequest::unblock(void) {
	if (_fd < 0) return;
	fcntl(_fd, F_SETFL, O_NONBLOCK);
}

/* Usefull for pipelining */
// void SimpleRequest::set_ipv4(char const *ipv4) { _ipv4 = ipv4; }
// char const *SimpleRequest::get_ipv4(void) const { return _ipv4.c_str(); }

}  // namespace network