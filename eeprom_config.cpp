#include "./eeprom_config.h"

#include <EEPROM.h>

namespace {
constexpr int EEPROM_CONFIG_ADDRESS = 0;
constexpr uint32_t EEPROM_CONFIG_MAGIC = 0x48475731;  // "HGW1"
constexpr uint16_t EEPROM_CONFIG_VERSION = 2;

struct PersistedHelloWorldConfig {
  uint32_t magic;
  uint16_t version;
  uint8_t helloWorldRunning;
  int32_t ledGpio;
  uint8_t invertLed;
  int32_t dit;
  char text[32];
};

bool configsEqual(const HelloWorldConfig &left, const HelloWorldConfig &right) {
  return left.helloWorldRunning == right.helloWorldRunning
      && left.ledGpio == right.ledGpio
      && left.invertLed == right.invertLed
      && left.dit == right.dit
      && left.text == right.text;
}

bool isValidLedGpio(int ledGpio) {
  return ledGpio >= 0 && ledGpio <= 40;
}

bool isValidDit(int dit) {
  return dit >= 20 && dit <= 2000;
}

bool loadConfigFromEeprom(HelloWorldConfig &config) {
  PersistedHelloWorldConfig persisted = {};
  EEPROM.get(EEPROM_CONFIG_ADDRESS, persisted);

  if (persisted.magic != EEPROM_CONFIG_MAGIC || persisted.version != EEPROM_CONFIG_VERSION) {
    return false;
  }

  int loadedGpio = static_cast<int>(persisted.ledGpio);
  if (!isValidLedGpio(loadedGpio)) {
    return false;
  }

  int loadedDit = static_cast<int>(persisted.dit);
  if (!isValidDit(loadedDit)) {
    return false;
  }

  config.helloWorldRunning = persisted.helloWorldRunning != 0;
  config.ledGpio = loadedGpio;
  config.invertLed = persisted.invertLed != 0;
  config.dit = loadedDit;
  config.text = String(persisted.text);
  return true;
}

bool saveConfigToEeprom(const HelloWorldConfig &config) {
  PersistedHelloWorldConfig persisted = {};
  persisted.magic = EEPROM_CONFIG_MAGIC;
  persisted.version = EEPROM_CONFIG_VERSION;
  persisted.helloWorldRunning = static_cast<uint8_t>(config.helloWorldRunning ? 1 : 0);
  persisted.ledGpio = static_cast<int32_t>(config.ledGpio);
  persisted.invertLed = static_cast<uint8_t>(config.invertLed ? 1 : 0);
  persisted.dit = static_cast<int32_t>(config.dit);
  strncpy(persisted.text, config.text.c_str(), sizeof(persisted.text) - 1);
  persisted.text[sizeof(persisted.text) - 1] = '\0';

  EEPROM.put(EEPROM_CONFIG_ADDRESS, persisted);
  return EEPROM.commit();
}
}  // namespace

void initConfigFromEeprom(HelloWorldConfig &config, int defaultLedGpio, bool defaultInvertLed,
  int defaultDit, const char *defaultText) {
  EEPROM.begin(EEPROM_SIZE_BYTES);

  if (loadConfigFromEeprom(config)) {
    Serial.println("[config] Loaded configuration from EEPROM");
    return;
  }

  config.helloWorldRunning = true;
  config.ledGpio = defaultLedGpio;
  config.invertLed = defaultInvertLed;
  config.dit = defaultDit;
  config.text = String(defaultText);
  if (saveConfigToEeprom(config)) {
    Serial.println("[config] Stored default configuration to EEPROM");
  } else {
    Serial.println("[config] Failed to store default configuration to EEPROM");
  }
}

bool persistConfigIfChanged(const HelloWorldConfig &previousConfig, const HelloWorldConfig &currentConfig) {
  if (configsEqual(previousConfig, currentConfig)) {
    return false;
  }

  if (saveConfigToEeprom(currentConfig)) {
    Serial.println("[config] Saved configuration to EEPROM");
  } else {
    Serial.println("[config] Failed to save configuration to EEPROM");
  }

  return true;
}
