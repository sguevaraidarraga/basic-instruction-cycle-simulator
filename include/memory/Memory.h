#ifndef MEMORY_H
#define MEMORY_H

#include <vector>
#include <string>

class Memory {
    std::vector<std::string> data;

public:
    Memory(); 
    std::string readRaw(int address) const;
    void writeRaw(int address, const std::string value);
    static int readIntFromRaw(const std::string &raw);
    int readInt(int address) const;
};

#endif