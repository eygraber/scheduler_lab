#include "Scheduler.h"
#include "FCFSAlgorithm.h"
#include "RRAlgorithm.h"
#include "SRJFAlgorithm.h"

Scheduler::SchedulingAlgorithm* Scheduler::getSchedulingAlgorithm(AlgorithmTypes algorithmType) {
    switch(algorithmType) {
        case FCFS:
            return new FCFSAlgorithm(this);
        case RR:
            return new RRAlgorithm(this);
        case SRJF:
            return new SRJFAlgorithm(this);
        default:
            return NULL; //its an error
    }
}

void Scheduler::printStatistics() {
    cout << "Finishing time: " << (getCurrentCycle() - 1) << endl;
    cout.precision(2);
    cout << "CPU utilization: " << getCpuUtilization() << endl;

    for(it = allProcesses->begin(); it != allProcesses->end(); it++) {
        cout << "Turnaround process " << (*it)->getProcessId() << ": " << (*it)->getTurnaroundTime() << endl;
    }
}
