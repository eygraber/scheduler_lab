#ifndef RRALGORITHM_H
#define RRALGORITHM_H

#include "Scheduler.h"

class Scheduler::RRAlgorithm : public Scheduler::SchedulingAlgorithm {
    public:
        RRAlgorithm(Scheduler* scheduler) : scheduler(scheduler), processBlockedThisCycle(false), processRemovedThisCycle(false),
                                            quantum(2), runningProcessId(-1), runningProcessQuantum(quantum) {}
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
        int quantum;
        int runningProcessId;
        int runningProcessQuantum;
};

#endif // RRALGORITHM_H
