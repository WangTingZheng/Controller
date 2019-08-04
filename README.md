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