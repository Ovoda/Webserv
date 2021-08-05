#ifndef NETWORK_POLL_HPP
#define NETWORK_POLL_HPP

#include <string>
#include <poll.h>
#include "Socket.hpp"
#include <vector>
#include <errno.h>
#include <sys/socket.h>
#include <unistd.h> // close()
#include <sys/select.h> // select()
#include "Request.hpp"
#include <algorithm> // max_element()

namespace network
{
	class Poll
	{
	public:
		Poll(std::vector<network::Socket> s);

		~Poll();

		int get_size(void) const;
		int get_nb_socket(void) const;
		std::vector<network::Request> &get_requests();

		void	add(int fd, int flags);
		void	init_sockets(void);
		void	init_requests(void);
		void	remove(int fd, int flags);
		int		do_poll(void);
		void	check_sockets();
		void	check_requests(void);
		void	run_servers(std::vector<network::Socket> s);

	private:
		int _size;
		int _nb_socket;
		int _ready;
		int _nb_requests;
		fd_set	_read_set;
		fd_set	_write_set;
		int		_max_fd;
		std::vector<network::Request>	_requests;
		std::vector<network::Socket>	_sockets;
	};
}

std::ostream &operator<<(std::ostream &o, network::Poll const &p);

#endif