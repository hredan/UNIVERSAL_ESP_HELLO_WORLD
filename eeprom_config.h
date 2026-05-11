#pragma once

#include <Arduino.h>
#include "./serial_promt.h"

constexpr int EEPROM_SIZE_BYTES = 64;

void initConfigFromEeprom(HelloWorldConfig &config, int defaultLedGpio, bool defaultInvertLed, int defaultDit);
bool persistConfigIfChanged(const HelloWorldConfig &previousConfig, const HelloWorldConfig &currentConfig);
