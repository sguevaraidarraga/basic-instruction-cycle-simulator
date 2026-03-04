#ifndef CPU_H
#define CPU_H

#include "memory/Memory.h"
#include "cpu/Registers.h"
#include "cpu/ControlUnit.h"
#include "cpu/ALU.h"
#include <string>

class CPU {
    Memory &memory;
    Registers registers;
    ControlUnit control;
    ALU alu;
    std::string currentInstruction;
    std::string parsedInstr, op1, op2, op3;
    std::string op2_raw;

    static std::string toUpper(const std::string &s);
    static int parseAddress(const std::string &token);
    static bool isNumber(const std::string &s);
    void showAccumulator() const;
    void showPC() const;
    void showICR() const;
    void showMAR() const;
    void showMDR() const;
    void showControlUnit() const;
    void showMemory(int addr) const;

public:
    CPU(Memory &m);
    void fetchStage(int &pc);
    void decodeStage();
    void executeStage();
    void nextInstructionStage();
    const Registers &getRegisters() const;
    const ControlUnit &getControlUnit() const;
};

#endif