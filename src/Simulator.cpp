#include "Simulator.h"
#include "utils/Constants.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>

Simulator::Simulator(const std::string &programFile) : filename(programFile), mem(), cpu(mem) {}

void Simulator::trim(std::string &s) {
    auto a = s.find_first_not_of(" \t\r\n");
    if(a == std::string::npos) {
        s.clear();
        return;
    }
    auto b = s.find_last_not_of(" \t\r\n");
    s = s.substr(a, b - a + 1);
}

void Simulator::run() {
    std::ifstream file(Constants::FILES_PATH + filename);
    if(!file) {
        std::cerr << "Cannot open input file: " << filename << std::endl;
        return;
    }

    std::string line;
    int startAddr = 100;
    int loadAddr = startAddr;
    int lastAddr = -1;
    while(getline(file, line)) {
        trim(line);
        if(line.empty() || line[0] == '#') continue;
        std::string tag;
        int val;
        std::stringstream ss(line);
        ss >> tag >> val;
        std::string up = tag;
        transform(up.begin(), up.end(), up.begin(), ::toupper);
        if((up == "START" || up == "PC") && val >= 0) {
            startAddr = val;
            loadAddr = startAddr;
            continue;
        }
        mem.writeRaw(loadAddr, line);
        lastAddr = loadAddr;
        loadAddr++;
    }

    int pc = startAddr;
    if(lastAddr < pc) {
        std::cerr << "No program loaded." << std::endl;
        return;
    }

    while(pc <= lastAddr && cpu.getControlUnit().isRunning()) {
        cpu.fetchStage(pc);
        cpu.decodeStage();
        cpu.executeStage();
        cpu.nextInstructionStage();
    }
}