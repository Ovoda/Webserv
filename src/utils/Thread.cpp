#include "Thread.hpp"

namespace utils {

/*
 * Creates a Thread instance, does nothing else
 */
Thread::Thread(void) {
    // std::cout << "Worker ready" << std::endl;
}

/*
 * Destructor
 */
Thread::~Thread() {}

/*
 * Init(fn, args) : creates a thread, its cond and its mutex then calls the
 * routine with given args
 */
void Thread::init(void *fn(void *), void *args) {
    if (pthread_create(&_id, NULL, fn, args) != 0) {
        std::cerr << "Error: creating thread" << std::endl;
        this->~Thread();
    }
}

/*
 * join() : joins a thread if possible, outputs error otherwise
 */
void Thread::join() {
    if (is_joinable()) {
        pthread_join(_id, NULL);
        _joinable = false;
    } else
        std::cerr << "Error: thread not joinable" << std::endl;
}

/*
 * detach() : detach a thread if possible, outputs error otherwise
 */
void Thread::detach() {
    if (pthread_detach(_id) != 0) {
        _joinable = false;
        // std::cout << strerror(errno) << std::endl;
        std::cerr << "Non c'est bon en fait" << std::endl;
    } else {
        std::cerr << "Error: thread not here I guess joinable" << std::endl;
    }
}

/*
 *  Returns true if the current instance is joinable. false otherwise
 */
bool Thread::is_joinable() const { return (_joinable); }

pthread_t Thread::get_id(void) const { return _id; }
}  // namespace utils