#include "KqueueThread.hpp"

namespace utils {
KqueueThread::KqueueThread(void) : _index(0) {
}
KqueueThread::~KqueueThread() {}

void KqueueThread::add(int fd) {
    EV_SET(&_event_to_monitor[_index], fd, EVFILT_READ, EV_ADD | EV_ENABLE,
           0, 0, 0);
    _index++;
}

void KqueueThread::monitor() {
    int event, kq;
    if ((kq = kqueue()) < 0) {
        std::cerr << "Error: kqueue" << std::endl;
    }
    for (;;) {
        event = kevent(kq, _event_to_monitor, _index, _triggered_events, _index,
                       NULL);
        if (event < 0) {
            std::cerr << "Error: kevent" << std::endl;
            return;
        } else if (event > 0) {
            if (_triggered_events[0].flags & EV_EOF) exit(EXIT_FAILURE);

            for (int i = 0; i < event; i++) {
                if (_triggered_events[i].flags & EV_ERROR) {
                    fprintf(stderr, "EV_ERROR: %s\n",
                            strerror(_triggered_events[i].data));
                    exit(EXIT_FAILURE);
                }
                if (_triggered_events[i].fflags & NOTE_WRITE) {
                    std::cout << "EVENT: write" << std::endl;
                }
            }
        }
    }
}

}  // namespace utils