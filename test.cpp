#include <errno.h>
#include <poll.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>
#include <set>

#include "networking/ServerSocket.hpp"
#include "networking/Poll.hpp"
#include "networking/PollFd.hpp"

#include <map>
int main(void)
{

    // network::PollFd fds;
    
    // std::set<int>               ports;
    // ports.insert(18000);
    // ports.insert(18001);
    // ports.insert(18002);

    // std::vector<network::ServerSocket> sockets(ports.begin(), ports.end());

    // network::Poll p(sockets);
    // p.run_servers(sockets);

    // check if non blocking works well
    // s1.do_accept();

    return (0);
}