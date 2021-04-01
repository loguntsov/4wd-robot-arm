// -------------------------------------------------

#include "serv.h"

serv::serv(Adafruit_PWMServoDriver * pwm) {
  this->pwm = pwm;
}

void serv::init(const __FlashStringHelper * name, byte port, t_angle angle) {
  this->pos = angle;
  this->pos_init = angle;
  this->port = port;
  this->name = name;
  Serial.println(name);
  this->write(angle);
}

void serv::write(int angle) {
  int value = map(angle, 0, 180, 133, 589);
  this->pwm->setPWM(this->port, 0, value);
}

t_angle serv::move(t_angle angle) {
  t_angle p = this->pos;
  if (angle < this->min) angle = this->min;
  if (angle > this->max) angle = this->max;  
  int delta = (p > angle) ? -1 : 1;
  while(p != angle ) {  
    p += delta;
    Serial.println(this->name);
    this->write(p);
    Serial.println(p);    
    ::delay(this->delay);
    int val = analogRead(A3);
    Serial.println(val);        
    Serial.println(map(val, 666, 1023, 20, 70));     
  }
  this->pos = angle;
  return angle;
}

t_angle serv::move_on(t_angle angle) {
  return this->move(this->pos + angle);
}

t_angle serv::go_init() {
  return this->move(this->pos_init);
}
