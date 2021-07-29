#ifndef NETWORK_USER_HPP
#define NETWORK_USER_HPP

#include <netinet/in.h>
#include <vector>

#include "SimpleRequest.hpp"

namespace network {
class User {
   public:
	User(void);
	User(SimpleRequest const & request);
	~User();

	void		add(SimpleRequest const & request);
	char const	*get_ipv4(void) const ;
	void		requests_log( void );

   private:
	std::vector<network::SimpleRequest> _requests;
	std::string	    					_ipv4;
	int 								_fd;
};
}  // namespace network

#endif