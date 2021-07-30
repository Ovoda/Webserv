#include <unistd.h>

#include <iostream>
#include <vector>

#include "src/utils/Thread.hpp"

int done = 0;

void *exec_when_sig(void *arg) {
    utils::Thread *self = (utils::Thread *)arg;

    while (1) {
        pthread_mutex_lock(self->get_lock());

        pthread_cond_wait(self->get_cond(), self->get_lock());
        std::cout << "Thread " << pthread_self() << " is awaken" << std::endl;
        sleep(5);
        std::cout << "Back to bed for Thread " << pthread_self() << std::endl;

        pthread_mutex_unlock(self->get_lock());
    }
    std::cout << "Returning to main" << std::endl;
    return (NULL);
}

int main(void) {
    std::vector<utils::Thread> thread_pool(5);
    std::vector<utils::Thread>::iterator it;

    for (it = thread_pool.begin(); it != thread_pool.end(); it++)
        it->init(exec_when_sig);

    std::cout << "Thread pool ready" << std::endl;

    int i;
    while (1) {
        std::cin >> i;
        pthread_mutex_lock(thread_pool[i].get_lock());
        pthread_cond_signal(thread_pool[i].get_cond());
        pthread_mutex_unlock(thread_pool[i].get_lock());
    }

    // thread_pool.insert(pair<utils::Thread,
    // pthread_cond_t>(utils::Thread(exec_when_sig), 40));

    // utils::Thread thread(exec_when_sig);
    // usleep(100);

    // int i;
    // while (1) {
    //     std::cin >> i;
    //     pthread_cond_signal(&cond);
    // }

    return (0);
}