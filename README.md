![SleepUINO_Logo_PreDev](https://user-images.githubusercontent.com/48091357/111156537-25298a00-8596-11eb-8726-1fe5cd7bed93.png)
# ESP Hello World
ESP_HELLO_WORLD is an example to build ESP firmware binaries on a GitHub Runner with the help of [eep-build-action](https://github.com/hredan/eep-build-action), to create a package for different OS types with the help of [load-eef-action](https://github.com/hredan/load-eef-action) and [ESPEASYFLASHER_2.0](https://github.com/hredan/ESPEASYFLASHER_2.0). This example is intended to show how easy it is to build and share esp firmware binaries with [ESPEASYFLASHER_2.0](https://github.com/hredan/ESPEASYFLASHER_2.0).

The ESP_HELLO_WORLD example it self, is sending a "Hello World" as Morse Code by the Build-in LED and sends messages over the serial interface that can be received with the [ESPEASYFLASHER_2.0](https://github.com/hredan/ESPEASYFLASHER_2.0) for analyzing.

[Morse Code](https://en.wikipedia.org/wiki/Morse_code)

## Serial prompt commands
The device exposes a simple command prompt over the serial interface. The available commands are:

- `help` - Show the full command list
- `start` - Start the Hello World loop
- `stop` - Stop the Hello World loop
- `getLedGpio` - Show the current LED GPIO pin
- `setLedGpio <n>` - Set the LED GPIO pin
- `getLedInvert` - Show whether the LED signal is inverted
- `toggleLedInvert` - Toggle the LED invert state
- `getDit` - Show the current Morse dit length in milliseconds
- `setDit <ms>` - Set the Morse dit length (valid range: 20..2000)
- `getText` - Show the current text to transmit
- `setText <text>` - Set the text to transmit over Morse code
- `test_r` - Output the Morse pattern for `R` once

The default text is `Hello World`.

The prompt is available in the serial monitor at startup and can be used to reconfigure the LED pin, invert behavior, timing or message text without recompiling the firmware.

# Disclaimer
All this code is released under the GPL, and all of it is to be used at your own risk. If you find any bugs, please let me know via the GitHub issue tracker or drop me an email ([hredan@sleepuino.de](mailto:hredan@sleepuino.de)).
