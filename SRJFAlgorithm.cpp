#include "SRJFAlgorithm.h"

struct ShortestRemainingCpuTimePriority {
    inline bool operator() (Process* p1, Process* p2) {
        if(p1->getCpuTimeRemaining() < p2->getCpuTimeRemaining()) {
            return true;
        }
        else if(p1->getCpuTimeRemaining() == p2->getCpuTimeRemaining()) {
            if(p1->getProcessId() < p2->getProcessId()) {
                return true;
            }
            else {
                return false;
            }
        }
        else {
            return false;
        }
    }
};

void Scheduler::SRJFAlgorithm::onProcessArrivedCallback(Scheduler* s, Process* proc) {
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

void Scheduler::SRJFAlgorithm::onProcessReadyCallback(Scheduler* s, Process* proc) {
    s->movedToReadyState->push_back(proc);
}

void Scheduler::SRJFAlgorithm::onProcessRunningCallback(Scheduler* s, Process* proc) {
    s->incrementCpuUtilizationCyclesCount();
}

void Scheduler::SRJFAlgorithm::onProcessBlockedCallback(Scheduler* s, Process* proc) {
    if(s->readyQ->front() != proc) {
        //sanity check
    }

    s->readyQ->pop();

    processBlockedThisCycle = true;
}

void Scheduler::SRJFAlgorithm::onProcessFinishedCallback(Scheduler* s, Process* proc) {
    if(s->readyQ->front() != proc) {
        //sanity check
    }
    s->readyQ->pop();
    s->incrementFinishedProcessesCount();
    processRemovedThisCycle = true;
}

void Scheduler::SRJFAlgorithm::run() {
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
            proc->setIsReady();
            sortReadyQueue(readyQ);
            proc = readyQ->front();
            proc->setIsRunning();
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

void Scheduler::SRJFAlgorithm::sortReadyQueue(queue<Process*>* readyQ) {
    vector<Process*>* temp = new vector<Process*>();

    while(!readyQ->empty()) {
        temp->push_back(readyQ->front());
        readyQ->pop();
    }

    sort(temp->begin(), temp->end(), ShortestRemainingCpuTimePriority());

    vector<Process*>::iterator temp_it;
    for(temp_it = temp->begin(); temp_it != temp->end(); temp_it++) {
        readyQ->push((*temp_it));
    }

    delete temp;

}

