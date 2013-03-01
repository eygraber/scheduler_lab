#ifndef PROCESS_H
#define PROCESS_H

#include<iostream>
#include<cmath>

using namespace std;

enum ProcessState { NOT_ARRIVED, RUNNING, READY, BLOCKED, FINISHED };
const int UNINITIALIZED_PROCESS_VAL = -1;

class Process {
    public:
        class ProcessStateChangeCallback {
            public:
                virtual void run(Process* proc) = 0;
        };

    private:
        int procId;
        int totalCpuTime;
        int cpuTimeBeforeBlock;
        int cpuTimeRemaining;
        int totalIoTime;
        int ioTimeRemaining;
        int arrivalCycle;
        int startCycle;
        int finishCycle;
        ProcessState state;

        ProcessStateChangeCallback* onArrived;
        ProcessStateChangeCallback* onReady;
        ProcessStateChangeCallback* onRunning;
        ProcessStateChangeCallback* onBlocked;
        ProcessStateChangeCallback* onFinished;

        void setArrivalCycle(int arrivalCycle) { this->arrivalCycle = arrivalCycle; }
        void setStartCycle(int startCycle) { this->startCycle = startCycle; }
        void setFinishCycle(int finishCycle) { this->finishCycle = finishCycle; }

        void setState(ProcessState state) { this->state = state; }
        void setIsBlocked() { setState(BLOCKED); }
        void setIsFinished() { setState(FINISHED); }

        bool setCpuTimeRemaining(int cpuTimeRemaining) {
            if(cpuTimeRemaining < 0) {
                return false;
            }
            this->cpuTimeRemaining = cpuTimeRemaining;
            return true;
        }
        bool decrementCpuTimeRemaining() { return setCpuTimeRemaining(getCpuTimeRemaining() - 1); }
        bool setIoTimeRemaining(int ioTimeRemaining) {
            if(ioTimeRemaining < 0) {
                return false;
            }
            this->ioTimeRemaining = ioTimeRemaining;
            return true;
        }
        bool decrementIoTimeRemaining() { return setIoTimeRemaining(getIoTimeRemaining() - 1); }
    public:
        Process(int procId, int cpuTime, int totalIoTime, int arrivalCycle, ProcessState state = NOT_ARRIVED)
            : procId(procId), totalCpuTime(cpuTime), totalIoTime(totalIoTime), arrivalCycle(arrivalCycle), state(state) {
            cpuTimeBeforeBlock = ceil(totalCpuTime / 2);
            cpuTimeRemaining = totalCpuTime;

            ioTimeRemaining = totalIoTime;

            startCycle = UNINITIALIZED_PROCESS_VAL;
            finishCycle = UNINITIALIZED_PROCESS_VAL;

            onArrived = NULL;
            onReady = NULL;
            onRunning = NULL;
            onBlocked = NULL;
            onFinished = NULL;
        }

        ~Process() {
            delete onArrived;
            delete onReady;
            delete onRunning;
            delete onBlocked;
            delete onFinished;
        }

        int getProcessId() { return procId; }
        int getTotalCpuTime() { return totalCpuTime; }
        int getCpuTimeBeforeBlock() { return cpuTimeBeforeBlock; }
        int getCpuTimeRemaining() { return cpuTimeRemaining; }
        int getTotalIoTime() { return totalIoTime; }
        int getIoTimeRemaining() { return ioTimeRemaining; }
        int getArrivalCycle() { return arrivalCycle; }

        int getTurnaroundTime() { return (getFinishedCycle() - getArrivalCycle()) + 1; }

        ProcessState getState() { return state; }
        bool isNotArrived() { return getState() == NOT_ARRIVED; }
        bool isRunning() { return getState() == RUNNING; }
        bool isReady() { return getState() == READY; }
        bool isBlocked() { return getState() == BLOCKED; }

        int getStartCycle() { return startCycle; }
        int getFinishedCycle() { return finishCycle; }

        void setIsReady() { setState(READY); }
        void setIsRunning() { setState(RUNNING); }

        bool isStarted() { return this->startCycle != UNINITIALIZED_PROCESS_VAL; }
        bool isFinished() { return this->finishCycle != UNINITIALIZED_PROCESS_VAL && getState() == FINISHED; }

        void setOnArrivedCallback(ProcessStateChangeCallback* onArrived) {
            this->onArrived = onArrived;
        }

        void setOnReadyCallback(ProcessStateChangeCallback* onReady) {
            this->onReady = onReady;
        }

        void setOnRunningCallback(ProcessStateChangeCallback* onRunning) {
            this->onRunning = onRunning;
        }

        void setOnBlockedCallback(ProcessStateChangeCallback* onBlocked) {
            this->onBlocked = onBlocked;
        }

        void setOnFinishedCallback(ProcessStateChangeCallback* onFinished) {
            this->onFinished = onFinished;
        }

        bool operator<(Process& other);
        bool operator>(Process& other);

        void cycle(int cycleNum);
};

struct LowerProcessIdPriority {
    inline bool operator() (const Process* p1, const Process* p2) {
        return p1 > p2;
    }
};

struct HigherProcessIdPriority {
    inline bool operator() (const Process* p1, const Process* p2) {
        return p1 < p2;
    }
};

#endif // PROCESS_H
