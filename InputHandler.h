#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include<fstream>
#include<vector>

#include "Process.h"

class InputHandler {
    private:
        int parseData(ifstream* s);
    public:
        vector<Process*>* parseInput(char* path);
};

#endif //INPUTHANDLER_H
