
#include <fstream>
#include <sstream>

#include "Config/ConfigParser.hpp"
#include "network/Socket.hpp"
#include "network/SimpleRequest.hpp"
#include "network/UserBase.hpp"

double now_double(void) {
	struct timespec tv;

	if (clock_gettime(CLOCK_REALTIME, &tv) != 0) return 0;

	return (tv.tv_sec + (tv.tv_nsec / 1000000000.0));
}

int main(int ac, char **av) {
	std::string path;
	switch (ac) {
		case 1:
			path = "webserv.config";
			break;
		case 2:
			path = av[1];
			break;
		default:
			std::cerr << "./webserv [ConfigServerv]" << std::endl;
			return -1;
	}
	std::ifstream t(path.c_str());
	std::stringstream buffer;
	buffer << t.rdbuf();
	std::string str = buffer.str();
	slice cfg(str);

	ParserResult<std::vector<config::Server> > cfgs = ConfigParser()(cfg);
	if (cfgs.is_err()) {
		std::cerr << cfgs.unwrap_err() << std::endl;
#if LOG_LEVEL == LOG_LEVEL_TRACE  // start line error
		cfgs.unwrap_err().trace(cfg);
#endif
	} else {
		for (std::vector<config::Server>::iterator it = cfgs.unwrap().begin();
			 it != cfgs.unwrap().end(); it++) {
			std::cout << *it << std::endl;
		}
	}

	////----------------------------------------------------------------------------------------------------------------------////
	std::vector<network::Socket> servers_sockets;

	for (std::vector<config::Server>::iterator it = cfgs.unwrap().begin();
		 it != cfgs.unwrap().end(); it++) {
		servers_sockets.push_back(it->get_port());
	}

	network::Socket s(servers_sockets[0]);
	int fd;
	network::UserBase	base;

	while (1) {
		network::SimpleRequest	tmp_sr = s.do_accept();
		if (base.is_known_ip(tmp_sr.get_ipv4()) == true) {
			std::cout << tmp_sr.get_ipv4() << " is a known ip" << std::endl;
		}
		base.add(tmp_sr);

		std::cout << "requests summary : " << std::endl;
		base.requests_log();

		// char buffer[30000] = {0};
		// recv(fd, buffer, 30000, 0);
		// std::cout << "------------------------------------------- Received "
		// 			 "-------------------------------------------"
		// 		  << std::endl;
		// std::cout << buffer << std::endl;
	}

	return 0;
}
