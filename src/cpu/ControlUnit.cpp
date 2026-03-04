#include "cpu/ControlUnit.h"

ControlUnit::ControlUnit() {
    running = true;
}

bool ControlUnit::isRunning() const {
    return running;
}

void ControlUnit::halt() {
    running = false;
}