#include "Kqueue.hpp"

namespace utils {
KQueue::KQueue(int nb) {
    if ((_kqueue = kqueue()) < 0) {
        std::cerr << "Erreur : creating kqueue" << std::endl;
        this->~KQueue();
    }
    _nb_fd = nb;
    _index = 0;
    _event_to_monitor = new struct kevent;
    _triggered_events = new struct kevent;
}

KQueue::~KQueue() {}

void KQueue::add_event(int fd) {
    EV_SET(&_event_to_monitor[_index], fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0,
           0, 0);
    _index++;
}

struct kevent *KQueue::get_event_to_monitor(void) {
    return (_event_to_monitor);
}

struct kevent *KQueue::get_triggered_events(void) {
    return (_triggered_events);
}

void KQueue::monitor(std::vector<network::Socket> servers_sockets,
                     std::vector<utils::Thread> thread_pool) {
    int kq, event;
    struct kevent event_to_monitor[servers_sockets.size()];
    struct kevent triggered_events[servers_sockets.size()];

    if ((kq = kqueue()) < 0) {
        std::cerr << "Error: kqueue" << std::endl;
    }

    int index = 0;
    for (std::vector<network::Socket>::iterator serv_it =
             servers_sockets.begin();
         serv_it != servers_sockets.end(); serv_it++) {
        EV_SET(&event_to_monitor[index], serv_it->get_id(), EVFILT_READ,
               EV_ADD | EV_ENABLE, 0, 0, 0);
        index++;
    }

    for (;;) {
		std::cout << "ok" << std::endl;
        event = kevent(kq, event_to_monitor, servers_sockets.size(),
                       triggered_events, servers_sockets.size(), NULL);
        if (event < 0) {
            std::cerr << "Error: kevent" << std::endl;
            return;
        } else if (event > 0) {
            if (triggered_events[0].flags & EV_EOF) exit(EXIT_FAILURE);

            for (int i = 0; i < event; i++) {
                if (triggered_events[i].flags & EV_ERROR) {
                    fprintf(stderr, "EV_ERROR: %s\n",
                            strerror(triggered_events[i].data));
                    exit(EXIT_FAILURE);
                }

                for (int j = 0; j < static_cast<int>(servers_sockets.size());
                     j++) {
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
}

int KQueue::get_kq(void) const { return _kqueue; }
int KQueue::get_events(void) const { return _events; }
int KQueue::get_nb_fd(void) const { return _nb_fd; }

}  // namespace utils