#include "FCFSAlgorithm.h"

void Scheduler::FCFSAlgorithm::onProcessArrivedCallback(Scheduler* s, Process* proc) {
    //if this process has arrived and there are no processes in the readyQ
    //and no other process has been finished or blocked this cycle
    //put it at the front of the readyQ and start it
    if(s->readyQ->empty() && !processRemovedThisCycle && !processBlockedThisCycle) {
        s->readyQ->push(proc);
        proc->setIsRunning();
        proc->cycle(s->getCurrentCycle());
    }
    //otherwise, push it to the back of the readyQ and set it to ready
    else {
        s->readyQ->push(proc);
        proc->setIsReady();
        proc->cycle(s->getCurrentCycle());
    }
}

//if a process becomes ready during this cycle add it to the list of Process
//waiting for entry to the readyQ
void Scheduler::FCFSAlgorithm::onProcessReadyCallback(Scheduler* s, Process* proc) {
    s->movedToReadyState->push_back(proc);
}

//if a process ran during this cycle increment the amount of cycles the cpu
//was utilized
void Scheduler::FCFSAlgorithm::onProcessRunningCallback(Scheduler* s, Process* proc) {
    s->incrementCpuUtilizationCyclesCount();
}

//if a process blocked during this cycle remove it from the readyQ
void Scheduler::FCFSAlgorithm::onProcessBlockedCallback(Scheduler* s, Process* proc) {
    if(s->readyQ->front() != proc) {
        //sanity check
    }

    s->readyQ->pop();

    processBlockedThisCycle = true;
}

//if a process finished during this cycle remove it from the readyQ
//and increment the amount of processes that finished
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

    //set the callbacks for all processes
    for(it = allProcesses->begin(); it != allProcesses->end(); it++) {
        (*it)->setOnArrivedCallback(new OnProcessArrivedCallback(scheduler));
        (*it)->setOnReadyCallback(new OnProcessReadyCallback(scheduler));
        (*it)->setOnRunningCallback(new OnProcessRunningCallback(scheduler));
        (*it)->setOnBlockedCallback(new OnProcessBlockedCallback(scheduler));
        (*it)->setOnFinishedCallback(new OnProcessFinishedCallback(scheduler));
    }

    //loop while there are still unfinished processes
    while(scheduler->getFinishedProcessesCount() != scheduler->getProcessesCount()) {
        //if the readyQ isn't empty and a process isn't running
        //start running the top
        if(!readyQ->empty()) {
            if(!readyQ->front()->isRunning()) {
                readyQ->front()->setIsRunning();
            }
        }

        cout << scheduler->getCurrentCycle() << " ";
        //run a cycle for each process
        for(it = allProcesses->begin(); it != allProcesses->end(); it++) {
            (*it)->cycle(scheduler->getCurrentCycle());
        }
        cout << endl;

        //if there are processes ready to be moved to the readyQ
        if(!movedToReadyState->empty()) {
            //sort the list so that lower ids are higher
            sort(movedToReadyState->begin(), movedToReadyState->end(), LowerProcessIdPriority());

            //push all of these processes to the readyQ
            for(it = movedToReadyState->begin(); it != movedToReadyState->end(); it++) {
                readyQ->push((*it));
            }

            //clear the list of processes waiting to be moved to the readyQ
            movedToReadyState->clear();
        }

        //increments the cycle count in the scheduler
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
