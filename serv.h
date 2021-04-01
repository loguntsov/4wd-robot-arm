#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

typedef int t_angle;

class serv {  
  protected:
    Adafruit_PWMServoDriver * pwm;
    void write(int angle);    
  public:    
    const __FlashStringHelper *name;  
    byte port;
    int min = 0, max = 180, pos = 0, pos_init = 0;
    int delay = 5;
    serv(Adafruit_PWMServoDriver  * pwm);
    void init(const __FlashStringHelper * name, byte port, t_angle angle);
    t_angle move(t_angle angle);
    t_angle move_on(t_angle angle);    
    t_angle go_init();
};
