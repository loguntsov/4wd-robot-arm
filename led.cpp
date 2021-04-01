//#include <stdint.h>
//#include <avr/pgmspace.h>
#include <Arduino.h> 

#include "led.h"

const unsigned char PROGMEM digit[][8] = {
  { 0x00, 0x3c, 0x42, 0x81, 0x81, 0x42, 0x3c, 0x00 }, // 0
  { 0x00, 0x08, 0x04, 0x02, 0xff, 0x00, 0x00, 0x00 }, // 1
  { 0x00, 0x84, 0xc2, 0xa1, 0xa1, 0x92, 0x8c, 0x00 }, // 2 
  { 0x00, 0x00, 0x42, 0x81, 0x81, 0x99, 0x66, 0x00 }, // 3
  { 0x00, 0x00, 0x1f, 0x10, 0x10, 0xff, 0x00, 0x00 }, // 4
  { 0x00, 0x00, 0x8f, 0x89, 0x89, 0xf9, 0x00, 0x00 }, // 5
  { 0x00, 0x00, 0xff, 0x89, 0x89, 0xf9, 0x00, 0x00 }, // 6 
  { 0x00, 0x00, 0x01, 0x01, 0xf1, 0x09, 0x07, 0x00 }, // 7
  { 0x00, 0x00, 0xff, 0x89, 0x89, 0xff, 0x00, 0x00 }, // 8
  { 0x00, 0x00, 0x8f, 0x89, 0x89, 0xff, 0x00, 0x00 } // 9
};

const unsigned char PROGMEM icons16[][16] = {
  {0x00,0x00,0x00,0x00,0x26,0x41,0x86,0x80,0x80,0x80,0x86,0x41,0x26,0x00,0x00,0x00} // "-)
 ,{0x00,0x00,0x00,0x00,0x00,0x1C,0x22,0x42,0x84,0x42,0x22,0x1C,0x00,0x00,0x00,0x00} // heart
 ,{0x00,0x00,0x00,0x00,0x20,0x44,0x42,0x84,0x80,0x84,0x42,0x44,0x20,0x00,0x00,0x00} // :-)                          
 ,{0x00,0x00,0x00,0x00,0xC0,0x40,0xF8,0xD8,0x7E,0xFF,0xC0,0x00,0x00,0x00,0x00,0x00} // Д-русская
 ,{0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x22, 0x42, 0x84, 0x42, 0x22, 0x1c, 0x00, 0x00, 0x00, 0x00} // "-(
 ,{0x00, 0x00, 0xff, 0x08, 0x08, 0xff, 0x00, 0x00, 0x00, 0xfb, 0xfb, 0x00, 0x00, 0x00, 0xbf, 0x00} // Hi !
 ,{0x00, 0x00, 0x00, 0x01, 0x62, 0x92, 0x91, 0x90, 0x90, 0x90, 0x91, 0x92, 0x62, 0x01, 0x00, 0x00} // :-O
};

const unsigned char PROGMEM icons32[][32] = {
 { 0xff, 0x08, 0x08, 0xff, 0x00, 0xff, 0x89, 0x89, 0x89, 0x00, 0xff, 0x80, 0x80, 0xc0, 0x00, 0xff, 0x80, 0x80, 0xc0, 0x00, 0x7e, 0x81, 0x81, 0x7e,0,0,0,0,0,0,0,0 }, // HELLO
 { 0x00, 0x86, 0x89, 0x89, 0x71, 0x00, 0x01, 0x01, 0xff, 0x01, 0x01, 0x00, 0x7e, 0x81, 0x81, 0x81, 0x7e, 0x00, 0xff, 0x11, 0x11, 0x11, 0x0e, 0x00, 0,0,0,0,0,0,0,0 }, // STOP
 { 0x00, 0x00, 0x00, 0x81, 0x42, 0x24, 0x18, 0x00, 0x00, 0x00, 0x00, 0x81, 0x42, 0x24, 0x18, 0x00, 0x00, 0x00, 0x81, 0x42, 0x24, 0x18, 0x00, 0x00, 0x00, 0x81, 0x42, 0x24, 0x18, 0x00, 0x00, 0x00 }, // > > >
 { 0x00, 0x00, 0x00, 0x00, 0x18, 0x24, 0x42, 0x81, 0x00, 0x00, 0x00, 0x18, 0x24, 0x42, 0x81, 0x00, 0x00, 0x00, 0x18, 0x24, 0x42, 0x81, 0x00, 0x00, 0x00, 0x18, 0x24, 0x42, 0x81, 0x00, 0x00, 0x00 }, // < < <
 { 0x40, 0x40, 0x20, 0x10, 0x08, 0x08, 0x10, 0x20, 0x40, 0x40, 0x20, 0x10, 0x08, 0x08, 0x10, 0x20, 0x40, 0x40, 0x20, 0x10, 0x08, 0x08, 0x10, 0x20, 0x40, 0x40, 0x20, 0x10, 0x08, 0x08, 0x10, 0x20 }, // _/^\_/^\_ (WAVE) 
 { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01, 0x01, 0x01, 0x01 } // BIG WAVE  
};

