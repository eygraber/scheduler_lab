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
    //saves stdout's buffer so we can restore it later
    streambuf* coutbuf = cout.rdbuf();

    InputHandler ih;

    for(int i = 1; i < argc; i += 2) {
        //gets the type of algorithm to use from the parameters
        int algType = atoi(argv[i + 1]);

        //processes the input file and builds a vector of processes
        vector<Process*>* processes = ih.parseInput(argv[i]);
        if(processes == NULL) {
            cout << "There was an error parsing the input for " << argv[i] << "!" << endl;
            continue;
        }

        ostringstream oss;
        //strips the extension off the filename and builds the output file name
        oss << getFileNameWithoutExtension(argv[i]) << "-" << algType << ".txt";
        ofstream out(oss.str().c_str());
        //sets cout's buffer to the output file
        cout.rdbuf(out.rdbuf());

        //creates and runs the scheduler on the vector of processes
        //based on the algorithm type
        Scheduler* s = new Scheduler((AlgorithmTypes)algType, processes);
        s->startScheduler();
        s->printStatistics();
        delete s;

        //deletes the process pointers
        vector<Process*>::iterator it;
        for(it = processes->begin(); it != processes->end(); it++) {
            delete (*it);
        }
        delete processes;

        //gives cout its buffer back so we can print any errors
        cout.rdbuf(coutbuf);

        /*for(int algType = FCFS; algType <= SRJF; algType++) {
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
        }*/
    }

    //gives cout its buffer back
    cout.rdbuf(coutbuf);

    return 0;
}

char* getFileNameWithoutExtension(char* name) {
    int dotPos = -1;
    for(int i = 0; name[i] != '\0'; i++) {
        if(name[i] == '.') {
            //if another dot is found
            //store its index
            dotPos = i;
        }
    }

    if(dotPos < 0) {
        return name;
    }
    else {
        //allocates a new array for the stripped filename
        char* newName = new char[dotPos + 1];
        strncpy(newName, name, dotPos);
        newName[dotPos] = '\0';
        return newName;
    }
}
