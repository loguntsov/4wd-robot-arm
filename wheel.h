#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>


enum direction_t {
  FORWARD = true,
  BACKWARD = false
};

class wheel {
  protected:
    Adafruit_PWMServoDriver * pwm;
  public:
    // const __FlashStringHelper *name;  
    byte port;
    direction_t direction = FORWARD;
    int speed;

    wheel(Adafruit_PWMServoDriver * pwm, byte port);
    direction_t static opposite_direction(direction_t dir);
    void move();
    void stop();
};
