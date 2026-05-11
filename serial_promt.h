#pragma once

#include <Arduino.h>

struct HelloWorldConfig {
    bool helloWorldRunning;
    int ledGpio;
    bool invertLed;
    int dit;
};

void initSerialPromt(const HelloWorldConfig &config);
bool handleSerialPromt(HelloWorldConfig &config);
