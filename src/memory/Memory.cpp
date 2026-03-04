#include "memory/Memory.h"
#include "utils/Constants.h"

Memory::Memory() {
    data = std::vector<std::string>(Constants::MEMORY_SIZE, "0");
}

std::string Memory::readRaw(int address) const {
    if(address >= 0 && address < Constants::MEMORY_SIZE) {
        return data[address];
    } else {
        return "0";
    }
}

void Memory::writeRaw(int address, std::string text) {
    if(address >= 0 && address < Constants::MEMORY_SIZE) {
        data[address] = text;
    }
}

int Memory::readIntFromRaw(const std::string &raw) {
    if(raw.empty()) return 0;
    
    int i = 0;
    if(raw[0] == '+' || raw[0] == '-') {
        i = 1;
    }
    
    for(; i < raw.size(); ++i) {
        if(!isdigit((unsigned char)raw[i])) return 0;
    }
    
    try {
        return stoi(raw);
    } catch(...) {
        return 0;
    }
}

int Memory::readInt(int addr) const {
    return readIntFromRaw(readRaw(addr));
}