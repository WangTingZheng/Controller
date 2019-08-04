/*
  This is a project with u8g2 library(the more powerful u8glib)
  because of high ram utilization(104%), I abandoned it
  It's terrible, It seems that I should change my arduino
  when I used u8glib ,the utilization is 38.7%,it's ok.
 */
#include <Arduino.h>
#include <U8g2lib.h>
#include <IRremote.h>   //IR receiver
#include <avr/pgmspace.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

const unsigned long HEXN[21]={ //mini remote control key hex id
 0xFD00FF,0xFD807F,0xFD40BF
,0xFD20DF,0xFDA05F,0xFD609F
,0xFD10EF,0xFD906F,0xFD50AF
,0xFD30CF,0xFDB04F,0xFD708F
,0xFD08F7,0xFD8877,0xFD48B7
,0xFD28D7,0xFDA857,0xFD6897
,0xFD18E7,0xFD9867,0xFD58A7
}; 

String Name[21]={  //mini remote control key name
  "power",
  "vol+",
  "func",
  "playBack",
  "playStop",
  "playForward",
  "down",
  "vol-",
  "up",
  "0",
  "EQ",
  "replay",
  "1","2","3","4","5","6","7","8","9"
};
int RECV_PIN = 11;   //IR revicer pin
int D3 = 3;          //interrupt pin
int D2 =2;           //interrupt pin
long  controller=0;    // storage key hex id
int page=1;         //page number now
IRrecv irrecv(RECV_PIN); 
decode_results results;
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0,U8X8_PIN_NONE);


long NumToHex(int n){
  return pgm_read_byte(&HEXN[n]);
}
void page_1(){
    u8g2.clearBuffer();         //
    u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
    u8g2.drawStr(0,10,"Hello World!");  // write something to the internal memory
    u8g2.drawStr(0,20,"This is page1!");  // write something to the internal memory
    u8g2.drawStr(0,30,"Welcome to here!");  // write something to the internal memory
    u8g2.sendBuffer();          // transfer internal memory to the display
}

void page_1_1(){
  u8g2.clearBuffer();        
  u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
  u8g2.drawStr(0,10,"Hello!");  // write something to the internal memory
  u8g2.drawStr(0,20,"This is page2!");  // write something to the internal memory
  u8g2.drawStr(0,30,"Welcome to here!");  // write something to the internal memory
  u8g2.sendBuffer();          // transfer internal memory to the display
}
void PageS(long n){              //show page with gui code
  if(n==1)
     page_1();
  else if(n==1.1){
     page_1_1();
  }
}
/*
String HexToName(long H){ 
  for(int i=1;i<21;i++){
    if(HEXN[i]==H){
      return Name[i];
    }
  }
}
long NameToHex(String N){
  for(int i=1;i<21;i++){
    if(Name[i]==N){
      return HEXN[i];
    }
  }
}*/
void keyChange(){                     //interrupt function
    if(page==1&&controller==NumToHex(4)){    // you press playStop button when your screen is page 1
      page=1.1;                          //set page flag to 1.1
    }
    if(page==1.1&&controller==NumToHex(3)){  //your press playback button when your screen is page 1.1
      page=1;                            //set page flag to 1
    }
}
void valueChange(){
     
}
void setup(){   
  u8g2.begin();                          
  pinMode(D3,INPUT);                   //set d3 mode(d3 is interrupt pin)
  delay(500);                                    //I don't know why it is here
  irrecv.enableIRIn();                           //enable IR revicer 
  attachInterrupt(1,keyChange,CHANGE);         //set interrupt pin,set interrupt function,set interrupt mode(Triggered when changing)
  attachInterrupt(0,valueChange,CHANGE); 
}

void loop() {                
   PageS(page);                         //show page acconding to flag (when flag changed,page would be changed)
   if (irrecv.decode(&results)) {   //if mini remote controller's button is pressed
      controller=results.value;     //storage key id
      digitalWrite(D3,HIGH);        //trigger inturrpt
      irrecv.resume();             //resume IR revicer
   }
}