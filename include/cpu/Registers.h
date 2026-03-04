#ifndef REGISTERS_H
#define REGISTERS_H

#include <string>

struct Registers {
    int PC = 0;
    int ACC = 0;
    int MAR = 0;
    std::string MDR;
    std::string ICR;
};

#endif