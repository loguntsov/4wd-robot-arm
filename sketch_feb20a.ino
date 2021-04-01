
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <Servo.h>
#include <IRremote.h>

#include <Thread.h>

#include "led.h"
#include "serv.h"
#include "wheel.h"

static Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x47);

const int RECV_PIN = A0;
const int echoPin = 13; // ultrasonic module   ECHO to D13
const int trigPin = 12; // ultrasonic module  TRIG to D12
static IRrecv irrecv(RECV_PIN);

#define IR_Go       0x00ff629d
#define IR_Back     0x00ffa857
#define IR_Left     0x00ff22dd
#define IR_Right    0x00ffc23d
#define IR_Ok       0x00ff02fd
#define IR_1        0x00FF6897
#define IR_2        0x00FF9867
#define IR_3        0x00FFB04F
#define IR_4        0x00FF30CF
#define IR_5        0x00FF18E7
#define IR_6        0x00FF7A85
#define IR_7        0x00FF10EF
#define IR_8        0x00FF38C7
#define IR_9        0x00FF5AA5
#define IR_0        0x00FF4AB5
#define IR_Sharp    0x00FF52AD
#define IR_Star     0x00FF42BD
#define IR_LastButton 0xFFFFFFFF

#define SensorLeft    6   //input pin of left sensor
#define SensorMiddle  7   //input pin of middle sensor
#define SensorRight   8   //input pin of right sensor

#define DELAY 500

static unsigned char SL;        //state of left sensor
static unsigned char SM;        //state of middle sensor
static unsigned char SR;        //state of right sensor

static unsigned long irLastAction = 0;

#define DStop 0
#define DForward 1
#define DBack 2
#define DLeft 3
#define DRight 4

unsigned char direction = DStop;
direction_t dir = FORWARD;

int i = 0, j = 0, t = 0, speeds = 1500;


serv servo1(&pwm);
serv servo2(&pwm);
serv servo3(&pwm);

wheel wheelFL(&pwm, 0);
wheel wheelBL(&pwm, 2);
wheel wheelFR(&pwm, 4);
wheel wheelBR(&pwm, 6);

void setup() {
  Serial.begin(9600); //set baud rate to 9600
  Serial.println(F("Hello. This is Bob"));

  pinMode(A3, INPUT);
  analogReference(INTERNAL); // 1.1V internal base voltage of ADC  
  pwm.begin();
  pwm.setPWMFreq(60);
  led_setup();


  from_progmem(icons16[5], led_buf, 16); 
  led_show(led_buf);
  delay(1000);
  from_progmem(icons16[6], led_buf, 16); 
  led_show(led_buf);
  delay(1000);
  
  Serial.println(F("PWF inited"));
  stopp();
  Serial.println(F("Wheels stopped"));  
  servo1.init(F("servo1"), 15, 70);
  servo1.min = 40;  
  servo1.max = 70;

  servo2.init(F("servo2"), 14, 120);
  servo2.min = 20;
  servo2.max = 120;  
  
  servo3.init(F("servo3"), 13, 90);
  servo3.min = 0;
  servo3.max = 180;
  servo3.delay = 15;
 
  delay(1000);
  pinMode(SensorLeft, INPUT);
  pinMode(SensorMiddle, INPUT);
  pinMode(SensorRight, INPUT);

  irrecv.enableIRIn(); // enable IR receiver
  stopp();
}

