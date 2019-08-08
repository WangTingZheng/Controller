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
int key[11]={-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,0};  //key[10] is flag
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
int idN;
int powerOfTen(int n){
  int resu=1;
  for(int i=0;i<n;i++){
      resu=resu*10;
  }
  return resu;
}
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

void page_1_1_1(){
  
  u8g.drawStr(0, line*1, "change done!");
}
void page_1_2(){
  u8g.setFont(u8g_font_unifont);
  u8g.drawStr( 0, line*1, "the speed is:");
  u8g.setPrintPos(0, line*2);
  u8g.print(speed);
}

void draw_Input(int l,int c) {
  u8g.setFont(u8g_font_unifont);
  if(l==1){
    u8g.drawStr( 0, 30, " 4  5  6");
    u8g.drawStr( 0, 45, " 7  8  9");
    u8g.drawStr( 0, 60, " 0     .");
    if(c==1){
        u8g.drawStr( 0, 15, "#1  2  3");
    }
    else if(c==2){
        u8g.drawStr( 0, 15, " 1 #2  3");
    }
    else if(c==3){
        u8g.drawStr( 0, 15, " 1  2 #3");
    }
  }
  else if(l==2){
    u8g.drawStr( 0, 15, " 1  2  3");
    u8g.drawStr( 0, 45, " 7  8  9");
    u8g.drawStr( 0, 60, " 0     .");
    if(c==1){
        u8g.drawStr( 0, 30, "#4  5  6");
    }
    else if(c==2){
        u8g.drawStr( 0, 30, " 4 #5  6");
    }
    else if(c==3){
        u8g.drawStr( 0, 30, " 4  5 #6");
    }
  }
  else if (l==3){
    u8g.setFont(u8g_font_unifont);
    u8g.drawStr( 0, 15, " 1  2  3");
    u8g.drawStr( 0, 30, " 4  5  6");
    u8g.drawStr( 0, 60, " 0     .");
    if(c==1){
         u8g.drawStr( 0, 45, "#7  8  9");
    }
    else if(c==2){
         u8g.drawStr( 0, 45, " 7 #8  9");
    }
    else if(c==3){
         u8g.drawStr( 0, 45, " 7  8 #9");
    }
  }
  else if(l==4){
      u8g.drawStr( 0, 15, " 1  2  3");
      u8g.drawStr( 0, 30, " 4  5  6");
      u8g.drawStr( 0, 45, " 7  8  9");
      if(c==1){
         u8g.drawStr( 0, 60, "#0     .");
      }
      else if(c==2){
         u8g.drawStr( 0, 60, " 0 #   .");
      }
      else if(c==3){
         u8g.drawStr( 0, 60, " 0    #.");
      }
  }
  else {
      u8g.drawStr( 0, 15, " 1  2  3");
      u8g.drawStr( 0, 30, " 4  5  6");
      u8g.drawStr( 0, 45, " 7  8  9");
      u8g.drawStr( 0, 60, " 0     .");
    }
}
int thePower(int z){
  int temp=1;
  for(int q=0;q<z;q++){
    temp *=10;
  }
  return temp;
}
void page_1_1(){
  int number_press;
  int x,y;
  int value;
  //int tmp;
  int numberP=-1;  //the buuton you pressed id
  for(int l=0;l<21;l++){
    if(controller==HEXN[l]){
        numberP=l;
    }
  }
  u8g.drawStr(80,15, "|Enter");
  u8g.drawStr(80,30, "|the ");
  u8g.drawStr(80,45, "|speed");
  if(numberP==9){
      draw_Input(4,1);
      if(key[10]!=10){
        key[key[10]]=0;
        key[10]++;
      }else {
        key[10]=0;
      }
  }
  else if(numberP==4){  //confirm
      flag=4;
      for(int n=key[10];n>=0;n--){
          value+=key[n]*thePower(key[10]-n);
      }
     speed=value;
  }
  else if(numberP==3){ //cansle
        flag=1;
        key[10]=0;
  }
  else if(numberP>11){
    number_press=numberP-11;
    if(key[10]!=10){
      key[key[10]]=number_press;
      key[10]++;
    }else {           // be fulled
      key[10]=0;
    }
    if((number_press%3)!=0){
        x=number_press/3+1;
        y=number_press%3;
    }
    else {
        x=number_press/3;
        y=3;
    }
  }
  else{
    x=0;
    y=0;
  }
  draw_Input(x,y);
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
    }
}
void setup(){      
  flag=1;   
  u8glibSet();      
  delay(500);                                    //I don't know why it is here
  irrecv.enableIRIn();                           //enable IR revicer 
  speedInputInit();
  idN=-1;
}


void loop() {
   if (irrecv.decode(&results)) {  
        controller=results.value;     //storage key id
        idN=idToNu(controller);
        if(flag==1&&controller==HEXN[12]) flag=2;
        else if(flag==1&&controller==HEXN[12]) flag=2;
        else if(flag==1&&controller==HEXN[13]) flag=3;
        else if(flag==3&&controller==HEXN[3])  flag=1;
        else if(flag==4){delay(300);flag=1;} 
        irrecv.resume();  
  }
  runtest(flag);
}