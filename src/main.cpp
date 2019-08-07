#include <Arduino.h>
#include <U8glib.h>
#include <IRremote.h>   //IR receiver
#define line 15
#define column 10
long HEXN[21]={ //mini remote control key hex id
 0xFD00FF,0xFD807F,0xFD40BF
,0xFD20DF,0xFDA05F,0xFD609F
,0xFD10EF,0xFD906F,0xFD50AF
,0xFD30CF,0xFDB04F,0xFD708F
,0xFD08F7,0xFD8877,0xFD48B7
,0xFD28D7,0xFDA857,0xFD6897
,0xFD18E7,0xFD9867,0xFD58A7
}; 
const char Number[10]={'0','1','2','3','4','5','6','7','8','9'};
int speedInput[10];
int speedInputFlag=0;

int RECV_PIN = 11;   //IR revicer pin
long  controller=0;    // storage key hex id
int speed=100;
int delaymsloop;

IRrecv irrecv(RECV_PIN); 
decode_results results;
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE|U8G_I2C_OPT_DEV_0);	// I2C / TWI 
int flag;
int number;

void speedInputInit(){
   speedInputFlag=0;
  for(int i=0;i<10;i++){                         //init speedInput
    speedInput[i]=-1;
  }
}
int idToNu(long id){
  for(int i=0;i<21;i++){
    if(HEXN[i]==id){
      return i;
    }
  }
  return -1;
}
void u8glibSet(){
  if ( u8g.getMode() == U8G_MODE_R3G3B2 ) {
    u8g.setColorIndex(255);     // white
  }
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT ) {
    u8g.setColorIndex(3);         // max intensity
  }
  else if ( u8g.getMode() == U8G_MODE_BW ) {
    u8g.setColorIndex(1);         // pixel on
  }
  else if ( u8g.getMode() == U8G_MODE_HICOLOR ) {
    u8g.setHiColorByRGB(255,255,255);
  }
}


void page_1(){
  u8g.setFont(u8g_font_unifont);
  u8g.drawStr(0, line*1, "May I help you?");
  u8g.drawStr(0, line*2, "1.modify speed.");
  u8g.drawStr(0 ,line*3, "2.read speed.");
}

void page_1_1(){
  u8g.setFont(u8g_font_unifont);
  u8g.drawStr(0, line*1, "Enter the speed:");
}

void page_1_1_1(){
  u8g.setFont(u8g_font_unifont);
  u8g.drawStr(0, line*1, "change done!");
}
void page_1_2(){
  
  u8g.setFont(u8g_font_unifont);
  u8g.drawStr( 0, line*1, "the speed is:");
  u8g.setPrintPos(0, line*2);
  u8g.print(speed);
}
void numberPress(){
      speedInput[speedInputFlag]=idToNu(controller)-11;
      if(speedInputFlag==10){
        u8g.firstPage();
        do{
            u8g.drawStr(0,line,"out of length.");
        }while(u8g.nextPage());
        speedInputInit();
        flag=1;
      }
      else{
        //speedInputFlag=speedInputFlag+1;
        u8g.firstPage();
        do{
          u8g.setPrintPos(0,line*1);
          u8g.print(idToNu(controller)-11);
        }while(u8g.nextPage());
      }
}
void runtest(int n){
    switch (n){
      case 1:{
        u8g.firstPage();  
        do {
            page_1();
        }while( u8g.nextPage() );
        break;
      }
      case 2:{
        u8g.firstPage();  
        do {
            page_1_1();
        }while( u8g.nextPage() );
        break;
      }
      case 3:{
        u8g.firstPage();  
        do {
            page_1_2();
        }while( u8g.nextPage() );
        break;
      }
      case 4:{
        u8g.firstPage();  
        do {
            page_1_1_1();
        }while( u8g.nextPage());
        break;
      }
      case 5:{
        numberPress();
        break;
      }
    }
  }
void setup(){      
  flag=1;   
  u8glibSet();      
  delay(500);                                    //I don't know why it is here
  irrecv.enableIRIn();                           //enable IR revicer 
  speedInputInit();
}


void loop() {
   if (irrecv.decode(&results)) {  
        controller=results.value;     //storage key id
        if(flag==1&&controller==HEXN[12]) flag=2;
        else if(flag==1&&controller==HEXN[13]) flag=3;
        else if(flag==2&&controller==HEXN[3]) flag=1;   //back
        else if(flag==2&&controller==HEXN[4]) {flag=4;speedInputInit();}   //go in
        else if(flag==2&&(idToNu(controller)>=12)) flag=5;  //if you press number
        else if(flag==3&&controller==HEXN[3]) flag=1;
        else if(flag==3&&controller==HEXN[3]) flag=1;
        else if(flag==4) flag=1;
        irrecv.resume();  
  }
  runtest(flag);
}