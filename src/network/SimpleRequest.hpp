#ifndef NETWORK_SIMPLE_REQUEST_HPP
#define NETWORK_SIMPLE_REQUEST_HPP

#include <netinet/in.h>
#include <time.h>

#include <string>

namespace network {
class SimpleRequest {
   public:
    SimpleRequest(void);
    ~SimpleRequest();

    void set_fd(int fd);
    void set_ipv4(char const ipv4[INET_ADDRSTRLEN]);
	void set_time( void );

    int 	get_fd(void) const;
    char const	*get_ipv4(void) const;

   private:
    int 		_fd;
    std::string _data;
    std::string	_ipv4;
	time_t		_time;
};
}  // namespace network

#endif