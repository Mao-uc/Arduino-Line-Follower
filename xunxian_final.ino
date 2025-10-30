#include<ADS1115_WE.h> 
#include<Wire.h>
#define I2C_ADDRESS 0x48
#define EN1_RATE 1.0
#define EN2_RATE 0.95
int EN1 = 6;
int EN2 = 5;
int IN1 = 9;
int IN2 = 8;
int IN3 = 3;
int IN4 = 2;
ADS1115_WE adc = ADS1115_WE(I2C_ADDRESS);
int irSensorDigital[3] = {0, 0, 0};
int threshold = 200;
int irSensors = B000;
int error = 0;
int errorLast = 0;
volatile int speed;
void motor(int m, int dir, int speed) {
  int pinEN = 0;
  int pinIN1 = 0;
  int pinIN2 = 0;
  float enRate = 0;
  switch (m) {
    case 1:
      pinEN = EN1;
      pinIN1 = IN1;
      pinIN2 = IN2;
      enRate = EN1_RATE;
      break;
    case 2:
      pinEN = EN2;
      pinIN1 = IN3;
      pinIN2 = IN4;
      enRate = EN2_RATE;
      break;
    default:
      return;
      break;
  }
  switch (dir) {
    case 0:
      digitalWrite(pinIN1, 0);
      digitalWrite(pinIN2, 0);
      analogWrite(pinEN, 0);
      break;
    case 1:
      digitalWrite(pinIN1, 1);
      digitalWrite(pinIN2, 0);
      analogWrite(pinEN, speed * enRate);
      break;
    case -1:
      digitalWrite(pinIN1, 0);
      digitalWrite(pinIN2, 1);
      analogWrite(pinEN, speed * enRate);
      break;
    default:
      return;
      break;
  }
}
void forward() {
  motor(1, 1, speed+70);
  motor(2, 1, speed+70);
}
void lowforward() {
  motor(1, 1, speed);
  motor(2, 1, speed);
}
void back() {
  motor(1, -1, speed);
  motor(2, -1, speed);
}
void left() {
  motor(1, -1, speed+80);
  motor(2, 1, speed+80);
}
void sleft() {
  motor(1, -1, speed+30);
  motor(2, 1, speed+30);
}
void ssleft() {
  motor(1, -1, speed+30);
  motor(2, 1, speed+30);
}
void right() {
  motor(1, 1, speed+80);
  motor(2, -1, speed+80);
}
void sright() {
  motor(1, 1, speed+30);
  motor(2, -1, speed+30);
}
void ssright() {
  motor(1, 1, speed+30);
  motor(2, -1, speed+30);
}
void stop() {
  motor(1, 0, speed);
  motor(2, 0, speed);
}
void Scan(){
  irSensors = B000;
  for (int i = 0; i < 3; i++) {
    if(i==0){
      irSensorDigital[i] = readChannel(ADS1115_COMP_0_GND);
    }else if(i==1){
      irSensorDigital[i] = readChannel(ADS1115_COMP_1_GND);
    }else if(i==2){
      irSensorDigital[i] = readChannel(ADS1115_COMP_2_GND);
    }
    int b = 2 - i;
    irSensors = irSensors + (irSensorDigital[i] << b);
  }
}
int stopflag=0;
int count=0;
int lowcount=0;
void UpdateDirection(int xjspeed) {
  speed=xjspeed;
  errorLast = error;
  Serial.println(irSensors);
  switch (irSensors) {
    case B000:  // no sensor detects the line
      if (errorLast < 0) {
        left();               
        error = -1;
      } else if (errorLast > 0) {
        right();
        error = 1;
      }
      count=0;
      lowcount=0;
      stopflag=1;
      break;
    case B100:  
      sleft(); 
      error = -1;
      count=0; 
      lowcount=0;  
      stopflag=1;        
      break;
    case B110:
      ssleft();
      error = -1;
      count=0;
      lowcount=0;
      stopflag=1;
      break;
    case B010:
      if(count<30){
        forward();
        count++;
      }else if(lowcount<4){
        lowforward();
        lowcount++;
      }else if(lowcount>4){
        forward();
        count=0;
      }
      error = 0;
      stopflag=1;
      break;
    case B011:
      ssright();
      error = 1;
      count=0;
      lowcount=0;
      stopflag=1;
      break;
    case B001: 
      sright();
      error = 1;
      count=0;
      lowcount=0;
      stopflag=1;
      break;
    case B111:
      error = 1;
      stopflag=1;
      forward();
      break;
    default:
      if(stopflag==1){
        stop();
        stopflag=0;
      }else{
        stopflag=1;
      }
      error = errorLast;
      count=0;
      lowcount=0;
  }
}
void setup() {
  Wire.begin();
  Wire.setClock(400000);
  Serial.begin(9600);
  if(!adc.init()){
    Serial.println("ADS1115 not connected!");
  }
  adc.setVoltageRange_mV(ADS1115_RANGE_6144);
  adc.setConvRate(ADS1115_860_SPS);
  adc.setMeasureMode(ADS1115_SINGLE);   
  speed = 90;
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(EN1, OUTPUT);
  pinMode(EN2, OUTPUT);
  Serial.begin(9600);
}
void loop() {
  Scan();
  UpdateDirection(speed);
}
int readChannel(ADS1115_MUX channel) {
  adc.setCompareChannels(channel);
  adc.startSingleMeasurement();
  while (adc.isBusy()) {
    delay(1);
  }
  float voltage = adc.getResult_V();
  return voltage > 1;  
}