unsigned char led_buf[16];

void led_setup() 
{
  pinMode(IIC_SCL,OUTPUT);
  pinMode(IIC_SDA,OUTPUT);
  digitalWrite(IIC_SCL,LOW);
  digitalWrite(IIC_SDA,LOW);
}

void IIC_start();
void IIC_send(unsigned char send_data);
void IIC_end();

void from_progmem(const uint8_t *progmem, unsigned char *buf, unsigned int len) {
  for (; len > 0; len--) {
    *buf = pgm_read_byte(progmem);
    progmem++;
    buf++;
  }
}

/*----------------------------------------------------------------*/
void led_show(const unsigned char *picture) {
    /**************set the address plus 1***************/
    IIC_start();
    IIC_send(0x40);// set the address plus 1 automatically
    IIC_end();
    /************end the process of address plus 1 *****************/
    /************set the data display*****************/ 
    IIC_start();
    IIC_send(0xc0);// set the initial address as 0
    for(char i = 0;i < 16;i++) {
       IIC_send(*picture);// send the display data 
       picture++;
    }
    IIC_end();
    /************end the data display*****************/
    /*************set the brightness display***************/ 
    IIC_start();
    IIC_send(0x8A);// set the brightness display
    IIC_end(); 
    /*************end the brightness display***************/ 
}
/*----------------------------------------------------------------*/
void led_rotate(const uint8_t *progmem, uint8_t size){
   static int delta=0;
   static uint8_t *old_progmem = NULL;
   if (old_progmem != progmem) {
    old_progmem = progmem;
    delta = 16;
    from_progmem(progmem, led_buf, 16);
   } else {
     for(uint8_t i=0;i<15;i++) {
      *(led_buf+i)=*(led_buf+i+1);
     }
     if (delta < size ) { 
      from_progmem(progmem + delta, led_buf + 15, 1);
     } else {
      delta = -1;
     }
   }
   led_show(led_buf);
   delta++;  
}

void led_show_number(uint8_t n) {
  Serial.println(n);
  uint8_t a,b;
  n = min(99, n);
  a = ( n / 10 ) % 10;
  b = n % 10;
  from_progmem(digit[a], led_buf, 8);
  from_progmem(digit[b], led_buf+8, 8);
  led_show(led_buf);
}

void IIC_start() {
  digitalWrite(IIC_SCL,LOW);
  delayMicroseconds(3);
  digitalWrite(IIC_SDA,HIGH);
  delayMicroseconds(3);
  digitalWrite(IIC_SCL,HIGH);
  delayMicroseconds(3);
  digitalWrite(IIC_SDA,LOW);
  delayMicroseconds(3);
}

void IIC_send(unsigned char send_data)
{
  for(char i = 0;i < 8;i++)
  {
      digitalWrite(IIC_SCL,LOW);
      delayMicroseconds(3); 
      if(send_data & 0x01)
      {
        digitalWrite(IIC_SDA,HIGH);
      }
      else
      {
        digitalWrite(IIC_SDA,LOW);
      }
      delayMicroseconds(3);
      digitalWrite(IIC_SCL,HIGH); 
      delayMicroseconds(3);
      send_data = send_data >> 1;
  }
}
void IIC_end()
{
  digitalWrite(IIC_SCL,LOW);
  delayMicroseconds(3);
  digitalWrite(IIC_SDA,LOW);
  delayMicroseconds(3);
  digitalWrite(IIC_SCL,HIGH);
  delayMicroseconds(3);
  digitalWrite(IIC_SDA,HIGH);
  delayMicroseconds(3);
}
/*----------------------------------------------------------------*/