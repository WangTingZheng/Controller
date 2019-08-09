#include <Arduino.h>
#include <U8glib.h>
#include <IRremote.h>   //IR receiver
#include <Servo.h>

Servo servo1; 
Servo servo2;

#define line 15
#define column 10
#define maxKey 50   //the max length of the array which include the match you input
#define back 3
#define in 4
#define powerUpTime 2000 
#define fireTime 2000





long HEXN[21]={     //mini remote control key hex id
 0xFD00FF,0xFD807F,0xFD40BF
,0xFD20DF,0xFDA05F,0xFD609F
,0xFD10EF,0xFD906F,0xFD50AF
,0xFD30CF,0xFDB04F,0xFD708F
,0xFD08F7,0xFD8877,0xFD48B7
,0xFD28D7,0xFDA857,0xFD6897
,0xFD18E7,0xFD9867,0xFD58A7
}; 

int key[maxKey+1];  //key[10] is flag

const int servo1Pin=8;
const int servo2Pin=9;
const int IRPower =12;
const int vcc=7;
//const int relay_1_1 = 3;      //powerup
const int relay_1 = 2;
const int relay_2 = 5;

const int relay_1_power=11;

const int relay_2_power=3;    //have black tip
const int relay_2_gnd=6;


int posOfServo1 = 90;
int posOfServo2 = 90;
int fireflag=0;

int RECV_PIN = 13;   //IR revicer pin
long  controller=0;    // storage key hex id
int   speed=100;      //the value i want to modify
int   flag;           //the flag target differernt page
int   updateF;       //the updateValue's flag


int d=100;
int alpha=10;

IRrecv irrecv(RECV_PIN); 
decode_results results;
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE|U8G_I2C_OPT_DEV_0);	// I2C / TWI 


