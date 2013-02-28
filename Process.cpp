#include "Process.h"

void Process::cycle(int cycleNum) {
    switch(getState()) {
        case NOT_ARRIVED:
            if(cycleNum == getArrivalCycle()) {
                setIsReady();

                if(onArrived != NULL) {
                    onArrived->run(this);
                }
            }
            break;
        case RUNNING:
            cout << getProcessId() << ":running ";

            if(getCpuTimeRemaining() == getTotalCpuTime()) {
                setStartCycle(cycleNum);
            }

            if(onRunning != NULL) {
                onRunning->run(this);
            }

            if(!decrementCpuTimeRemaining()) {
                //tried decrementing a process with no remaining cpu time?
                //throw exception
            }

            if(getCpuTimeRemaining() == getCpuTimeBeforeBlock()) {
                if(getIoTimeRemaining() > 0) {
                    setIsBlocked();
                    if(onBlocked != NULL) {
                        onBlocked->run(this);
                    }
                }
            }
            else if(getCpuTimeRemaining() == 0) {
                setIsFinished();
                if(onFinished != NULL) {
                    onFinished->run(this);
                }
                setFinishCycle(cycleNum);
            }

            break;
        case READY:
            cout << getProcessId() << ":ready ";
            break;
        case BLOCKED:
            cout << getProcessId() << ":blocked ";

            if(!decrementIoTimeRemaining()) {
                //tried decrementing a process with no remaining io time?
                //throw exception
            }

            if(getIoTimeRemaining() == 0) {
                setIsReady();

                if(onReady != NULL) {
                    onReady->run(this);
                }
            }

            break;
        default:
            break;
    }
}

bool Process::operator<(Process& other) {
    return getProcessId() < other.getProcessId();
}

bool Process::operator>(Process& other) {
    return getProcessId() > other.getProcessId();
}
