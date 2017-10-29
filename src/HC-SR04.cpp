#include "HC-SR04.h"
#include <Arduino.h>

namespace {

inline void split(unsigned int* a, int n, float x, int& i, int& j)
{
    do {
        while (a[i] < x)
            i++;
        while (x < a[j])
            j--;
        if (i <= j) {
            const unsigned int t = a[i];
            a[i] = a[j];
            a[j] = t;
            i++;
            j--;
        }
    } while (i <= j);
}

unsigned int median(unsigned int* a, int n)
{
    int L = 0;
    int R = n - 1;
    int k = n / 2;
    while (L < R) {
        unsigned int x = a[k];
        int i = L;
        int j = R;
        split(a, n, x, i, j);
        if (j < k)
            L = i;
        if (k < i)
            R = j;
    }
    return a[k];
}
} // anonymous namespace

HCSR04::HCSR04(uint8_t trigger, uint8_t echo)
    : trigger_pin(trigger)
    , echo_pin(echo)
{
    pinMode(trigger_pin, OUTPUT);
    pinMode(echo_pin, INPUT);
}

unsigned int HCSR04::distance(int maxDistance)
{
    // If the same pin is used to read the signal from the PING)))
    if (trigger_pin == echo_pin)
        pinMode(trigger_pin, OUTPUT);

    // Reset the sensor.
    // Some sensors are stuck when there is no echo.
    digitalWrite(trigger_pin, LOW);
    delayMicroseconds(2);

    // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
    digitalWrite(trigger_pin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigger_pin, LOW);

    // If the same pin is used to read the signal from the PING)))
    if (trigger_pin == echo_pin)
        pinMode(trigger_pin, INPUT);

    // Sound speed in air is aprox. 343m/s and need 29.1microsecond/cm.
    const unsigned long duration = pulseIn(echo_pin, HIGH, 2 * 29.1f * maxDistance);

    // If we timed out (no echo)
    if (duration == 0)
        return maxDistance;

    return duration * 0.0343f / 2;
}

unsigned int HCSR04::medianDistance(int maxDistance, int numPing)
{
    unsigned int measures[numPing];
    for (int i = 0; i < numPing; ++i) {
        measures[i] = distance(maxDistance);
    }
    return median(measures, numPing);
}
