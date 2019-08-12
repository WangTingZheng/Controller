# Controller for Arduino   
[![Build Status](https://travis-ci.com/WangTingZheng/Controller.svg?branch=master)](https://travis-ci.com/WangTingZheng/Controller) 

[Engish edition](https://githuab.com/wangtingzheng/controller/README-EN.md)
# 安装
- [vscode](https://code.visualstudio.com/)
- [PlatformIO IDE for vscode](https://marketplace.visualstudio.com/items?itemName=platformio.platformio-ide)
# 硬件准备
- Arduino uno
- 带有4个引脚的，具有I2C通信功能的oled屏幕
- 迷你红外遥控器
- 红外接收管
- LD-27MG舵机
- 电池: 7.4v 1500mah 20c || 3.7v 2000mah 7.4wh
- 继电器:  Q3F-1Z  5V  4PIN
# 配置
- 执行 
```git
git clone https://github.com/wangtingzheng/controller.git
```
来把我的项目克隆到你的电脑

- 打开 vscode 
- 打开 PlatformIO 
- 引入 my project
- 配置项目的设置

打开 `.vscode/c_cpp_properties.json`，你会发现：
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
根据你的实际情况修正带有 `./lib`的路径，这个是库的路径。
- 修改红外遥控器的按键id 

在Arduino IDE 里新建一个工程：
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
当然，你可以使用PlatformIO，但是你得在文件前加`#include <Arduino.h>` 
- 打开串口

按下每一个按键并且通过串口记录下每一个按键的id并在项目中修正它
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
按键顺序是从上到下，从左向右
- 修正舵机参数

在 `main.cpp`中找到以下函数：
```c
long  AlplaTransform1(long big){
  return big/1.5-6;
}
long  AlplaTransform2(long big){
  return big/1.5+102;
}
```
改变 `return`后面的函数, 让项目能输入度数，返回转的度数的PWM波。
- 在vscode下端，有一个打勾的按钮，点击它PlatformIO会编译当前的项目

- 如果一切正常的话，编译会通过

# 连接
oled:
```
GND-GND
VCC-3.3V
SCL-SCL
SDA-SDA
```
迷你红外遥控器 :
```
不需要连接，这是无线的
```
红外接收管
```
GND-GND
VCC-12
Vout-13
```
舵机
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
继电器
Arduino控制端
控制上电的继电器
```
GND-GND
H-free
L-2
VCC-11
```
控制开火的继电器
```
GND-6
VCC-3
H-free
L-5
```
继电器输出
```
NO-用电器的+
COM-3.3V
NC-不接
```
# 函数
- 像抽屉一样的窗口
- 在oled中显示项目中的变量值
- 用红外遥控器修改项目中任意的变量值
- 能够控制两个舵机
- 能够控制两个继电器控制电磁炮充电和放电

![function.jpg](https://s2.ax1x.com/2019/08/08/e7BD6P.png)
# 一些重要的事
## 关于库
- GFX(不推荐)
- SSD1306(与GFX一起的)
- IRremote
- u8glib(可行)
- u8g2(SRAM占有率太高，uno板没法用)
### 如果安装库
- 把要安装的库拷贝到 `./lib`目录下
- 在vscode的`included path` 中加入所有库的地址

## 关于RAM
如果你想要使用Arduino uno，我很不幸地告诉你，有一些GUI库中，有一些你无法使用，因为这些GUI库地RAM占有率太高，但如果你有其它的Arduino开发板，你可以随便使用。

## 关于 `./test`
这里包含着三个GUI库：

|                 | U8glib                               | U8g2                            | GXF             |
| --------------- | -------------------------------------| --------------------------------| ----------------|
| ram 占有率       | 38.7%                                | 104%                            | 91%             |
| 使用情况         | :+1:                                 | :no_entry_sign:                 | :fearful:       |
| PS              | 有一些waring，但是[不影响](https://github.com/olikraus/u8glib/issues/366) | u8glib的升级版 | 另外一个GUI库 |

## 关于继电器

| H    | L    | Relay      | OUTPUT                       |
| ---- | ---- | ---------- | ---------------------------- |
| 激活 | 激活 | 工作        | NO-COM: 连通  \|\| NC-COM: 不连通    |
| 激活 | 不激活  | 工作         | NO-COM: 连通  \|\| NC-COM: 不连通 |
| 不激活  | 激活 | 不工作        | NO-COM: 不连通 \|\| NC-COM: 连通 |
| 不激活  | 不激活  | 不工作        | NO-COM: 不连通 \|\| NC-COM: 连通 |

