#ifndef NETWORK_REQUEST_HPP
#define NETWORK_REQUEST_HPP

#include <iostream>
#include <string>
#include <vector>

namespace network {
class Request {
   public:
    Request(int fd);
    Request(Request const &src);
    ~Request();
    std::string &get_buffer(void);
    std::string get_buffer_copy(void) const;
    int get_fd(void) const;
    bool operator<(Request const &rhs) const;
    bool operator>(Request const &rhs) const;
    bool operator==(int rhs) const;
    Request &operator=(Request const &rhs);

   private:
    std::string _buffer;
    int _fd;
};
}  // namespace network

std::ostream &operator<<(std::ostream &o, network::Request const &r);

#endif