#ifndef KQUEUE_THREAD_HPP
#define KQUEUE_THREAD_HPP

#include <sys/event.h>
#include <sys/time.h>
#include <sys/types.h>

#include <iostream>
#include <vector>

#include "SocketThread.hpp"
#include "Thread.hpp"

namespace utils {
class KqueueThread : public Thread {
   public:
	KqueueThread(void);
	~KqueueThread();

	void add(int fd);
	void monitor();

   private:
	struct kevent	_event_to_monitor[4096];
	struct kevent	_triggered_events[4096];
	int				_index;
};
}  // namespace utils

#endif