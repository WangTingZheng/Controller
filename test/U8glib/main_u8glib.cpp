#include <Arduino.h>
#include <U8glib.h>
#include <IRremote.h>   //IR receiver
#define line 15
#define column 10
#define maxKey 50   //the max length of the array which include the match you input
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

int RECV_PIN = 11;   //IR revicer pin
long  controller=0;    // storage key hex id
int   speed=100;      //the value i want to modify
int   flag;           //the flag target differernt page
int   updateF;       //the updateValue's flag


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

void page_1(){                               
  u8g.setFont(u8g_font_unifont);
  u8g.drawStr(0, line*1, "May I help you?");
  u8g.drawStr(0, line*2, "1.modify speed.");
  u8g.drawStr(0 ,line*3, "2.read speed.");
  key[maxKey]=0;                             //reroad input data flag
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
void page_1_1(){
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
      flag=4;            //the next page is page_1_1_1
      modify(&speed);   //modify the value speed
  }
  else if(numberP==3){ //cansle
        flag=1;        //next value is page_1
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
  irrecv.enableIRIn();                           //enable IR revicer 
}
void loop() {
   updateF=0;
   if (irrecv.decode(&results)) {  
        controller=results.value;     //storage key id
        updateF=1;
        if(flag==1&&controller==HEXN[12]) flag=2;     //if we are in page_1, and you press number"1", the next page is page_1_1
        else if(flag==1&&controller==HEXN[13]) flag=3; //in page_1,pressed number "2", the next is page_1_2;
        else if(flag==3&&controller==HEXN[3])  flag=1; //in page_1_2, pressed button "playback", the next is page_1
        else if(flag==4){delay(300);flag=1;}    //in page_1_1_1,pressed button "play", the next is page_1
        irrecv.resume();  
  }
  runtest(flag);
}