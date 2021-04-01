#include <Arduino.h> 

//data display from right to left, from bottom to top, HIGH level display. 
#define IIC_SCL  2
#define IIC_SDA  3

void led_setup();
void from_progmem(const uint8_t *progmem, unsigned char *buf, unsigned int len);
void led_show(const unsigned char *picture);
void led_rotate(const uint8_t *progmem, uint8_t size);
void led_show_number(uint8_t n);

extern unsigned char led_buf[16];

extern const unsigned char digit[][8];
extern const unsigned char icons16[][16];
extern const unsigned char icons32[][32];
