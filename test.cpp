#include <errno.h>
#include <poll.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

#include "networking/Socket.hpp"
#include "networking/Poll.hpp"

int main(void)
{
    network::Socket s1(18000);
    network::Socket s2(18001);
    network::Socket s3(18002);
    std::vector<network::Socket> s;

    s.push_back(s1);
    s.push_back(s2);
    s.push_back(s3);

    network::Poll p(10000, s);
    std::cout << p << std::endl;

    p.run_servers(s);

    return (0);
}