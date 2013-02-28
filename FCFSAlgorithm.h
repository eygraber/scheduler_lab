#ifndef FCFSALGORITHM_H
#define FCFSALGORITHM_H

#include "Scheduler.h"

class Scheduler::FCFSAlgorithm : public Scheduler::SchedulingAlgorithm {
    public:
        FCFSAlgorithm(Scheduler* scheduler) : scheduler(scheduler), processBlockedThisCycle(false), processRemovedThisCycle(false) {}
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
};

#endif // FCFSALGORITHM_H
