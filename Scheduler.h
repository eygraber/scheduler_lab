#ifndef SCHEDULER_H
#define SCHEDULER_H

#include<queue>
#include<algorithm>
#include<vector>
#include<iostream>

#include "Process.h"

//forward declaration
class Process;

enum AlgorithmTypes { FCFS = 0, RR = 1, SRJF = 2 };

class Scheduler {
    private:
        //forward declaration of abstract algorithm class
        class SchedulingAlgorithm {
            friend class Scheduler;
            virtual void run() = 0;
            virtual void onProcessArrivedCallback(Scheduler* s, Process* proc) = 0;
            virtual void onProcessReadyCallback(Scheduler* s, Process* proc) = 0;
            virtual void onProcessRunningCallback(Scheduler* s, Process* proc) = 0;
            virtual void onProcessBlockedCallback(Scheduler* s, Process* proc) = 0;
            virtual void onProcessFinishedCallback(Scheduler* s, Process* proc) = 0;
        };

        //forward declarations of algorithm implementations
        class FCFSAlgorithm;
        class RRAlgorithm;
        class SRJFAlgorithm;

        //making these friends classes gives access to SchedulingAlgorithm
        friend class FCFSAlgorithm;
        friend class RRAlgorithm;
        friend class SRJFAlgorithm;

        SchedulingAlgorithm* alg;
        SchedulingAlgorithm* getSchedulingAlgorithm(AlgorithmTypes algorithmType);

        queue<Process*>* readyQ;
        vector<Process*>* movedToReadyState;
        vector<Process*>* allProcesses;

        vector<Process*>::iterator it;

        int currentCycle;
        int cpuUtilizationCyclesCount;

        int processesCount;
        int finishedProcessesCount;

        int getCurrentCycle() { return currentCycle; }
        int getCpuUtilizationCyclesCount() { return cpuUtilizationCyclesCount; }

        int getProcessesCount() { return processesCount; }
        int getFinishedProcessesCount() { return finishedProcessesCount; }
        void incrementFinishedProcessesCount() { finishedProcessesCount++; }

        double getCpuUtilization() { return (double)getCpuUtilizationCyclesCount() / (double)getCurrentCycle(); }

        void incrementCycle() { currentCycle++; }
        void incrementCpuUtilizationCyclesCount() { cpuUtilizationCyclesCount++; }

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