void u8glibSet(){                                //oled screen init
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

//////////////////////////////////////////////////////////////////////////////////

void EAControl(int action,int status ){
   if(action==1){
      if(status==1){
          pinMode(relay_1, OUTPUT);
      }
      else if(status==0){
          pinMode(relay_1, INPUT);
      }
   }else if(action ==2){
     if(status==1){
       pinMode(relay_2,OUTPUT);
     }else if(status==0){
       pinMode(relay_2,INPUT);
     }
   }
}

long  AlplaTransform1(long big){
  return big/1.5-6;
}
long  AlplaTransform2(long big){
  return big/1.5+102;
}

void setServoAlpla(int flag,long a){
  if(flag==1)
  servo1.write(AlplaTransform1(a));
  else if(flag==2)servo2.write(AlplaTransform1(a));
}
int thePower(int z){   //calculate the 10^z
  int temp=1;
  for(int q=0;q<z;q++){
    temp *=10;
  }
  return temp;
}

void updateValue(int add){   //add one value behind the array "key"
  if(key[maxKey]!=maxKey){
    key[key[maxKey]]=add;
    key[maxKey]++;
  }else {
    key[maxKey]=0;
  }
}
void display_Side(){                    //display the side which we don't change it
    u8g.drawStr(80,15, "|Enter");
    u8g.drawStr(80,30, "|the ");
    u8g.drawStr(80,45, "|speed");
}

int findID(){                      //find the id which you press now,according to the value of "controller"
  for(int l=0;l<21;l++){
      if(controller==HEXN[l]){
          return l;
      }
    }
    return -1;
}
void modify(int *address){         //modify your value in your project, usage: modify(&valueName);
    int valueK;
    int www;
    valueK=0;
    www=key[maxKey]-1;
    for(int n=www;n>0;n--){
        valueK+=key[n]*thePower(key[maxKey]-n-1);
    }
   *address=valueK;   
}
void draw_Input(int l,int c) {
  u8g.setFont(u8g_font_unifont);
  if(l==1){                                        //if you want display "#" in line 1
      u8g.drawStr( 0, 30, " 4  5  6");
      u8g.drawStr( 0, 45, " 7  8  9");
      u8g.drawStr( 0, 60, " 0     .");
      if(c==1){                                 //if you want to display "# in font of column  1,it's number:1
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
////////////////////////////////////////////////////////////////////////////////////////////////////////
void page_modify(int * valueToM,int nextPage,int backPage){
  int number_press;
  int x,y;
  int numberP=-1;  //the buuton you pressed id
  numberP=findID();     //find the id of your pressed button
  display_Side();      //display the side
  if(numberP==9){       //if your pressed number "0"
      draw_Input(4,1);  //draw the picture which the "#" in font of 0
      updateValue(0);   //add 0 to the array key to modify in the reasonable time
  }
  else if(numberP==4){  //confirm
      flag=nextPage;            //the next page is page_1_1_1
      modify(valueToM);   //modify the value speed
  }
  else if(numberP==3){ //cansle
        flag=backPage;        //next value is page_1
  }
  else if(numberP>11){  //if you pressed number 
    number_press=numberP-11; //get the real number 
    if(updateF==1) {updateValue(number_press); updateF=0;}
    if((number_press%3)!=0){  //get the number's position
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
  draw_Input(x,y);  //display number 
  u8g.setPrintPos(160,60);
  u8g.print(flag);
} 
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
long getAlpha1(){
     long x;
     x=d+alpha;
     return x;
}
long getAlpha2(){
     long y;
     y=d-alpha;
     return y; 
}
void toThePosition(){
     setServoAlpla(1,getAlpha1());
     setServoAlpla(2,getAlpha2());
}
void PowerUp(){
     EAControl(1,1);
     EAControl(2,0);
}
void fire(){
     EAControl(1,0);
     EAControl(2,1);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void page_1(){                               
  u8g.setFont(u8g_font_unifont);
  u8g.drawStr(0, line*1, "0.read the value");
  u8g.drawStr(0, line*2, "1.subOne setting");
  u8g.drawStr(0 ,line*3, "2.subTwo setting");
  u8g.drawStr(0, line*4, "3.fire!");
  key[maxKey]=0;                             //reroad input data flag
  fireflag=0;
}
void page_0(){
  u8g.setFont(u8g_font_unifont);
  u8g.drawStr(0,line*1,"the distance is:");
  u8g.setPrintPos(0,line*2);
  u8g.print(d);
  u8g.drawStr(0,line*3,"the alphe is:");
  u8g.setPrintPos(0,line*4);
  u8g.print(alpha);
}
void page_1_1(){
  u8g.setFont(u8g_font_unifont);
  u8g.drawStr(0, line*1, "1.set distance");
}
void page_1_1_1(){
   page_modify(&d,9,2);
}

void page_1_2(){
  u8g.setFont(u8g_font_unifont);
  u8g.drawStr(0, line*1, "1.set distance");
  u8g.drawStr(0, line*2, "2.set angle");
}

void page_1_2_1(){
    page_modify(&d,9,4);
}

void page_1_2_2(){
    page_modify(&alpha,9,4);
}
void page_1_3(){
   u8g.setFont(u8g_font_unifont);
   u8g.drawStr(0, line*1, "To be launched");
   toThePosition();
   PowerUp();
   flag=8;
}
void page_1_3_ok(){
    if(fireflag==0)
   delay(powerUpTime);
   u8g.setFont(u8g_font_unifont);
   u8g.drawStr(0, line*1, "Launched");
   fire();
   fireflag=1;
}

void page_tips(){
  u8g.setFont(u8g_font_unifont);
  u8g.drawStr(0, line*1, "modify is finshed");
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void runtest(int n){
    switch (n){
      case 0:{
        u8g.firstPage();
        do{
          page_0();
        }while(u8g.nextPage());
        break;
      }
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
            page_1_1_1();
        }while( u8g.nextPage() );
        break;
      }
      case 4:{
        u8g.firstPage();  
        do {
            page_1_2();
        }while( u8g.nextPage());
        break;
      }
      case 5:{
        u8g.firstPage();  
        do {
            page_1_2_1();
        }while( u8g.nextPage());
        break;
      }
      case 6:{
        u8g.firstPage();  
        do {
            page_1_2_2();
        }while( u8g.nextPage());
        break;
      }
      case 7:{
        u8g.firstPage();  
        do {
            page_1_3();
        }while( u8g.nextPage());
        break;
      }
      case 8:{
        u8g.firstPage();  
        do {
            page_1_3_ok();
        }while( u8g.nextPage());
        break;
      }
      case 9:{
        u8g.firstPage();
      do{
          page_tips();
      }while(u8g.nextPage());
      break;
      }
    }
}
void test(){
  for(int i=0;i<90;i=i+0.1){
      servo1.write(i);
      delay(10);
  }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup(){      
  flag=1;   
  u8glibSet();      
  irrecv.enableIRIn();   
  servo1.attach(servo1Pin, 500, 2500); // pin8,
  servo2.attach(servo2Pin, 500, 2500); // pin9

  pinMode(vcc, OUTPUT);
  pinMode(IRPower, OUTPUT);
  pinMode(relay_1_power,OUTPUT);
  pinMode(relay_2_power,OUTPUT);
  pinMode(relay_2_gnd,OUTPUT);

  digitalWrite(relay_1_power,HIGH);
  digitalWrite(relay_2_power,HIGH);
  digitalWrite(relay_2_gnd,LOW);
  digitalWrite(vcc,HIGH); 
  digitalWrite(IRPower,HIGH);
  setServoAlpla(1,90);
  EAControl(1,0);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop() {
   updateF=0; 
   if (irrecv.decode(&results)) {  
        controller=results.value;     //storage key id
        updateF=1;
        if(flag==1&&controller==HEXN[9]) flag=0;
        else if(flag==1&&controller==HEXN[12]) flag=2;
        else if(flag==1&&controller==HEXN[13]) flag=4;
        else if(flag==1&&controller==HEXN[14]) flag=7;

        else if(flag==0&&controller==HEXN[back]) flag=1;
        else if(flag==2&&controller==HEXN[12]) flag=3;
        else if(flag==2&&controller==HEXN[back]) flag=1;
        else if(flag==4&&controller==HEXN[12]) flag=5;
        else if(flag==4&&controller==HEXN[13]) flag=6;
        else if(flag==4&&controller==HEXN[back]) flag=1;
        else if(flag==7) {flag=8;}
        else if(flag==8&&controller==HEXN[in]) {flag=1; EAControl(2,0);}
        else if(flag==9&&controller==HEXN[in]) flag=1;
        irrecv.resume();  
  }
  runtest(flag);
}