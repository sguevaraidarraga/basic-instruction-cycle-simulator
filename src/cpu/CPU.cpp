#include "cpu/CPU.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <limits>

CPU::CPU(Memory &m) : memory(m) {}

void CPU::fetchStage(int &pc) {
    registers.PC = pc;
    registers.MAR = registers.PC;
    registers.MDR = memory.readRaw(registers.MAR);
    registers.ICR = registers.MDR;
    std::cout << "[FETCH] PC=" << registers.PC
         << " MAR=" << registers.MAR
         << " MDR=\"" << registers.MDR
         << "\" ICR=\"" << registers.ICR << "\"" << std::endl;
    pc += 1;
    registers.PC = pc;
}

void CPU::decodeStage() {
    parsedInstr.clear();
    op1.clear();
    op2.clear();
    op3.clear();
    op2_raw.clear();
    
    std::string t0, t1, t2, t3;
    std::stringstream ss(registers.ICR);
    ss >> t0 >> t1 >> t2 >> t3;
    parsedInstr = toUpper(t0);
    op1 = toUpper(t1);
    op2 = toUpper(t2);
    op3 = toUpper(t3);
    op2_raw = t2;
    std::cout << "[DECODE] PC=" << registers.PC << " ICR=\"" << registers.ICR << "\" -> instr=" << parsedInstr
            << " op1=" << op1 << " op2=" << op2 << " op3=" << op3 << std::endl;
    std::cout << "control unit reads ICR -> \"" << registers.ICR << "\"" << std::endl;
}

void CPU::executeStage() {
    int a = parseAddress(op1);
    int b = parseAddress(op2);
    int c = parseAddress(op3);

    std::cout << "[EXECUTE] ";

    if(parsedInstr == "SET") {
        if(a >= 0) {
            if(isNumber(op2_raw)) {
                registers.MAR = a;
                memory.writeRaw(a, op2_raw);
                registers.MDR = memory.readRaw(a);
                std::cout << "SET D" << a << " = " << registers.MDR << std::endl;
            } else if(b >= 0) {
                registers.MAR = b;
                registers.MDR = memory.readRaw(b);
                registers.MAR = a;
                memory.writeRaw(a, registers.MDR);
                std::cout << "SET D" << a << " = D" << b << " (" << registers.MDR << ")" << std::endl;
            } else {
                std::cout << "SET ignored (invalid operands)" << std::endl;
            }
        }
    } else if(parsedInstr == "LDR" || parsedInstr == "LOAD") {
        if(a >= 0) {
            std::cout << "control unit executing: " << parsedInstr << std::endl;
            registers.MAR = a;
            registers.MDR = memory.readRaw(a);
            registers.ACC = memory.readIntFromRaw(registers.MDR);
            std::cout << "MAR=" << registers.MAR << " MDR=\"" << registers.MDR << "\" ACC=" << registers.ACC << std::endl;
        }
    } else if(parsedInstr == "ADD") {
        if(a >= 0 && op2 == "NULL") {
            std::cout << "control unit executing: ADD with accumulator and D" << a << std::endl;
            registers.MAR = a;
            registers.MDR = memory.readRaw(a);
            int val = memory.readIntFromRaw(registers.MDR);
            std::cout << "MAR=" << registers.MAR << " MDR=\"" << registers.MDR << "\" val=" << val << " ACC(before)=" << registers.ACC << std::endl;
            int res = alu.add(registers.ACC, val);
            registers.ACC = res;
            std::cout << "ACC(after)=" << registers.ACC << std::endl;
        } else if(a >= 0 && b >= 0 && op3 == "NULL") {
            std::cout << "control unit executing: ADD D" << a << " + D" << b << " -> ACC" << std::endl;
            registers.MAR = a; registers.MDR = memory.readRaw(a); int v1 = memory.readIntFromRaw(registers.MDR);
            registers.MAR = b; registers.MDR = memory.readRaw(b); int v2 = memory.readIntFromRaw(registers.MDR);
            int res = alu.add(v1, v2);
            registers.ACC = res; registers.MDR = std::to_string(res);
            std::cout << "v1=" << v1 << " v2=" << v2 << " ACC=" << registers.ACC << std::endl;
        } else if(a >= 0 && b >= 0 && c >= 0) {
            std::cout << "control unit executing: ADD D" << a << " + D" << b << " -> D" << c << std::endl;
            registers.MAR = a; registers.MDR = memory.readRaw(a); int v1 = memory.readIntFromRaw(registers.MDR);
            registers.MAR = b; registers.MDR = memory.readRaw(b); int v2 = memory.readIntFromRaw(registers.MDR);
            int res = alu.add(v1, v2);
            memory.writeRaw(c, std::to_string(res));
            registers.MAR = c; registers.MDR = memory.readRaw(c);
            std::cout << "D" << c << "=" << registers.MDR << std::endl;
        }
    } else if(parsedInstr == "SUB") {
        if(a >= 0 && op2 == "NULL") {
            std::cout << "control unit executing: SUB with accumulator and D" << a << std::endl;
            registers.MAR = a;
            registers.MDR = memory.readRaw(a);
            int val = memory.readIntFromRaw(registers.MDR);
            std::cout << "MAR=" << registers.MAR << " MDR=\"" << registers.MDR << "\" val=" << val << " ACC(before)=" << registers.ACC << std::endl;
            int res = registers.ACC - val;
            registers.ACC = res;
            std::cout << "ACC(after)=" << registers.ACC << std::endl;
        } else if(a >= 0 && b >= 0 && op3 == "NULL") {
            std::cout << "control unit executing: SUB D" << a << " - D" << b << " -> ACC" << std::endl;
            registers.MAR = a; registers.MDR = memory.readRaw(a); int v1 = memory.readIntFromRaw(registers.MDR);
            registers.MAR = b; registers.MDR = memory.readRaw(b); int v2 = memory.readIntFromRaw(registers.MDR);
            int res = v1 - v2;
            registers.ACC = res; registers.MDR = std::to_string(res);
            std::cout << "v1=" << v1 << " v2=" << v2 << " ACC=" << registers.ACC << std::endl;
        } else if(a >= 0 && b >= 0 && c >= 0) {
            std::cout << "control unit executing: SUB D" << a << " - D" << b << " -> D" << c << std::endl;
            registers.MAR = a; registers.MDR = memory.readRaw(a); int v1 = memory.readIntFromRaw(registers.MDR);
            registers.MAR = b; registers.MDR = memory.readRaw(b); int v2 = memory.readIntFromRaw(registers.MDR);
            int res = v1 - v2;
            memory.writeRaw(c, std::to_string(res));
            registers.MAR = c; registers.MDR = memory.readRaw(c);
            std::cout << "D" << c << "=" << registers.MDR << std::endl;
        }
    } else if(parsedInstr == "INC") {
        if(a >= 0) {
            registers.MAR = a;
            registers.MDR = memory.readRaw(a);
            int val = memory.readIntFromRaw(registers.MDR) + 1;
            memory.writeRaw(a, std::to_string(val));
            registers.MDR = memory.readRaw(a);
            std::cout << "INC D" << a << " -> " << registers.MDR << std::endl;
        }
    } else if(parsedInstr == "DEC") {
        if(a >= 0) {
            registers.MAR = a;
            registers.MDR = memory.readRaw(a);
            int val = memory.readIntFromRaw(registers.MDR) - 1;
            memory.writeRaw(a, std::to_string(val));
            registers.MDR = memory.readRaw(a);
            std::cout << "DEC D" << a << " -> " << registers.MDR << std::endl;
        }
    } else if(parsedInstr == "STR") {
        if(a >= 0) {
            registers.MAR = a;
            memory.writeRaw(a, std::to_string(registers.ACC));
            registers.MDR = memory.readRaw(a);
            std::cout << "STR ACC -> D" << a << " = " << registers.MDR << std::endl;
        }
    } else if(parsedInstr == "SHW") {
        if(op1 == "ACC") showAccumulator();
        else if(op1 == "PC") showPC();
        else if(op1 == "ICR") showICR();
        else if(op1 == "MAR") showMAR();
        else if(op1 == "MDR") showMDR();
        else if(op1 == "UC") showControlUnit();
        else if(a >= 0) showMemory(a);
    } else if(parsedInstr == "PAUSE") {
        std::cout << "Execution paused. Press Enter to continue..." << std::endl;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    } else if(parsedInstr == "END") {
        control.halt();
        std::cout << "END encountered. Halting." << std::endl;
    } else {
        std::cout << "Unknown instruction: " << parsedInstr << std::endl;
    }
}

