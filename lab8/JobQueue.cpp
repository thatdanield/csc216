//
// Created by Daniel Devadas on 10/18/26.
//

#include "JobQueue.h"

void JobQueue::insert(Job* j) {
    queue.push(j);
}

void JobQueue::runHighestPriority() {
    queue.top()->execute();
    queue.pop();
}