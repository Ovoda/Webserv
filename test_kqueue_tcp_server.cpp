#include "src/utils/KqueueThread.hpp"

int main(void) {
    utils::KqueueThread kt;

    kt.add(4);
    kt.add(5);
    kt.add(6);
    kt.add(7);
    return (0);
}