void CPU::nextInstructionStage() {
    std::cout << "[NEXT]" << std::endl << std::endl;
}

const Registers& CPU::getRegisters() const {
    return registers;

}

const ControlUnit& CPU::getControlUnit() const {
    return control;
}

std::string CPU::toUpper(const std::string &s) {
    std::string r = s;
    transform(r.begin(), r.end(), r.begin(), ::toupper);
    return r;
}

int CPU::parseAddress(const std::string &token) {
    if(token.size() >= 2 && (token[0] == 'D' || token[0] == 'd')) {
        try {
            return stoi(token.substr(1));
        } catch (...) {
            return -1;
        }
    }
    return -1;
}

bool CPU::isNumber(const std::string &s) {
    if(s.empty()) return false;
    int i = 0; if (s[0] == '+' || s[0] == '-') i = 1;
    for(; i < s.size(); i++) {
        if(!isdigit((unsigned char)s[i])) {
            return false;
        }
    }
    return true;
}

void CPU::showAccumulator() const {
    std::cout << "ACC: " << registers.ACC << std::endl;
}

void CPU::showPC() const {
    std::cout << "PC: " << registers.PC << std::endl;
}

void CPU::showICR() const {
    std::cout << "ICR: " << registers.ICR << std::endl;
}

void CPU::showMAR() const {
    std::cout << "MAR: " << registers.MAR << std::endl;
}

void CPU::showMDR() const {
    std::cout << "MDR: " << registers.MDR << std::endl;
}
void CPU::showControlUnit() const {
    std::cout << "Control Unit: " << (control.isRunning() ? "RUNNING" : "HALTED") << std::endl;
}

void CPU::showMemory(int addr) const {
    std::cout << "D" << addr << ": " << memory.readRaw(addr) << std::endl;
}