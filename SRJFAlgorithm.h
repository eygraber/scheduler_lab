#ifndef SRJFALGORITHM_H
#define SRJFALGORITHM_H

#include "Scheduler.h"

class Scheduler::SRJFAlgorithm : public Scheduler::SchedulingAlgorithm {
    public:
        SRJFAlgorithm(Scheduler* scheduler) : scheduler(scheduler), processBlockedThisCycle(false), processRemovedThisCycle(false) {}
        void run();
        void onProcessArrivedCallback(Scheduler* s, Process* proc);
        void onProcessReadyCallback(Scheduler* s, Process* proc);
        void onProcessRunningCallback(Scheduler* s, Process* proc);
        void onProcessBlockedCallback(Scheduler* s, Process* proc);
        void onProcessFinishedCallback(Scheduler* s, Process* proc);
    private:
        Scheduler* scheduler;
        bool processBlockedThisCycle;
        bool processRemovedThisCycle;

        void sortReadyQueue(queue<Process*>* readyQ);
};

#endif // SRJFALGORITHM_H
