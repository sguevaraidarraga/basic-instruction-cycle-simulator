#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <string>
#include "memory/Memory.h"
#include "cpu/CPU.h"

class Simulator {
    std::string filename;
    Memory mem;
    CPU cpu;

    static void trim(std::string &s);

public:
    Simulator(const std::string &programFile);
    void run();
};

#endif