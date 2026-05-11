/*
  Project: ESP Hello World
  Project URL: https://github.com/hredan/ESP_HELLO_WORLD
  Description:
  This sketch will send the message "Hello World" as Morse code and using the LED_BUILTIN.
  It is an example of how to share esp projects with the ESPEasyFlasher_2.0 for different platforms.
  The release assets are build for the boards esp8266_d1_mini and esp32_d1_mini32.
  If you need this example for another esp board, you can fork this project and run the manual workflow with settings of your board.
*/
#include <Arduino.h>
#include <Dictionary.h>
#include "./serial_promt.h"
#include "./eeprom_config.h"

Dictionary *dict = new Dictionary();
String text = "HELLO WORLD";
#ifndef LED_BUILTIN
constexpr int DEFAULT_LED_GPIO = 2;
#else
constexpr int DEFAULT_LED_GPIO = LED_BUILTIN;
#endif

#ifdef ESP8266
constexpr bool DEFAULT_INVERT_LED = true;
#else
constexpr bool DEFAULT_INVERT_LED = false;
#endif

constexpr int DEFAULT_DIT_MS = 80;

HelloWorldConfig serialPromtConfig = {
  true,
  DEFAULT_LED_GPIO,
  DEFAULT_INVERT_LED,
  DEFAULT_DIT_MS
};

// https://en.wikipedia.org/wiki/Morse_code#Representation,_timing,_and_speeds
int dah = 3 * DEFAULT_DIT_MS;
int symbol_space = DEFAULT_DIT_MS;
int letter_space = 3 * DEFAULT_DIT_MS;
int word_space = 7 * DEFAULT_DIT_MS;

void updateMorseTimingFromDit(const HelloWorldConfig &config) {
  dah = 3 * config.dit;
  symbol_space = config.dit;
  letter_space = 3 * config.dit;
  word_space = 7 * config.dit;
}

bool handlePromptAndPersistConfig(HelloWorldConfig &config) {
  HelloWorldConfig previousConfig = config;
  bool isRunning = handleSerialPromt(config);

  if (previousConfig.dit != config.dit) {
    updateMorseTimingFromDit(config);
  }

  persistConfigIfChanged(previousConfig, config);

  return isRunning;
}

void setup() {
  Serial.begin(115200);
  initConfigFromEeprom(serialPromtConfig, DEFAULT_LED_GPIO, DEFAULT_INVERT_LED, DEFAULT_DIT_MS);
  updateMorseTimingFromDit(serialPromtConfig);

  Serial.println("");
  Serial.println("Starting Hello World");
  Serial.printf("Built-in LED on GPIO PIN: %d\n", serialPromtConfig.ledGpio);
  Serial.printf("LED inverted: %s\n", serialPromtConfig.invertLed ? "yes" : "no");
  Serial.printf("dit speed: %d ms\n", serialPromtConfig.dit);
  pinMode(serialPromtConfig.ledGpio, OUTPUT);
  initSerialPromt(serialPromtConfig);

  dict->insert("H", "....");
  dict->insert("E", ".");
  dict->insert("L", ".-..");
  dict->insert("O", "---");
  dict->insert("W", ".--");
  dict->insert("R", ".-.");
  dict->insert("D", "-..");
  dict->insert(" ", " ");
}

// the loop function runs over and over again forever
void loop() {
  if (!handlePromptAndPersistConfig(serialPromtConfig)) {
    delay(50);
    return;
  }

  Serial.println("Start Hello World loop");
  delay(2000);
  for (int i = 0; i < text.length(); i++) {
    if (!handlePromptAndPersistConfig(serialPromtConfig)) {
      return;
    }
    String letter = text.substring(i, i + 1);
    String morse = dict->search(letter);

    Serial.printf("Letter: %s -> %s\n", letter.c_str(), morse.c_str());
    for (int j = 0; j < morse.length(); j++) {
      if (!handlePromptAndPersistConfig(serialPromtConfig)) {
        return;
      }
      char symbol = morse.charAt(j);
      if (symbol == '.') {
        ledOnForTimespan(serialPromtConfig.dit);
      } else if (symbol == '-') {
        ledOnForTimespan(dah);
      } else if (symbol == ' ') {
        delay(word_space);
      }
    }
    delay(letter_space);
  }
  Serial.println("End loop");
}

void ledOnForTimespan(int timespan) {
  if (serialPromtConfig.invertLed) {
    digitalWrite(serialPromtConfig.ledGpio, LOW);
  } else {
    digitalWrite(serialPromtConfig.ledGpio, HIGH);
  }
  delay(timespan);
  if (serialPromtConfig.invertLed) {
    digitalWrite(serialPromtConfig.ledGpio, HIGH);
  } else {
    digitalWrite(serialPromtConfig.ledGpio, LOW);
  }
  delay(symbol_space);
}
