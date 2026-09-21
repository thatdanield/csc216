#include "random_walk.h"

#include <cstdlib>

int randomWalk(int steps) {
    int x = 0;
    for (int i = 0; i < steps; i++) {
        if (std::rand() % 2 == 0) {
            x++;
        } else {
            x--;
        }
    }

    return x;
}

#ifndef RANDOM_WALK_NO_MAIN
int main() {
    int trials = 10;
    for (int steps = 5; steps < 10; steps++) {
        for (int i = 0; i < trials; i++) {
            randomWalk(steps);
        }
    }
}
#endif
