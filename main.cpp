#include "Process.h"
#include "Scheduler.h"
#include "InputHandler.h"

#include<iostream>
#include<fstream>
#include<sstream>
#include<cstring>

#include<vector>

using namespace std;

char* getFileNameWithoutExtension(char* name);

int main(int argc, char** argv) {
    streambuf* coutbuf = cout.rdbuf();

    InputHandler ih;

    for(int i = 1; i < argc; i++) {
        for(int algType = FCFS; algType <= SRJF; algType++) {
            ostringstream oss;
            oss << getFileNameWithoutExtension(argv[i]) << "-" << algType << ".txt";
            ofstream out(oss.str().c_str());
            cout.rdbuf(out.rdbuf());

            vector<Process*>* processes = ih.parseInput(argv[i]);
            if(processes == NULL) {
                cout << "There was an error parsing the input for " << argv[i] << "!" << endl;
                continue;
            }

            Scheduler* s = new Scheduler((AlgorithmTypes)algType, processes);
            s->startScheduler();
            s->printStatistics();
            delete s;

            vector<Process*>::iterator it;
            for(it = processes->begin(); it != processes->end(); it++) {
                delete (*it);
            }
            delete processes;

            cout.rdbuf(coutbuf);
        }
    }

    cout.rdbuf(coutbuf);

    return 0;
}

char* getFileNameWithoutExtension(char* name) {
    int dotPos = -1;
    for(int i = 0; name[i] != '\0'; i++) {
        if(name[i] == '.') {
            dotPos = i;
        }
    }

    if(dotPos < 0) {
        return name;
    }
    else {
        char* newName = new char[dotPos + 1];
	strncpy(newName, name, dotPos);
        newName[dotPos] = '\0';
        return newName;
    }
}
