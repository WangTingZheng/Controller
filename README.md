# Controller for arduino
## install
- vscode
- PlatformIO IDE for Arduino
## libary
- GFX
- SSD1306
- IRremote
- u8glib
### how to install libary
- copy your library to `./lib`
- add every library path to `included path` in vscode
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
for example, you want to add u8g2 to your project's library(you have already copied the u8g2 folder to `./lib`), you should add `"c:/Users/王听正/Documents/PlatformIO/Projects/controller/lib/U8g2",`
## hardware
- Arduino uno
- oled screen with `I2c` protocol and `ssd1306` control chip
- mini IR remote controller
- IR revicer
## RAM
In the `./test`, The `main.cpp` is a project with ssd1306 library, because of high ram utilization(91.3%), I abandoned it. when I used u8glib, the utilization is 38.7%. In the `./src`, The `main.cpp` is a project with u8glib, according my compiler result, the date(ram) utilization is `38.7% (used 792 bytes from 2048 bytes)`, the program utilization is `54.1% (used 17450 bytes from 32256 bytes)`
## usage
- connect your oled and IR revicer to your arduino uno

IR revicer:
```
GND:GND
VCC:5V
Vout:11
```
- edit the function `PageS` to new a page and it's id with u8g2
- edit the function `keyChange` to new the rule of page switching
## about `./test`
The folder has three C++ source file, they are:
### main_gxf.cpp:
This is a project with ssd1306 library.
because of high ram utilization(91.3%), I abandoned it.
when I used u8glib ,the utilization is 38.7%.
### main_u8g2.cpp:
This is a project with u8g2 library(the more powerful u8glib)
because of high ram utilization(104%), I abandoned it
It's terrible, It seems that I should change my arduino
when I used u8glib ,the utilization is 38.7%,it's ok.
### main_u8g2lib.cpo:
This is a project with u8glib library.
because of high ram utilization(38.7%), I am using it noew.
It has poor performance, but it's enough.
The drawback is that the u8glib/utility/u8g_rot.c(line 48) has an warning)
The developer said that it can be ignored.
https://github.com/olikraus/u8glib/issues/366
