#include "Poll.hpp"

namespace network
{
	/*
	 * Constructors and destructor
	 */
	Poll::Poll(void) : _timeout(10000), _capacity(0), _size(0), _nb_socket(0)
	{
		_fds = new struct pollfd[0];
	}
	Poll::Poll(int timeout, int size) : _timeout(timeout), _capacity(size), _size(size)
	{
		_fds = new struct pollfd[size];
	}
	Poll::Poll(int timeout, std::vector<network::Socket> s) : _timeout(timeout)
	{
		_fds = new struct pollfd[s.size()];
		_size = 0;
		for (std::vector<network::Socket>::iterator it = s.begin(); it != s.end(); it++)
		{
			_fds[_size].fd = it->get_id();
			_fds[_size].events = POLLIN;
			_size++;
		}
		_nb_socket = _size;
		_capacity = _size;
	}
	Poll::~Poll()
	{
		delete[] _fds;
	}

	/*
	 * Getters and Setters
	 */
	int Poll::get_size(void) const { return _size; }
	struct pollfd *Poll::get_fds(void) const { return _fds; }
	int Poll::get_nb_socket(void) const { return _nb_socket; }
	int Poll::get_timeout(void) const { return _timeout; }

	/*
	 * Member functions
	 */

	void Poll::add(int fd, int flags)
	{
		if (_size == _capacity)
		{
			if (_capacity == 0)
				_capacity = 1;
			struct pollfd *tmp = new struct pollfd[_capacity * 2];
			for (int i = 0; i < _size; i++)
			{
				tmp[i] = _fds[i];
			}
			_fds = tmp;
		}
		if (fd > 0)
		{
			_fds[_size].fd = fd;
			_fds[_size].events = flags;
			_size++;
		}
	}

	int Poll::do_poll(void)
	{
		_ready = poll(_fds, _size, _timeout);
		return (_ready);
	}

	void Poll::check_sockets(std::vector<network::Socket> s)
	{
		if (_ready > 0)
		{
			for (int j = 0; j < _nb_socket; j++)
			{
				if (_fds[j].revents & POLLIN)
				{
					int tmp = 0;
					while (tmp != -1)
					{
						tmp = s[j].do_accept();
						if (tmp < 0)
						{
							if (errno != EWOULDBLOCK)
							{
								std::cerr << "Error on socket accept" << std::endl;
							}
							break;
						}
						std::cout << "Event on socket" << std::endl;
						// std::cout << _ready << std::endl;
						add(tmp, POLLIN | POLLOUT);
						// std::cout << *this << std::endl;
					}
				}
			}
		}
	}

	void Poll::check_requests(void)
	{
		if (_ready > 0)
		{
			for (int i = _nb_socket; i < _size; i++)
			{
				if (_fds[i].revents & POLLIN)
				{
					char buffer[3000];
					int ret = recv(_fds[i].fd, buffer, 3000, 0);
					if (ret == 0)
					{
						close(_fds[i].fd);
						_fds[i].fd = -1;
					}
					if (ret < 0)
					{
						std::cerr << "Error while reading on socket" << std::endl;
						close(_fds[i].fd);
						_fds[i].fd = -1;
					}
					// std::cout << buffer << std::endl;
				}
				else if (_fds[i].revents & POLLOUT)
				{
					int rets = send(_fds[i].fd, "OK!", 3, 0);
					if (rets < 0)
						std::cout << "error sending" << std::endl;
					// check if message is sent fully and if so close()
					if (rets >= 3)
					{
						usleep(100);
						close(_fds[i].fd);
						_fds[i].fd = -1;
					}
				}
			}
			resize_fds();
		}
	}

	void Poll::resize_fds(void)
	{
		for (int i = _nb_socket; i < _size; i++)
		{
			if (_fds[i].fd == -1)
			{
				for (int j = i; j < _size; j++)
				{
					_fds[j].fd = _fds[j + 1].fd;
				}
				_size--;
			}
		}
	}

	void Poll::run_servers(std::vector<network::Socket> s) {
		for (;;) {
			do_poll();
			check_sockets(s);
			check_requests();
		}
	}
}

std::ostream &operator<<(std::ostream &o, network::Poll const &p)
{
	o << "Poll : " << std::endl;
	std::cout << "Socket fds:" << std::endl;
	for (int i = 0; i < p.get_size(); i++)
	{
		if (i == p.get_nb_socket())
			o << "Accepted fds :" << std::endl;
		o << i << ": " << p.get_fds()[i].fd << std::endl;
	}
	return (o);
}