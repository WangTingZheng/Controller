# Controller for arduino
**I am sorry to telling you that I haven't tested this project in my arduino, but it has been passed builting in vscode with PlatformIO**
# install
- vscode
- PlatformIO IDE for vscode

# hardware
- Arduino uno
- oled screen with `I2c` protocol and `ssd1306` control chip
- mini IR remote controller
- IR revicer
  
# Something Important
## about libary
- GFX(not recomand)
- SSD1306(together with GFX)
- IRremote
- u8glib(it's work)
- u8g2(not work because of high ram utilization)
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
For example, you want to add u8g2 to your project's library(you have already copied the u8g2 folder to `./lib`), you should add `"my project path in your pc/controller/lib/U8g2"`,

## about RAM
If you want use it in Arduino uno, I am sorry to telling you that your arduino may lost all usable ram becaue of some GUI library, I have already shou the ram utilization of different GUI library, you can choose what you want. if you have more powerful Arduino, everything is nothong.

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

The drawback is that the u8glib/utility/u8g_rot.c(line 48) has an warning

The developer said that it can be ignored.

https://github.com/olikraus/u8glib/issues/366

# usage
**Play attention: If vscode tells you it can not found some library, please detect my include path and add your project's.**
- git clone to your comptuer
- built this project with vscode(PlatformIO) to make should it's work on your comptuer
- connect your oled and IR revicer to your arduino uno
- not finshed, i will add entire step when i test it in my arduino

IR revicer:
```
GND:GND
VCC:5V
Vout:11
```
- edit the function `PageS` to new a page and it's id with u8g2
- edit the function `keyChange` to new the rule of page switching
