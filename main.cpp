#include "Process.h"
#include "Scheduler.h"

#include<vector>

using namespace std;

int main() {
    vector<Process*>* processes = new vector<Process*>();

    /*processes->push_back(new Process(0, 2, 2, 0));
    processes->push_back(new Process(1, 2, 1, 5));
    processes->push_back(new Process(2, 2, 1, 3));*/

    processes->push_back(new Process(0, 2, 5, 10));
    processes->push_back(new Process(1, 2, 7, 0));
    processes->push_back(new Process(2, 4, 2, 5));
    processes->push_back(new Process(3, 4, 0, 6));

    Scheduler* s = new Scheduler(SRJF, processes);
    s->startScheduler();
    s->printStatistics();

    delete s;

    return 0;
}
