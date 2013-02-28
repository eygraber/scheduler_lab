#include "FCFSAlgorithm.h"

void Scheduler::FCFSAlgorithm::onProcessArrivedCallback(Scheduler* s, Process* proc) {
    if(s->readyQ->empty() && !processRemovedThisCycle && !processBlockedThisCycle) {
        s->readyQ->push(proc);
        proc->setIsRunning();
        proc->cycle(s->getCurrentCycle());
    }
    else {
        s->readyQ->push(proc);
        proc->setIsReady();
        proc->cycle(s->getCurrentCycle());
    }
}

void Scheduler::FCFSAlgorithm::onProcessReadyCallback(Scheduler* s, Process* proc) {
    s->movedToReadyState->push_back(proc);
}

void Scheduler::FCFSAlgorithm::onProcessRunningCallback(Scheduler* s, Process* proc) {
    s->incrementCpuUtilizationCyclesCount();
}

void Scheduler::FCFSAlgorithm::onProcessBlockedCallback(Scheduler* s, Process* proc) {
    if(s->readyQ->front() != proc) {
        //sanity check
    }

    s->readyQ->pop();

    processBlockedThisCycle = true;
}

void Scheduler::FCFSAlgorithm::onProcessFinishedCallback(Scheduler* s, Process* proc) {
    if(s->readyQ->front() != proc) {
        //sanity check
    }
    s->readyQ->pop();
    s->incrementFinishedProcessesCount();
    processRemovedThisCycle = true;
}

void Scheduler::FCFSAlgorithm::run() {
    vector<Process*>* allProcesses = scheduler->allProcesses;
    vector<Process*>* movedToReadyState = scheduler->movedToReadyState;
    vector<Process*>::iterator it = scheduler->it;

    queue<Process*>* readyQ = scheduler->readyQ;

    for(it = allProcesses->begin(); it != allProcesses->end(); it++) {
        (*it)->setOnArrivedCallback(new OnProcessArrivedCallback(scheduler));
        (*it)->setOnReadyCallback(new OnProcessReadyCallback(scheduler));
        (*it)->setOnRunningCallback(new OnProcessRunningCallback(scheduler));
        (*it)->setOnBlockedCallback(new OnProcessBlockedCallback(scheduler));
        (*it)->setOnFinishedCallback(new OnProcessFinishedCallback(scheduler));
    }

    while(scheduler->getFinishedProcessesCount() != scheduler->getProcessesCount()) {
        if(!readyQ->empty()) {
            if(!readyQ->front()->isRunning()) {
                readyQ->front()->setIsRunning();
            }
        }

        cout << scheduler->getCurrentCycle() << " ";
        for(it = allProcesses->begin(); it != allProcesses->end(); it++) {
            (*it)->cycle(scheduler->getCurrentCycle());
        }
        cout << endl;

        if(!movedToReadyState->empty()) {
            sort(movedToReadyState->begin(), movedToReadyState->end(), LowerProcessIdPriority());

            for(it = movedToReadyState->begin(); it != movedToReadyState->end(); it++) {
                readyQ->push((*it));
            }

            movedToReadyState->clear();
        }

        scheduler->incrementCycle();
        if(processRemovedThisCycle) {
            processRemovedThisCycle = false;
        }
        if(processBlockedThisCycle) {
            processBlockedThisCycle = false;
        }
    }
    cout << endl;
}
