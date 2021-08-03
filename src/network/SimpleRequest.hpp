#ifndef NETWORK_SIMPLE_REQUEST_HPP
#define NETWORK_SIMPLE_REQUEST_HPP

#include <netinet/in.h>
#include <fcntl.h>
#include <time.h>
#include <iostream>
#include <string>

namespace network {
class SimpleRequest {
   public:
	SimpleRequest( int );
	SimpleRequest( void );
	~SimpleRequest();

	void 	set_fd(int fd);
	int 	get_fd(void) const;
	std::string	get_data(void) const;
	void	unblock( void );
	int	extract( void );

	/* Usefull for pipelining */
	//	void set_ipv4(char const ipv4[INET_ADDRSTRLEN]);
	//	char const	*get_ipv4(void) const;


   private:
	int 		_fd;
	std::string _data;
	// std::string	_ipv4;
};
}  // namespace network

#endif