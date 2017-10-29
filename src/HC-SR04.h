#pragma once

#include <inttypes.h>

class HCSR04 {
public:
    HCSR04(uint8_t trigger, uint8_t echo);

    unsigned int distance(int maxDistance = 400);
    unsigned int medianDistance(int maxDistance = 400, int numPing = 7);

private:
    uint8_t trigger_pin, echo_pin;
};
