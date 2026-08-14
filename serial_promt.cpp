#include "./serial_promt.h"

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

void printPromptHelp() {
  Serial.println("[prompt] Available commands:");
  Serial.println("[prompt]   help                 - Show this help");
  Serial.println("[prompt]   start                - Start Hello World loop");
  Serial.println("[prompt]   stop                 - Stop Hello World loop");
  Serial.println("[prompt]   getLedGpio           - Show current LED GPIO");
  Serial.println("[prompt]   setLedGpio <n>       - Set LED GPIO to <n>");
  Serial.println("[prompt]   getLedInvert         - Show LED invert state");
  Serial.println("[prompt]   toggleLedInvert      - Toggle LED invert state");
  Serial.println("[prompt]   getDit               - Show Morse dit in ms");
  Serial.println("[prompt]   setDit <ms>          - Set Morse dit (20..2000)");
  Serial.println("[prompt]   getText              - Show current text");
  Serial.println("[prompt]   setText <text>       - Set Morse text");
  Serial.println("[prompt]   test_r               - Output Morse R (.-.) once");
}

void processCommand(String command, HelloWorldConfig &config) {
  const String originalCommand = command;
  command.trim();
  String commandLower = command;
  commandLower.toLowerCase();

  if (commandLower == "help") {
    printPromptHelp();
  } else if (commandLower == "start") {
    config.helloWorldRunning = true;
    Serial.println("[prompt] Hello World loop started");
  } else if (commandLower == "stop") {
    config.helloWorldRunning = false;
    Serial.println("[prompt] Hello World loop stopped");
  } else if (commandLower == "getledgpio") {
    Serial.printf("[prompt] Current LED GPIO: %d\n", config.ledGpio);
  } else if (commandLower == "getledinvert") {
    Serial.printf("[prompt] LED inverted: %s\n", config.invertLed ? "yes" : "no");
  } else if (commandLower == "getdit") {
    Serial.printf("[prompt] Current dit: %d ms\n", config.dit);
  } else if (commandLower == "gettext") {
    Serial.printf("[prompt] Current text: %s\n", config.text.c_str());
  } else if (commandLower.startsWith("setledgpio")) {
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
  } else if (commandLower.startsWith("setdit")) {
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
  } else if (commandLower.startsWith("settext")) {
    String value = originalCommand.substring(String("settext").length());
    value.trim();

    if (value.length() == 0) {
      Serial.println("[prompt] Usage: setText <text>");
      return;
    }

    if (value == config.text) {
      Serial.printf("[prompt] text remains: %s\n", config.text.c_str());
      return;
    }

    config.text = value;
    Serial.printf("[prompt] text set to: %s\n", config.text.c_str());
  } else if (commandLower == "toggleledinvert") {
    config.invertLed = !config.invertLed;
    Serial.printf("[prompt] LED inverted: %s\n", config.invertLed ? "yes" : "no");
  } else if (commandLower == "test_r") {
    Serial.println("[prompt] Test Morse R (.-.)");
    outputMorseR(config.ledGpio, config.invertLed, config.dit);
  } else if (command.length() > 0) {
    Serial.println("[prompt] Unknown command. Type 'help' for command list.");
  }
}
}  // namespace

void initSerialPromt(const HelloWorldConfig &config) {
  printPromptHelp();
  Serial.printf("[prompt] Current LED GPIO: %d\n", config.ledGpio);
  Serial.printf("[prompt] LED inverted: %s\n", config.invertLed ? "yes" : "no");
  Serial.printf("[prompt] Current dit: %d ms\n", config.dit);
  Serial.printf("[prompt] Current text: %s\n", config.text.c_str());
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
