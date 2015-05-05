/*
 * ssd1306.c
 *
 *  Created on: 05-May-2015
 *      Author: root
 */

#include<avr/io.h>
#include <string.h>
#include <avr/pgmspace.h>
#include "ssd1306.h"
#include "i2c_dev.h"
#include "font.h"

void sendcommand(unsigned char com){
	i2c_write(0x80,com);
}

void SendChar(unsigned char data){
	i2c_write(0x40,data);
}



//==========================================================//
// Turns display on.
void displayOn(void)
{
    sendcommand(0xaf);        //display on
}

//==========================================================//
// Turns display off.
void displayOff(void)
{
  sendcommand(0xae);		//display off
}

//==========================================================//
// Set the cursor position in a 16 COL * 8 ROW map.
void setXY(unsigned char row,unsigned char col)
{
  sendcommand(0xb0+row);                //set page address
  sendcommand(0x00+(8*col&0x0f));       //set low col address
  sendcommand(0x10+((8*col>>4)&0x0f));  //set high col address
}


//==========================================================//
// Prints a string in coordinates X Y, being multiples of 8.
// This means we have 16 COLS (0-15) and 8 ROWS (0-7).
void sendStrXY( char *string, int X, int Y)
{
  setXY(X,Y);
  unsigned char i=0;
  while(*string)
  {
    for(i=0;i<8;i++)
    {
      SendChar(pgm_read_byte(myFont[*string-0x20]+i));
    }
    *string++;
  }
}


//==========================================================//
// Inits oled and draws logo at startup
void init_OLED(void)
{
	InitI2C();

  sendcommand(0xae);		//display off
  sendcommand(0xa6);            //Set Normal Display (default)
    // Adafruit Init sequence for 128x64 OLED module
    sendcommand(0xAE);             //DISPLAYOFF
    sendcommand(0xD5);            //SETDISPLAYCLOCKDIV
    sendcommand(0x80);            // the suggested ratio 0x80
    sendcommand(0xA8);            //SSD1306_SETMULTIPLEX
    sendcommand(0x3F);
    sendcommand(0xD3);            //SETDISPLAYOFFSET
    sendcommand(0x0);             //no offset
    sendcommand(0x40 | 0x0);      //SETSTARTLINE
    sendcommand(0x8D);            //CHARGEPUMP
    sendcommand(0x14);
    sendcommand(0x20);             //MEMORYMODE
    sendcommand(0x00);             //0x0 act like ks0108

    sendcommand(0xA0 | 0x1);      //SEGREMAP   //Rotate screen 180 deg
    //sendcommand(0xA0);

    sendcommand(0xC8);            //COMSCANDEC  Rotate screen 180 Deg
    //sendcommand(0xC0);

    sendcommand(0xDA);            //0xDA
    sendcommand(0x12);           //COMSCANDEC
    sendcommand(0x81);           //SETCONTRAS
    sendcommand(0xCF);           //
    sendcommand(0xd9);          //SETPRECHARGE
    sendcommand(0xF1);
    sendcommand(0xDB);        //SETVCOMDETECT
    sendcommand(0x40);
    sendcommand(0xA4);        //DISPLAYALLON_RESUME
    sendcommand(0xA6);        //NORMALDISPLAY

  clear_display();
  sendcommand(0x2e);            // stop scroll
  //----------------------------REVERSE comments----------------------------//
  //  sendcommand(0xa0);		//seg re-map 0->127(default)
  //  sendcommand(0xa1);		//seg re-map 127->0
  //  sendcommand(0xc8);
  //  delay(1000);
  //----------------------------REVERSE comments----------------------------//
  // sendcommand(0xa7);  //Set Inverse Display
  // sendcommand(0xae);		//display off
  sendcommand(0x20);            //Set Memory Addressing Mode
  sendcommand(0x00);            //Set Memory Addressing Mode ab Horizontal addressing mode
  //  sendcommand(0x02);         // Set Memory Addressing Mode ab Page addressing mode(RESET)

   setXY(0,0);
  /*
  for(int i=0;i<128*8;i++)     // show 128* 64 Logo
  {
    SendChar(pgm_read_byte(logo+i));
  }
  */
  sendcommand(0xaf);		//display on
}

//==========================================================//
// Clears the display by sendind 0 to all the screen map.
void clear_display(void)
{
  unsigned char i,k;
  for(k=0;k<8;k++)
  {
    setXY(k,0);
    {
      for(i=0;i<128;i++)     //clear all COL
      {
        SendChar(0);         //clear all COL
        //delay(10);
      }
    }
  }
}


//==========================================================//
// Resets display depending on the actual mode.
void reset_display(void)
{
  displayOff();
  clear_display();


  displayOn();
}

