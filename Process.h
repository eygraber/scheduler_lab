#ifndef PROCESS_H
#define PROCESS_H

#include<iostream>
#include<cmath>

using namespace std;

//provides an enum for the types of algorithms
enum ProcessState { NOT_ARRIVED, RUNNING, READY, BLOCKED, FINISHED };
//value given to a uninitialized variable
const int UNINITIALIZED_PROCESS_VAL = -1;

class Process {
    public:
        //base class to use as a callback in the scheduler
        class ProcessStateChangeCallback {
            public:
                virtual void run(Process* proc) = 0;
                virtual ~ProcessStateChangeCallback() {}
        };

    private:
        //holds the process id
        int procId;
        //holds the time the cpu will be running
        int totalCpuTime;
        //the amount of time the cpu will be utilized before blocking
        int cpuTimeBeforeBlock;
        //the amount of cpu time that will be utilized
        int cpuTimeRemaining;
        //holds the time the process will block
        int totalIoTime;
        //the amount of time the process will remain blocked
        int ioTimeRemaining;
        //the cycle the process will arrive
        int arrivalCycle;
        //the cycle that the process started running
        int startCycle;
        //the cycle that the process finishes
        int finishCycle;
        //the state of the process
        ProcessState state;

        //pointers to callbacks that will be used in Scheduler
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
            //corner case if the cpu time is 1
            if(totalCpuTime == 1) {
                cpuTimeBeforeBlock = 1;
                cpuTimeRemaining = 2;
            }
            else {
                //corner case if there is no blocking time and the cpu time is odd
                if(totalIoTime == 0 && totalCpuTime % 2 != 0) {
                    //cpu time does not get an extra cycle to run since it doesn't block
                    cpuTimeBeforeBlock = totalCpuTime;
                }
                else {
                    //otherwise set the block time to be half of the total time
                    //use ceil to round it to the next cycle
                    cpuTimeBeforeBlock = ceil(totalCpuTime / 2);
                }
                cpuTimeRemaining = totalCpuTime;
            }

            ioTimeRemaining = totalIoTime;

            //lets us know that startCycle and finishCycle haven't been initialized yet
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

        //calculates the process's turnaround time
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

//this struct is provided to sorting algorithms
//and will sort a list of Process so that the lower
//the process id, the higher they will end up in the list
struct LowerProcessIdPriority {
    inline bool operator() (const Process* p1, const Process* p2) {
        return p1 > p2;
    }
};


//this struct is provided to sorting algorithms
//and will sort a list of Process so that the higher
//the process id, the higher they will end up in the list
struct HigherProcessIdPriority {
    inline bool operator() (const Process* p1, const Process* p2) {
        return p1 < p2;
    }
};

#endif // PROCESS_H
