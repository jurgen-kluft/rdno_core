# rdno_core, Arduino core library for ESP32 and ESP8266

For my own home development, automation, sensors, etc.. I have setup C++ libraries that build with Arduino libraries. 

Note: This uses ccode to pull in any dependencies, this also means that any used sensors and their libraries need to wrapped 
into a `rdno_{name}` libraries/packages. There are currently 2 examples of this in the `rdno_wifi` and `rdno_sensors` library.

# Arduino and ESP32/ESP8266 SDK

Certain things need to be installed on your system to get this to work, mainly the Esp32 SDK needs to be cloned.
Also you do have to have `git`, and `go` installed on your system.

On my machine the Esp32 SDK is located in `~/sdk/Arduino/esp32` but you can place it anywhere you like. You do have to make
sure that you have a environment variable named `ESP_SDK` that points to the location of the SDK. 

- `git clone https://github.com/espressif/arduino-esp32.git YourLocation`
- `cd YourLocation`
- `git submodule update --init --recursive`
- `cd esp32/tools`
- `python get.py`

# ESP32 SDK Version

URL: https://github.com/espressif/arduino-esp32  
Local Directory: `~/sdk/Arduino/esp32`  
Commit `6f92b604f62568eced0e519b01d17af00b80ebe4` (May 13, 2025)  

# ESP8266 SDK Version

URL: https://github.com/esp8266/Arduino  
Local Directory: `~/sdk/Arduino/esp8266`  
Commit `1297133cd27986a5153d47ef60f1fe6ec66d1006` (Sep 9, 2025)  


# Environment Variables

- MacOS
  - edit your `~/.bash_profile` or `~/.bashrc` and add the following line:
  ```bash
  export ESP32_SDK=~/sdk/Arduino/esp32
  export ESP8266_SDK=~/sdk/Arduino/esp8266
  ```
