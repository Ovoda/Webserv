#include "SocketThread.hpp"

namespace utils {

/*
 * Creates a SocketThread instance, does nothing else
 */
SocketThread::SocketThread(void) {
    // std::cout << "Worker ready" << std::endl;
}

/*
 * Destructor
 */
SocketThread::~SocketThread() {}

/*
 * Init(fn, args) : creates a SocketThread, its cond and its mutex then calls the
 * routine with itself as arg
 */
void SocketThread::init(void *fn(void *)) {
    pthread_cond_init(&_cond, NULL);
    pthread_mutex_init(&_lock, NULL);
    Thread::init(fn, this);
}

/*
 *  Links a server to a SocketThread
 */
void SocketThread::link_server(network::Socket &socket) { _socket = socket; }

pthread_cond_t *SocketThread::get_cond(void) { return &_cond; }

pthread_mutex_t *SocketThread::get_lock(void) { return &_lock; }

network::Socket SocketThread::get_socket(void) const { return _socket; }

}  // namespace utils