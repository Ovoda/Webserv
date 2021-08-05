#include "Request.hpp"

namespace network {
Request::Request(int fd) : _fd(fd) {}
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
int Request::get_fd(void) const { return _fd; }

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