#include "Process.h"

void Process::cycle(int cycleNum) {
    switch(getState()) {
        case NOT_ARRIVED:
            //if the process is supposed to arrive at this cycle
            if(cycleNum == getArrivalCycle()) {
                setIsReady();

                if(onArrived != NULL) {
                    onArrived->run(this);
                }
            }
            break;
        case RUNNING:
            cout << getProcessId() << ":running ";

            //if this is the first cycle the process is running
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

            //in the special case of 0 ioTime and odd cpuTime
            //getCpuTimeRemaining() will never equal getCpuTimeBeforeBlock()
            //because we initialize cpuTimeRemaining and cpuTimeBeforeBlock to totalCpuTime
            //and since there is a call that decrements cpuTimeRemaining before this check
            //it will never equal totalCpuTime at this point
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