void loop() {
    if (Serial.available() > 0) { //receive Bluetooth signals
    char blue_val = Serial.read();
    Serial.println(blue_val);
    switch (blue_val) {
      case 'F': stopp();forward(); Serial.println("forward"); break; //receive‘F’, go forward

      case 'B': stopp();back();   Serial.println("back");   break; //receive‘B’, go back
      case 'L': stopp();turnL();  Serial.println("turn left"); break; //receive‘L’, turn left
      case 'R': stopp();turnR();  Serial.println("turn right"); break; //receive‘R’, turn right
      case 'S': stopp();  Serial.println("stop");   break; //receive‘S’, Car stops
      case 'a': speeds_inc(); break;//receive‘a’, motor speeds up
      case 'd': speeds_dec(); break;//receive‘d’, motor slows down    
      case 'f': claw_up(); break;     //robots arm lifts up
      case 'b': claw_down(); break;     //robots arm lowers

      case 'l': claw_left(); break;     //robot arm turns left

      case 'r': claw_right(); break;     //robot arm turns right

      case 'Q': claw_open(); break;      //claw opens

      case 'E': claw_close(); break;     //claw closes

      //case 't': read_servo(); break;  //receive‘t’, record action

      //case 'i': do_servo(); break;   //receive‘i’, execute motion

      //case 'Y': avoid();    Serial.println("avoid"); break;  //receive‘Y’ to enter the obstacle avoidance mode

      //case 'X': tracking(); Serial.println("tracking"); break;      //receive‘X’to enter line tracking mode

      //case 'U': follow();   Serial.println("follow"); break; //receive‘U’to enter ultrasonic follow mode
      //case 'G': Fall();     Serial.println("Fall"); break;      //receive‘G’to enter anti-fall mode
     
      default: break;
    }
  }

  decode_results results;
  if (irrecv.decode(&results)) {  //receive IR signals
    Serial.println("Current");
    Serial.println(results.value, HEX);
    Serial.println("Last");    
    Serial.println(irLastAction, HEX);
    unsigned long action = results.value;
    if (action == IR_LastButton) {
      action = irLastAction;
    } else {
      irLastAction = action;
    }
    
    switch (action) {
      case IR_Go: stopp();forward();  break;
      case IR_Back: stopp();back();   break;
      case IR_Left: stopp();turnL();  break;
      case IR_Right: stopp();turnR();  break;
      case IR_Ok: stopp();  break;
      case IR_1 : speeds_inc(); break;
      case IR_3 : speeds_dec(); break;      
      case IR_5 : claw_init();break;      
      case IR_2:  claw_up(); break;     //robots arm lifts up
      case IR_8:  claw_down(); break;  //robots arm lowers
      case IR_4:  claw_left(); break;     //robot arm turns left
      case IR_6:  claw_right(); break;     //robot arm turns right
      case IR_0:  claw_toogle(); break;
      case IR_Sharp: claw_close(); break;     //claw closes
      case IR_Star: claw_open(); break;      //claw opens    
      default:  break;
    }
    irrecv.resume();  //
  }
  check_distance();
  //follow(); // не работает
}

void claw_init() {
  servo1.go_init();
  servo2.go_init();
  servo3.go_init();
}

void claw_toogle() {
  if (!is_claw_open()) {
    claw_open();
  } else
  {
    claw_close();
  }
};

bool is_claw_open() {
  //int val = map(analogRead(A3), 666, 1023, 20, 70);
  //led_show_number(val);
  return servo1.pos < 55;
}

void check_distance() {
  if (is_claw_open()) return;
  int distance = get_distance();
  if (distance < 20 && (direction != DBack && direction != DStop )) {
    stopp();
    servo3.move(30);
    delay(1000);
    if (get_distance() > 20) {
      turnR();
      delay(500);
      stopp();      
      if (get_distance() > 20) { forward(); return; }
    }
    servo3.move(150);
    delay(1000);
    if (get_distance() > 20) {
      turnL();
      delay(500);
      stopp();
      if (get_distance() > 20) { forward(); return; }
    }
    back();
    delay(1000);
    if (rand() % 2 == 0 ) turnL(); else turnR();
    delay(500);
    forward();
  }
}

void follow() {
    static int prev_distance = 0;
    int distance = get_distance();
    if ((distance > 40 && distance < 60) && (abs(distance - prev_distance) < 20)) {
      prev_distance = distance;
      stopp();
      return;
    }
    if (distance < 40) { back(); return; }
    if (distance >= 60 && distance < 120) {
      prev_distance = distance;
      forward();
      return;
    }
    stopp();
    servo3.move(30);
    delay(1000);
    int d1 = get_distance();
    servo3.go_init();
    delay(1000);   
    int d2 = get_distance();    
    servo3.move(150);
    delay(1000);
    int d3 = get_distance();

    if (d1 > d3 + 20 && d1 > d2 + 20 && d1 > 20 && d1 < 120) {
      turnR();
      delay(300);      
      forward();
      return;
    } 
    if (d2 > d1 + 20 && d2 > d3 + 20 && d2 > 20 && d2 < 120) {
      servo3.go_init();      
      forward();
      return;     
    }
    if (d3 > d1 + 20 && d3 > d2 + 20 && d3 > 20 && d3 < 120) {
      turnL();
      delay(300);
      forward();
      return;
    }    
}


