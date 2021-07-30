#include "Thread.hpp"

namespace utils {

/***************************************************
Constructors and Destructor
***************************************************/

Thread::Thread(void) {
    // std::cout << "Worker ready" << std::endl;
}

void Thread::init(void *fn(void *), void *args) {
	pthread_cond_init(&_cond, NULL);
	pthread_mutex_init(&_lock, NULL);
    if (pthread_create(&_id, NULL, fn, args) != 0) {
        std::cerr << "Error: creating thread" << std::endl;
        this->~Thread();
    }
}
void Thread::init(void *fn(void *)) {
	pthread_cond_init(&_cond, NULL);
	pthread_mutex_init(&_lock, NULL);
    if (pthread_create(&_id, NULL, fn, this) != 0) {
        std::cerr << "Error: creating thread" << std::endl;
        this->~Thread();
    }
}

Thread::~Thread() {}

/***************************************************
Member Functions
***************************************************/

void Thread::join() {
    if (is_joinable()) {
        pthread_join(_id, NULL);
        _joinable = false;
    } else
        std::cerr << "Error: thread not joinable" << std::endl;
}

void Thread::detach() {
    if (pthread_detach(_id) != 0) {
        _joinable = false;
		// std::cout << strerror(errno) << std::endl;
        std::cerr << "Non c'est bon en fait" << std::endl;
	}
    else {
        std::cerr << "Error: thread not here I guess joinable" << std::endl;
	}
}

/*
*	A thread is not joinable if:
*		- It was default-constructed
*		- If either of its member join or detach has been called
*		- It has been moved elsewhere (?)
*/

bool Thread::is_joinable() const { return (_joinable); }

void	Thread::link_server(network::Socket & socket) {
    _socket = socket;
}

/***************************************************
Getters and Setters
***************************************************/

pthread_cond_t	*Thread::get_cond( void ) { return &_cond; }

pthread_mutex_t	*Thread::get_lock( void ) { return &_lock; }

pthread_t       Thread::get_id(void) const { return _id; }

network::Socket	Thread::get_socket(void) const { return _socket; }


}  // namespace utils