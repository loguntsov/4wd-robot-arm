#include "wheel.h"

wheel::wheel(Adafruit_PWMServoDriver * pwm, byte port) {
  this->pwm = pwm;
  this->port = port;
}

void wheel::move() {
  this->pwm->setPWM(this->port, 0, this->direction ? 4095 : 0);
  this->pwm->setPWM(this->port+1, 0, min(4095, this->speed));
}

void wheel::stop() {
  this->speed=0;
  move();
}

direction_t static wheel::opposite_direction(direction_t dir) {
  if (dir == FORWARD) return BACKWARD;  
  return FORWARD;
}
