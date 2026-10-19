//
// Created by Daniel Devadas on 10/18/26.
//
#include "job.hpp"
#include <queue>
#include <vector>

#ifndef LAB8_JOBQUEUE_H
#define LAB8_JOBQUEUE_H
class JobQueue {
    struct PriorityCompare {
        bool operator()(Job* lhs, Job* rhs) const {
            return *lhs < *rhs;
        }
    };

    std::priority_queue<Job*, std::vector<Job*>, PriorityCompare> queue;
public:

    void insert(Job* j);
    void runHighestPriority();
};


#endif //LAB8_JOBQUEUE_H
