#ifndef NETWORK_SOCKET_HPP
#define NETWORK_SOCKET_HPP

#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

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
	int				do_listen(void) const;
	int				do_bind(void);
	SimpleRequest	do_accept(void) const;

   private:
	int _id;
	int _port;
	int _type;
	int _domain;
	int _addrlen;
	int _protocol;
	struct sockaddr_in _address;
};

}  // namespace network

std::ostream &operator<<(std::ostream &o, network::Socket const &s);
#endif