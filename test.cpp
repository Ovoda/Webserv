#include <errno.h>
#include <poll.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>
#include <set>

#include "networking/Socket.hpp"
#include "networking/Poll.hpp"

int main(void)
{
    std::set<int> ports;

    ports.insert(18000);
    ports.insert(18001);
    ports.insert(18002);


    std::vector<network::Socket>    sockets(ports.begin(), ports.end());

    network::Poll p(10000, sockets);
    std::cout << p << std::endl;
    p.run_servers(sockets);

    return (0);
}