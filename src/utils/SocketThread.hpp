#ifndef UTILS_SOCKET_THREAD_HPP
#define UTILS_SOCKET_THREAD_HPP

#include <errno.h>
#include <pthread.h>
#include <stdio.h>

#include <iostream>

#include "../network/Socket.hpp"
#include "../utils/Thread.hpp"

namespace utils {

/*
 * A Socket Thread differenciate itself from a regular Thread by its added
 * attributs. It has a Socket, a mutex and a pthread_condition to allow kqueue()
 * work
 */

class SocketThread : public Thread {
   public:
    /* Constructor and destructor */
    SocketThread(void);
    ~SocketThread();

    /* Getters and Setters */
    pthread_cond_t *get_cond(void);
    pthread_mutex_t *get_lock(void);
    network::Socket get_socket(void) const;

    /* Member functions */
    void init(void *fn(void *));
    void link_server(network::Socket &socket);

   private:
    pthread_cond_t _cond;
    pthread_mutex_t _lock;
    network::Socket _socket;
};
}  // namespace utils

#endif