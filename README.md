# Controller for Arduino
[![Build Status](https://travis-ci.com/WangTingZheng/Controller.svg?branch=master)](https://travis-ci.com/WangTingZheng/Controller)
# install
- [vscode](https://code.visualstudio.com/)
- [PlatformIO IDE for vscode](https://marketplace.visualstudio.com/items?itemName=platformio.platformio-ide)
# hardware
- Arduino uno
- oled screen with `I2c` protocol and `ssd1306` control chip(four pin)
- mini IR remote controller 
- IR reviver
- LD-27MG
- Battery 7.4v 1500mah 20c|3.7v 2000mah 7.4wh
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

open `.vscode/c_cpp_properties.json`, you can find this:
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
- set your IR remote button id  

new a project with Arduino IDE and copy this code:
```arduino
#include <IRremote.h>     // IRremote库声明  
int RECV_PIN = 11;        //定义红外接收器的引脚为11  
IRrecv irrecv(RECV_PIN);   
decode_results results;   //解码结果放在 decode results结构的 result中
void setup()  {  
      Serial.begin(9600);  
      irrecv.enableIRIn(); // 启动接收器  
}  
void loop() {  
	  if (irrecv.decode(&results)) { //解码成功，收到一组红外讯号   
	    Serial.println(results.value, HEX);//以16进制换行输出接收代码  
	    irrecv.resume(); // 接收下一个值  
	  }  
	  delay(100);  
}  
```
of course, if you use PlatformIO, you should add `#include <Arduino.h>` in font of this code  
- open serial monitor(I don't know how to use it in PlatformIO, maybe in the cli?)

press every button and write it down, and then, switch this part of my project:
``` c
long HEXN[21]={     //mini remote control key hex id
 0xFD00FF,0xFD807F,0xFD40BF
,0xFD20DF,0xFDA05F,0xFD609F
,0xFD10EF,0xFD906F,0xFD50AF
,0xFD30CF,0xFDB04F,0xFD708F
,0xFD08F7,0xFD8877,0xFD48B7
,0xFD28D7,0xFDA857,0xFD6897
,0xFD18E7,0xFD9867,0xFD58A7
}; 
```
The order is from  top to bottom, from left to right
- fix steering engine parameter

find this in `main.cpp`
```c
long  AlplaTransform1(long big){
  return big/1.5-6;
}
long  AlplaTransform2(long big){
  return big/1.5+102;
}
```
change the code after `return`, make should that your engine can turn to `big` degrees
- press the PlatformIO build under the vscode bar
- if everything is ok, the building will be passed.


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
steering engine
```
GND-GND(battery)
VCC-7.4v(battery)
signal-8
```

```
GND-GND(battery)
VCC-7.4V(battery)
signal-9
```
# function
- some windows like drawer 
- read values in the project in oled
- modify values with IR controller
- control two Steering engines

![function.jpg](https://s2.ax1x.com/2019/08/08/e7BD6P.png)
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