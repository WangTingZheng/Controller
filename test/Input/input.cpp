#include "U8glib.h"
#include <Arduino.h>
int x;
int y;
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE|U8G_I2C_OPT_DEV_0);	// I2C / TWI 

void draw(int l,int c) {
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
}

void setup(void) {
  x=1;
  y=1;
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

void loop(void) {
  u8g.firstPage();  
  do {
    draw(x,y);
  } while( u8g.nextPage() );
  
  if(x==4&&y==3){x=1;y=1;}
  else if(y==3) {
      x++;
      y=1;
  }
  else y++;
  delay(50);
}