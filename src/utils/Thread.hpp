#ifndef UTILS_THREAD_HPP
#define UTILS_THREAD_HPP

#include <pthread.h>
#include <iostream>
#include <errno.h>
#include <stdio.h>
#include "network/Socket.hpp"

namespace utils {
class Thread {
   public:
	Thread(void);
	~Thread();
	void init(void *fn(void *), void *args);
	void init(void *fn(void *));
	pthread_t get_id(void) const;
	const pthread_attr_t	*get_attr( void ) const;
	void	join(void);
	void	detach(void);
	bool	is_joinable() const;
	pthread_cond_t	*get_cond( void );
	pthread_mutex_t	*get_lock( void );
	void	link_server(network::Socket & socket);
	network::Socket	get_socket(void) const;


   private:
	pthread_t 		_id;
	bool			_joinable;
	pthread_cond_t	_cond;
	pthread_mutex_t	_lock;
	network::Socket _socket;
};
}  // namespace utils

#endif