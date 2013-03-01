#include "InputHandler.h"
#include<iostream>
#include<cstdlib>

vector<Process*>* InputHandler::parseInput(char* path) {
    ifstream inputFile(path);

    vector<Process*>* processes = new vector<Process*>();

    while(!inputFile.eof()) {
        int procId = parseData(&inputFile);
        int cpuTime = parseData(&inputFile);
        int ioTime = parseData(&inputFile);
        int arrivalCycle = parseData(&inputFile);
        if(procId < 0 || cpuTime < 0 || ioTime < 0 || arrivalCycle < 0) {
            return NULL;
        }
        processes->push_back(new Process(procId, cpuTime, ioTime, arrivalCycle)); 
    }

    inputFile.close();

    return processes;
}

int InputHandler::parseData(ifstream* s) {
    char c;
    string str = "";
    do {
        if(!s->good()) {
            return -1;
        }
        c = s->get();
    } while(c == ' ' || c == '\n');

    while(c != ' ' && c != '\n') {
        str += c;

        if(!s->good()){
            break; 
        }

        c = s->get();
    }

    while(c == ' ' || c == '\n') {
        c = s->get();

        if(!s->good()) {
            return atoi(str.c_str());
        } 
    }

    s->unget();

    return atoi(str.c_str());
}
