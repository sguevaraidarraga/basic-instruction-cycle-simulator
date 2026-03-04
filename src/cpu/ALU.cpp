#include "cpu/ALU.h"

ALU::ALU() {
    left = 0;
}

void ALU::pushLeft(int v) {
    left = v;
}
int ALU::addWith(int right) {
    return left + right;
}

int ALU::add(int a, int b) {
    return a + b;
}