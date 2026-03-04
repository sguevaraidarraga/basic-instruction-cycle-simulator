#ifndef ALU_H
#define ALU_H

class ALU {
    int left;

public:
    ALU();
    void pushLeft(int v);
    int addWith(int right);
    int add(int a, int b);
};

#endif