void set_speed(int speed) {
  wheelFL.speed = speeds;
  wheelBL.speed = speeds;
  wheelFR.speed = speeds;
  wheelBR.speed = speeds;
  move();
  
}

void forward() {

  if (!is_claw_open()) claw_init();
  wheelFL.speed = speeds;
  wheelFL.direction = FORWARD;
  wheelFL.move();

  wheelBL.speed = speeds;
  wheelBL.direction = FORWARD;
  wheelBL.move();  

  wheelFR.speed = speeds;
  wheelFR.direction = FORWARD;
  wheelFR.move();

  wheelBR.speed = speeds;
  wheelBR.direction = FORWARD;
  wheelBR.move();    
  
  direction = DForward;
  dir = FORWARD;
  if (is_claw_open()) {
    delay(100);stopp();
  }     
}

void back() {
  wheelFL.speed = speeds;   
  wheelFL.direction = BACKWARD;
  wheelFL.move();

  wheelBL.speed = speeds;
  wheelBL.direction = BACKWARD;
  wheelBL.move();  

  wheelFR.speed = speeds;
  wheelFR.direction = BACKWARD;
  wheelFR.move();

  wheelBR.speed = speeds;
  wheelBR.direction = BACKWARD;
  wheelBR.move();
  direction = DBack;
  dir = BACKWARD;
  
  if (is_claw_open()) {
    delay(100);
    stopp();
  }  
}

void turnL() {
  if (!is_claw_open()) servo3.move(150);
  wheelFL.speed = speeds*2;
  wheelFL.direction = wheel::opposite_direction(dir);
  wheelFL.move();

  wheelBL.speed = speeds*2;
  wheelBL.direction = wheel::opposite_direction(dir);
  wheelBL.move();  

  wheelFR.speed = speeds*2;
  wheelFR.direction = dir;
  wheelFR.move();

  wheelBR.speed = speeds*2;
  wheelBR.direction = dir;  
  wheelBR.move();    
  
  if (is_claw_open()) {
    delay(100);stopp();
  }  
  
}
void turnR() {
  if (!is_claw_open()) servo3.move(30);
  wheelFL.speed = speeds*2;
  wheelFL.direction = dir;  
  wheelFL.move();

  wheelBL.speed = speeds*2;
  wheelBL.direction = dir;  
  wheelBL.move();  

  wheelFR.speed = speeds*2;
  wheelFR.direction = wheel::opposite_direction(dir);
  wheelFR.move();

  wheelBR.speed = speeds*2;
  wheelBR.direction = wheel::opposite_direction(dir);
  wheelBR.move();
  
  if (is_claw_open()) {
    delay(100);
    stopp();
  }   
}

void move() {
  wheelFL.move();
  wheelBL.move();  
  wheelFR.move();
  wheelBR.move();   
}

void stopp() {
  wheelFL.stop();
  wheelFL.direction = FORWARD;
  wheelBL.stop();  
  wheelBL.direction = FORWARD;  
  wheelFR.stop();
  wheelFR.direction = FORWARD;  
  wheelBR.stop();  
  wheelBL.direction = FORWARD;  
  direction = DStop;
}

int get_distance() {
  unsigned int sum = 0;  
  for(int i = 0; i<10; i++) {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(5);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    int distance = pulseIn(echoPin, HIGH);  // reading the duration of high level
    sum += distance / 58; // Transform pulse time to distance
    delay(5);
  }
  sum = sum / 10;  
  led_show_number((uint8_t) sum);
  return sum;
}

void speeds_inc() {
  if (speeds <= 2250) { //speed up by 5, set the number to change speed, up to 2560
    speeds += 250;
    set_speed(speeds);            
  }
}

void speeds_dec() {
  if (speeds >= 255) { //reduce to 0 at least
    speeds -= 250;
    set_speed(speeds);       
  }     
}


void claw_open() { //claw opens
  servo1.move(servo1.min);
}
void claw_close() { //claw closes
  servo1.move(servo1.max);
}

void claw_up() { //robot arm lifts up
  servo2.move_on(10);
}

void claw_down() { //robot arm lowers
  servo2.move_on(-10);
}

void claw_left() { //robot arm turns left
  servo3.move_on(10);
}

void claw_right() { //robot arm turns right
  servo3.move_on(-10);
}


bool is_not_stop() {
  if (Serial.read() == 'S') {
    return false; //S’to stop speeding up 
  } else {
    return true;
  }
}

// ---------------------------------------------------------------------
