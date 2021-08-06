#include "Request.hpp"

namespace network {
Request::Request(int fd) : _fd(fd), _connection_is_closed(false), _status(0) {}
Request::Request(Request const &src) { *this = src; }
Request::~Request() {}
Request &Request::operator=(Request const &rhs) {
	if (this != &rhs) {
		_fd = rhs.get_fd();
		_buffer = rhs.get_buffer_copy();
	}
	return (*this);
}

std::string &Request::get_buffer(void) { return _buffer; }
std::string Request::get_buffer_copy(void) const { return _buffer; }

int 	Request::get_status( void ) const { return _status; }
void    Request::set_status( int status) { _status = status; }

int Request::get_fd(void) const { return _fd; }

void	Request::set_connection(bool value) { _connection_is_closed = value; }
bool	Request::get_connection( void ) const { return _connection_is_closed; }


bool Request::operator<(Request const &rhs) const {
	return (_fd < rhs.get_fd());
}

bool Request::operator>(Request const &rhs) const {
	return (_fd > rhs.get_fd());
}

bool Request::operator==(int rhs) const { return (_fd == rhs); }

std::ostream &operator<<(std::ostream &o, network::Request &r) {
	o << "Request " << r.get_fd() << ": " << r.get_buffer() << std::endl;
	return (o);
}
}  // namespace network