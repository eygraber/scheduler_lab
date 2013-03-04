#ifndef SCHEDULER_H
#define SCHEDULER_H

#include<queue>
#include<algorithm>
#include<vector>
#include<iostream>

#include "Process.h"

//forward declaration
class Process;

//provides an enum for the types of algorithms
enum AlgorithmTypes { FCFS = 0, RR = 1, SRJF = 2 };

class Scheduler {
    private:
        //abstract algorithm class
        class SchedulingAlgorithm {
            //gives Scheduler access to SchedulingAlgorithm's private members
            friend class Scheduler;

            //runs the algorithm
            virtual void run() = 0;

            //defines callbacks for changes in process state
            virtual void onProcessArrivedCallback(Scheduler* s, Process* proc) = 0;
            virtual void onProcessReadyCallback(Scheduler* s, Process* proc) = 0;
            virtual void onProcessRunningCallback(Scheduler* s, Process* proc) = 0;
            virtual void onProcessBlockedCallback(Scheduler* s, Process* proc) = 0;
            virtual void onProcessFinishedCallback(Scheduler* s, Process* proc) = 0;

            virtual ~SchedulingAlgorithm() {}
        };

        //forward declarations of algorithm implementations
        class FCFSAlgorithm;
        class RRAlgorithm;
        class SRJFAlgorithm;

        //making these friends classes gives access to SchedulingAlgorithm
        friend class FCFSAlgorithm;
        friend class RRAlgorithm;
        friend class SRJFAlgorithm;

        //pointer to an abstract SchedulingAlgorithm
        SchedulingAlgorithm* alg;
        //returns a pointer to a concrete implementation of a SchedulingAlgorithm
        SchedulingAlgorithm* getSchedulingAlgorithm(AlgorithmTypes algorithmType);


        //hold processes that are in the ready state
        queue<Process*>* readyQ;
        //holds processes that are going to be ready to
        //be moved to the readyQ at the end of the cycle
        vector<Process*>* movedToReadyState;
        //holds all processes
        vector<Process*>* allProcesses;

        //used as an iterator in the algorithm implementations
        vector<Process*>::iterator it;

        //holds the cycle the scheduler is up to
        int currentCycle;
        //how many cycles the cpu has been utilized
        int cpuUtilizationCyclesCount;

        //how many processes are there
        int processesCount;
        //how many processes have finished
        int finishedProcessesCount;

        int getCurrentCycle() { return currentCycle; }
        int getCpuUtilizationCyclesCount() { return cpuUtilizationCyclesCount; }

        int getProcessesCount() { return processesCount; }
        int getFinishedProcessesCount() { return finishedProcessesCount; }
        void incrementFinishedProcessesCount() { finishedProcessesCount++; }

        //calculates the cpu utilization
        double getCpuUtilization() { return (double)getCpuUtilizationCyclesCount() / (double)getCurrentCycle(); }

        void incrementCycle() { currentCycle++; }
        void incrementCpuUtilizationCyclesCount() { cpuUtilizationCyclesCount++; }

        //the following callbacks are used in the algorithm implementations
        //and set the callbacks in Process
        class OnProcessArrivedCallback : public Process::ProcessStateChangeCallback {
            private:
                Scheduler* s;
            public:
                OnProcessArrivedCallback(Scheduler* s) : s(s) {}
                void run(Process* proc) { s->alg->onProcessArrivedCallback(s, proc); }
        };

        class OnProcessReadyCallback : public Process::ProcessStateChangeCallback {
            private:
                Scheduler* s;
            public:
                OnProcessReadyCallback(Scheduler* s) : s(s) {}
                void run(Process* proc) { s->alg->onProcessReadyCallback(s, proc); }
        };

        class OnProcessRunningCallback : public Process::ProcessStateChangeCallback {
            private:
                Scheduler* s;
            public:
                OnProcessRunningCallback(Scheduler* s) : s(s) {}
                void run(Process* proc) { s->alg->onProcessRunningCallback(s, proc); }
        };

        class OnProcessBlockedCallback : public Process::ProcessStateChangeCallback {
            private:
                Scheduler* s;
            public:
                OnProcessBlockedCallback(Scheduler* s) : s(s) {}
                void run(Process* proc) { s->alg->onProcessBlockedCallback(s, proc); }
        };

        class OnProcessFinishedCallback : public Process::ProcessStateChangeCallback {
            private:
                Scheduler* s;
            public:
                OnProcessFinishedCallback(Scheduler* s) : s(s) {}
                void run(Process* proc) { s->alg->onProcessFinishedCallback(s, proc); }
        };

    public:
        Scheduler(AlgorithmTypes algorithmType, vector<Process*>* allProcesses)
            : alg(getSchedulingAlgorithm(algorithmType)), allProcesses(allProcesses) {

            //sorts the processes so that the lower the process id
            //the lower the process will be in the vector
            sort(allProcesses->begin(), allProcesses->end(), HigherProcessIdPriority());

            readyQ = new queue<Process*>();
            movedToReadyState = new vector<Process*>();

            currentCycle = 0;
            cpuUtilizationCyclesCount = 0;

            processesCount = allProcesses->size();
            finishedProcessesCount = 0;

            /*for(it = allProcesses->begin(); it != allProcesses->end(); it++) {
                (*it)->setOnArrivedCallback(new OnProcessArrivedCallback(this));
                (*it)->setOnReadyCallback(new OnProcessReadyCallback(this));
                (*it)->setOnBlockedCallback(new OnProcessBlockedCallback(this));
                (*it)->setOnFinishedCallback(new OnProcessFinishedCallback(this));
                (*it)->cycle(0);
            }*/
        }

        void startScheduler() {
            //runs the algorithm implementation
            alg->run();
        }

        void printStatistics();

        ~Scheduler() {
            delete alg;
            delete readyQ;
            delete movedToReadyState;
        }
};

#endif // SCHEDULER_H
