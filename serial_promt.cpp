#include "serial_promt.h"

namespace {
String serialInput;

void ledOnForTimespan(int ledGpio, bool invertLed, int timespan, int symbolSpace) {
  if (invertLed) {
    digitalWrite(ledGpio, LOW);
  } else {
    digitalWrite(ledGpio, HIGH);
  }
  delay(timespan);

  if (invertLed) {
    digitalWrite(ledGpio, HIGH);
  } else {
    digitalWrite(ledGpio, LOW);
  }
  delay(symbolSpace);
}

void outputMorseR(int ledGpio, bool invertLed, int dit) {
  const int dah = 3 * dit;
  const int symbolSpace = dit;
  const int letterSpace = 3 * dit;

  // Morse for R is .-.
  ledOnForTimespan(ledGpio, invertLed, dit, symbolSpace);
  ledOnForTimespan(ledGpio, invertLed, dah, symbolSpace);
  ledOnForTimespan(ledGpio, invertLed, dit, symbolSpace);
  delay(letterSpace);
}

bool isUnsignedNumber(const String &value) {
  if (value.length() == 0) {
    return false;
  }

  for (size_t i = 0; i < value.length(); i++) {
    if (!isDigit(value.charAt(i))) {
      return false;
    }
  }

  return true;
}

bool isValidDit(int dit) {
  return dit >= 20 && dit <= 2000;
}

void processCommand(String command, HelloWorldConfig &config) {
  command.trim();
  command.toLowerCase();

  if (command == "start") {
    config.helloWorldRunning = true;
    Serial.println("[prompt] Hello World loop started");
  } else if (command == "stop") {
    config.helloWorldRunning = false;
    Serial.println("[prompt] Hello World loop stopped");
  } else if (command == "getledgpio") {
    Serial.printf("[prompt] Current LED GPIO: %d\n", config.ledGpio);
  } else if (command == "getledinvert") {
    Serial.printf("[prompt] LED inverted: %s\n", config.invertLed ? "yes" : "no");
  } else if (command == "getdit") {
    Serial.printf("[prompt] Current dit: %d ms\n", config.dit);
  } else if (command.startsWith("setledgpio")) {
    String value = command.substring(String("setledgpio").length());
    value.trim();

    if (!isUnsignedNumber(value)) {
      Serial.println("[prompt] Usage: setLedGpio <gpio_number>");
      return;
    }

    int newLedGpio = value.toInt();
    if (newLedGpio == config.ledGpio) {
      Serial.printf("[prompt] LED GPIO remains at %d\n", config.ledGpio);
      return;
    }

    config.ledGpio = newLedGpio;
    pinMode(config.ledGpio, OUTPUT);
    Serial.printf("[prompt] LED GPIO set to %d\n", config.ledGpio);
  } else if (command.startsWith("setdit")) {
    String value = command.substring(String("setdit").length());
    value.trim();

    if (!isUnsignedNumber(value)) {
      Serial.println("[prompt] Usage: setDit <milliseconds>");
      return;
    }

    int newDit = value.toInt();
    if (!isValidDit(newDit)) {
      Serial.println("[prompt] dit must be in range 20..2000 ms");
      return;
    }

    if (newDit == config.dit) {
      Serial.printf("[prompt] dit remains at %d ms\n", config.dit);
      return;
    }

    config.dit = newDit;
    Serial.printf("[prompt] dit set to %d ms\n", config.dit);
  } else if (command == "toggleledinvert") {
    config.invertLed = !config.invertLed;
    Serial.printf("[prompt] LED inverted: %s\n", config.invertLed ? "yes" : "no");
  } else if (command == "test_r") {
    Serial.println("[prompt] Test Morse R (.-.)");
    outputMorseR(config.ledGpio, config.invertLed, config.dit);
  } else if (command.length() > 0) {
    Serial.println("[prompt] Unknown command. Use: start | stop | getLedGpio | setLedGpio <gpio_number> | getLedInvert | toggleLedInvert | getDit | setDit <milliseconds> | test_r");
  }
}
}  // namespace

void initSerialPromt(const HelloWorldConfig &config) {
  Serial.println("[prompt] Available commands: start | stop | getLedGpio | setLedGpio <gpio_number> | getLedInvert | toggleLedInvert | getDit | setDit <milliseconds> | test_r");
  Serial.printf("[prompt] Current LED GPIO: %d\n", config.ledGpio);
  Serial.printf("[prompt] LED inverted: %s\n", config.invertLed ? "yes" : "no");
  Serial.printf("[prompt] Current dit: %d ms\n", config.dit);
}

bool handleSerialPromt(HelloWorldConfig &config) {
  while (Serial.available() > 0) {
    char incoming = static_cast<char>(Serial.read());

    if (incoming == '\n' || incoming == '\r') {
      processCommand(serialInput, config);
      serialInput = "";
    } else {
      serialInput += incoming;
    }
  }

  return config.helloWorldRunning;
}