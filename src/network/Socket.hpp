#ifndef NETWORK_SOCKET_HPP
#define NETWORK_SOCKET_HPP

#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <iostream>

#include "SimpleRequest.hpp"

namespace network {
class Socket {
   public:
    Socket(int port);
    Socket(void);
    Socket(Socket const &src);
    ~Socket();

    // Overload
    Socket &operator=(Socket const &rhs);

    // Getters and Setters
    int get_id(void) const;
    int get_port(void) const;
    int get_type(void) const;
    int get_domain(void) const;
    int get_protocol(void) const;
    int get_addr_len(void) const;
    struct sockaddr_in *get_addr(void) const;

    // Member functions
    int do_listen(void);
    int do_bind(void);
    int do_accept(void);
    bool is_good(void) const;
    int unblock(void);

   private:
    int _id;
    int _port;
    int _type;
    int _domain;
    int _addrlen;
    int _protocol;
    struct sockaddr_in _address;
    bool _is_good;
};

}  // namespace network

std::ostream &operator<<(std::ostream &o, network::Socket const &s);
#endif