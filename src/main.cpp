
#include <sys/event.h>
#include <sys/time.h>
#include <unistd.h>

#include <fstream>
#include <sstream>

#include "Config/ConfigParser.hpp"
#include "network/SimpleRequest.hpp"
#include "network/Socket.hpp"
#include "utils/Thread.hpp"

double now_double(void) {
    struct timespec tv;

    if (clock_gettime(CLOCK_REALTIME, &tv) != 0) return 0;

    return (tv.tv_sec + (tv.tv_nsec / 1000000000.0));
}

void *exec_when_sig(void *arg) {
    utils::Thread *self = (utils::Thread *)arg;

    while (1) {
        pthread_mutex_lock(self->get_lock());
        pthread_cond_wait(self->get_cond(), self->get_lock());
        RequestHandler handler;
        network::SimpleRequest tmp(self->get_socket().do_accept());
        std::cout << std::endl
                  << "Request on port " << self->get_socket().get_port()
                  << std::endl;
        Result<Request> res =
            handler.update(tmp.get_data().c_str(), tmp.get_data().length());
        if (res.is_ok()) {
            Request req = res.unwrap();
			std::string rep = "HTTP/1.1 200 OK\nServer: WebServer\nContent-length: 2\nContent-type: text/html;\n\nok";
            if (send(tmp.get_fd(), rep.c_str(), rep.length(), 0) < 0)
				std::cerr << "Send Error" << std::endl;
            std::cout << "Response Rent from port "
                      << self->get_socket().get_port() << " to " << tmp.get_fd() << std::endl;
			close(tmp.get_fd());
        } else {
            std::cout << "Error : Request not okay" << std::endl;
        }
        pthread_mutex_unlock(self->get_lock());
    }
    return (NULL);
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

    std::vector<utils::Thread> thread_pool(servers_sockets.size());
    std::vector<network::Socket>::iterator serv_it = servers_sockets.begin();
    std::vector<utils::Thread>::iterator thread_it = thread_pool.begin();

	std::cout << "Building " << servers_sockets.size() << " threads with sockets on ports : ";
    for (; serv_it != servers_sockets.end(); serv_it++) {
        thread_it->link_server(*serv_it);
        thread_it->init(exec_when_sig);
		std::cout << serv_it->get_port() << " ";
        thread_it++;
    }
    std::cout << std::endl << "Pool OK : all workers are ready" << std::endl;

    int kq, event;
    struct kevent event_to_monitor[servers_sockets.size()];
    struct kevent triggered_events[servers_sockets.size()];

    if ((kq = kqueue()) < 0) {
        std::cerr << "Error: kqueue" << std::endl;
    }

    int index = 0;
    for (serv_it = servers_sockets.begin(); serv_it != servers_sockets.end();
         serv_it++) {
        EV_SET(&event_to_monitor[index], serv_it->get_id(), EVFILT_READ,
               EV_ADD | EV_ENABLE, 0, 0, 0);
        index++;
    }

    for (;;) {
        event = kevent(kq, event_to_monitor, servers_sockets.size(),
                       triggered_events, servers_sockets.size(), NULL);
        if (event < 0) {
            std::cerr << "Error: kevent" << std::endl;
            return (0);
        } else if (event > 0) {
            if (triggered_events[0].flags & EV_EOF) exit(EXIT_FAILURE);

            for (int i = 0; i < event; i++) {
                if (triggered_events[i].flags & EV_ERROR) {
                    fprintf(stderr, "EV_ERROR: %s\n",
                            strerror(triggered_events[i].data));
                    exit(EXIT_FAILURE);
                }

                for (int j = 0; j < static_cast<int>(servers_sockets.size()); j++) {
                    if (triggered_events[i].ident ==
                        static_cast<uintptr_t>(servers_sockets[j].get_id())) {
                        pthread_mutex_lock(thread_pool[j].get_lock());
                        pthread_cond_signal(thread_pool[j].get_cond());
                        pthread_mutex_unlock(thread_pool[j].get_lock());
                    }
                }
            }
        }
    }
    return 0;
}
