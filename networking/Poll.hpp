#ifndef NETWORK_POLL_HPP
#define NETWORK_POLL_HPP

#include <errno.h>
#include <poll.h>
#include <sys/select.h>  // select()
#include <sys/socket.h>
#include <unistd.h>  // close()

#include <algorithm>  // max_element()
#include <string>
#include <vector>

#include "Request.hpp"
#include "Socket.hpp"

namespace network {
class Poll {
   public:
    Poll(std::vector<network::Socket> s);

    ~Poll();

    int get_size(void) const;
    int get_nb_socket(void) const;
	void set_max_fd(void);
    std::vector<network::Request> &get_requests();

    void add(int fd, int flags, bool is_request);
    void init_sockets(void);
    void init_requests(void);
    void remove(int fd);
    int do_poll(void);
    void check_sockets();
    void check_requests(void);
    void run_servers(std::vector<network::Socket> s);

   private:
    int _size;
    int _nb_socket;
    int _ready;
    int _nb_requests;
    fd_set _read_set;
    fd_set _write_set;
    int _max_fd;
    std::vector<network::Request> _requests;
    std::vector<network::Socket> _sockets;
};
}  // namespace network

std::ostream &operator<<(std::ostream &o, network::Poll const &p);

#endif