#ifndef CONTROL_UNIT_H
#define CONTROL_UNIT_H

class ControlUnit {
    bool running;

public:
    ControlUnit();
    bool isRunning() const;
    void halt();
};

#endif