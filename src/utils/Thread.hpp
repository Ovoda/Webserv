#ifndef UTILS_THREAD_HPP
#define UTILS_THREAD_HPP

#include <errno.h>
#include <pthread.h>
#include <stdio.h>

#include <iostream>

#include "../network/Socket.hpp"

namespace utils {
class Thread {
   public:
    /* Constructor and destructor */
    Thread(void);
    virtual ~Thread();

    /* Getters and Setters */
    pthread_t get_id(void) const;

    /* Member functions */
    void init(void *fn(void *), void *args);
    void join(void);
    void detach(void);
    bool is_joinable() const;

   protected:
    pthread_t _id;
    bool _joinable;
};
}  // namespace utils

#endif