#include "RRAlgorithm.h"

void Scheduler::RRAlgorithm::onProcessArrivedCallback(Scheduler* s, Process* proc) {
    if(s->readyQ->empty() && !processRemovedThisCycle && !processBlockedThisCycle) {
        s->readyQ->push(proc);
        proc->setIsRunning();
        runningProcessId = proc->getProcessId();
        runningProcessQuantum = quantum;
        proc->cycle(s->getCurrentCycle());
    }
    else {
        s->readyQ->push(proc);
        proc->setIsReady();
        proc->cycle(s->getCurrentCycle());
    }
}

void Scheduler::RRAlgorithm::onProcessReadyCallback(Scheduler* s, Process* proc) {
    s->movedToReadyState->push_back(proc);
}

void Scheduler::RRAlgorithm::onProcessRunningCallback(Scheduler* s, Process* proc) {
    s->incrementCpuUtilizationCyclesCount();
}

void Scheduler::RRAlgorithm::onProcessBlockedCallback(Scheduler* s, Process* proc) {
    if(s->readyQ->front() != proc) {
        //sanity check
    }

    s->readyQ->pop();

    processBlockedThisCycle = true;
    runningProcessId = -1;
    runningProcessQuantum = quantum;
}

void Scheduler::RRAlgorithm::onProcessFinishedCallback(Scheduler* s, Process* proc) {
    if(s->readyQ->front() != proc) {
        //sanity check
    }
    s->readyQ->pop();
    s->incrementFinishedProcessesCount();
    processRemovedThisCycle = true;
    runningProcessId = -1;
    runningProcessQuantum = quantum;
}

void Scheduler::RRAlgorithm::run() {
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
            Process* proc = readyQ->front();
            if(!proc->isRunning()) {
                proc->setIsRunning();
                runningProcessId = proc->getProcessId();
                runningProcessQuantum = quantum;
            }
            else {
                if(proc->getProcessId() == runningProcessId) {
                    if(runningProcessQuantum == 0) {
                        readyQ->pop();
                        readyQ->push(proc);
                        proc->setIsReady();
                        proc = readyQ->front();
                        proc->setIsRunning();
                    }
                }
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

        if(runningProcessId != -1) {
            runningProcessQuantum--;
        }
    }
    cout << endl;
}

