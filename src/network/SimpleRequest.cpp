#include "SimpleRequest.hpp"

namespace network {
SimpleRequest::SimpleRequest(void) {}
SimpleRequest::~SimpleRequest() {}

void SimpleRequest::set_fd(int fd) { _fd = fd; }
void SimpleRequest::set_ipv4(char const *ipv4) { _ipv4 = ipv4; }
int SimpleRequest::get_fd(void) const { return _fd; }
char const *SimpleRequest::get_ipv4(void) const { return _ipv4.c_str(); }

void set_time(void) { time(&_timev); }

}  // namespace network