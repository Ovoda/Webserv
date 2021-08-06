#ifndef NETWORK_POLL_HPP
#define NETWORK_POLL_HPP

#include <errno.h>
#include <poll.h>
#include <sys/select.h>  // select()
#include <sys/socket.h>
#include <unistd.h> // close()
#include <vector>
#include <iostream>
#include "Socket.hpp"

namespace network
{
	class Poll
	{
	public:
		Poll(void);
		Poll(int timeout, int size);
		Poll(std::vector<network::Socket> s);

		~Poll();

		int get_size(void) const;
		int get_nb_socket(void) const;
		struct pollfd *get_fds(void) const;
		int get_timeout(void) const;

		void	add(int fd, int flags);
		int		do_poll(void);
		void	check_sockets(std::vector<network::Socket> s);
		void	check_requests(void);
		void	resize_fds(void);
		void	run_servers(std::vector<network::Socket> s);
		void	send_response(void);

	private:
		struct pollfd *_fds;
		int _timeout;
		int _capacity;
		int _size;
		int _nb_socket;
		int _ready;
		int	_request_nb;
		std::vector<bool>	_checked_socket;
	};
}

std::ostream &operator<<(std::ostream &o, network::Poll const &p);

#endif