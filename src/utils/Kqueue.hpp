#ifndef KQUEUE_HPP
#define KQUEUE_HPP

#include <sys/event.h>
#include <sys/time.h>
#include <sys/types.h>

#include <iostream>
#include <vector>

#include "../network/Socket.hpp"
#include "SocketThread.hpp"

namespace utils {
class KQueue {
   public:
    KQueue(int nb);
    ~KQueue();

    void add_event(int fd);

    int get_kq(void) const;
    int get_events(void) const;
    int get_nb_fd(void) const;
    struct kevent *get_event_to_monitor(void);
    struct kevent *get_triggered_events(void);
    void monitor(std::vector<network::Socket> servers_sockets,
                 std::vector<utils::SocketThread> thread_pool);

   private:
    KQueue(void) {}
    int _kqueue, _events, _nb_fd, _index;
    struct kevent *_event_to_monitor;
    struct kevent *_triggered_events;
};
}  // namespace utils

#endif