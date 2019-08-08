# Controller for Arduino
[![Build Status](https://travis-ci.com/WangTingZheng/Controller.svg?branch=master)](https://travis-ci.com/WangTingZheng/Controller)
# install
- vscode
- PlatformIO IDE for vscode
# configure
- run 
```git
git clone https://github.com/wangtingzheng/controller.git
```
to clone my project to your computer.

- open vscode 
- open PlatformIO 
- import my project
- configure project setting

open `.vscode/c_cpp_properties.json`, you can fond this:
```c
  "includePath": [
                "c:/Users/王听正/Documents/PlatformIO/Projects/controller/include",
                "c:/Users/王听正/Documents/PlatformIO/Projects/controller/src",
                "c:/Users/王听正/Documents/PlatformIO/Projects/controller/lib/Arduino-IRremote-master",
                "c:/Users/王听正/Documents/PlatformIO/Projects/controller/lib/Adafruit_SSD1306",
                "c:/Users/王听正/Documents/PlatformIO/Projects/controller/lib/U8g2",
                "C:/.platformio/packages/framework-arduinoavr/libraries/__cores__/arduino/Wire/src",
                "c:/Users/王听正/Documents/PlatformIO/Projects/controller/lib/Adafruit_GFX",
                "C:/.platformio/packages/framework-arduinoavr/cores/arduino",
                "C:/.platformio/packages/framework-arduinoavr/variants/standard",
                "C:/.platformio/packages/framework-arduinoavr/libraries/__cores__/arduino/EEPROM/src",
                "C:/.platformio/packages/framework-arduinoavr/libraries/__cores__/arduino/HID/src",
                "C:/.platformio/packages/framework-arduinoavr/libraries/__cores__/arduino/SPI/src",
                "C:/.platformio/packages/framework-arduinoavr/libraries/__cores__/arduino/SoftwareSerial/src",
                "C:/.platformio/packages/tool-unity",
                ""
            ],
```
fix the location which is include `./lib`, this is the library folder.
- press the platformio build under the vscode bar
- if everything is ok, the building will be passed.

# hardware
- Arduino uno
- oled screen with `I2c` protocol and `ssd1306` control chip(four pin)
- mini IR remote controller 
- IR reviver
# connect
oled:
```
GND-GND
VCC-3.3V
SCL-SCL
SDA-SDA
```
mini IR remote controller :
```
No need to connection, it's Wireless.
```
IR reviver
```
GND-GND
VCC-5V
Vout-11
```
# function
- some windows like drawer 
- read values in the project in oled
- modify values with IR controller

![function.jpg](https://i.loli.net/2019/08/08/qrbTC4jdxlURVaP.jpg)
# Something Important
## about library
- GFX(not recommend)
- SSD1306(together with GFX)
- IRremote
- u8glib(it's work)
- u8g2(not work because of high ram utilization)
### how to install library
- copy your library to `./lib`
- add every library path to `included path` in vscode

## about RAM
If you want use it in Arduino uno, I am sorry to telling you that your Arduino may lost all usable ram because of some GUI library, I have already show the ram utilization of different GUI library, you can choose what you want. if you have more powerful Arduino, everything is nothing.

## about `./test`
The folder has three C++ source file, they are:

|                 | U8glib                                                       | U8g2                            | GXF                 |
| --------------- | ------------------------------------------------------------ | ------------------------------- | ------------------- |
| ram utilization | 38.7%                                                        | 104%                            | 91%                 |
| usage           | :+1:                                                         | :no_entry_sign:                 | :fearful:           |
| PS              | a little warning, but it' [ok](https://github.com/olikraus/u8glib/issues/366) | the  Upgraded version of u8glib | another GUI library |