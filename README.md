# rdno_core, Arduino core library for ESP32, ESP32C3 and ESP32S3

For my own home development, automation, sensors, etc.. I have setup C++ libraries that build on top of Arduino core libraries. 

Note: This uses ccode to pull in any dependencies, this also means that any used sensors and their libraries need to wrapped 
into a `rdno_{name}` libraries/packages. There are currently 2 examples of this in the `rdno_wifi` and `rdno_network` libraries.

# Arduino and ESP32

Certain things need to be installed on your system to get this to work, mainly the Esp32 SDK needs to be cloned.
Also you do have to have `git`, and `go` installed on your system.

On my machine the Esp32 SDK is located in `~/sdk/Arduino/esp32` but you can place it anywhere you like. You do have to make
sure that you have a environment variable named `ESP_SDK` that points to the location of the SDK. 

- `git clone https://github.com/espressif/arduino-esp32.git YourLocation`
- edit your `~/.bash_profile` or `~/.bashrc` and add the following line:
```bash
export ESP_SDK=~/sdk/Arduino/esp32
```

# ESP32 SDK Version

Currently at commit `6f92b604f62568eced0e519b01d17af00b80ebe4` (May 13, 2025).

Going to try commit at tag 3.1.3, since there are issues with I2C.
`dbfde15b6ac25720c9132ebd33decf6b34d5e2af        refs/tags/3.1.3`
