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
    std::set<int>               seti;
    seti.insert(18000);
    seti.insert(18001);
    seti.insert(18002);

    std::vector<network::Socket> s(seti.begin(), seti.end());

    network::Poll p(s);
    p.run_servers(s);

    // check if non blocking works well
    // s1.do_accept();

    return (0);
}