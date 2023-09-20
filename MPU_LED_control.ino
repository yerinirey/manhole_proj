#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <LedControl.h>
#include <SoftwareSerial.h>
#include <string.h>
Adafruit_MPU6050 mpu;
SoftwareSerial BT_serial(3,2);       //Tx->3, Rx->2
LedControl lc = LedControl(8, 10, 9, 1); // (DIN, CLK, CS, number)

unsigned long timePrev;   //지난시간
unsigned long timeCurr;
bool checkTimePrev =false;
bool isLedOn = false; // LED 상태를 저장하는 변수
byte pattern[] = {B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111};


void setup(void) {
  Serial.begin(9600);
  BT_serial.begin(9600);
  if(!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while(1){
      delay(10);
    }
  }
//  Serial.println("MPU6050 Found!");
  // set accelerometer range to +-8G
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);

  // set gyro range to +- 500 deg/s
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);

  // set filter bandwidth to 21 Hz
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  
  lc.shutdown(0, false);
  lc.setIntensity(0, 15); // 밝기
  lc.clearDisplay(0); // 도트 모두지우기
  delay(100);
}

void loop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  if(BT_serial.available())
    {
      char toSend=(char)BT_serial.read();     //byte->char 강제 형변환
      switch(toSend)
      {
        case '1':
        isLedOn = true;
        break;
        case '0':
        isLedOn = false;
        checkTimePrev = false;
        Serial.println("신호 옴");
        break;
      }
    }
  if(Serial.available()) {
    char command = Serial.read();
    if(command == '1') {
      isLedOn = false;
      checkTimePrev = false;
    }
  }
    
  // 특정 조건만큼 기울어졌는지 확인
  if((a.acceleration.y <-4.00) || (a.acceleration.x <-4.00) || (a.acceleration.y >4.00) || (a.acceleration.x >4.00)) {
    if(checkTimePrev == false){
      timePrev = millis();
      checkTimePrev = true;
    }
    timeCurr = millis();
//    Serial.println(timePrev);
//    Serial.println(timeCurr);
    
    if(timeCurr - timePrev > 5000){
      isLedOn = true;
    } else {
        isLedOn =false;
    }
  } else {
    if(timeCurr - timePrev <= 5000) {
      isLedOn = false;
      checkTimePrev = false;
    }
  }
  if(isLedOn) {
    showLED(pattern, 1);
    Serial.print("LED ON,");
  } else {
    showLED(pattern, 0);
    Serial.print("off,");
  }
  
  Serial.print(a.acceleration.x);
  Serial.print(",");
  Serial.print(a.acceleration.y);
  Serial.print(",");
  Serial.println(a.acceleration.z);
  delay(500);
}



void showLED(byte arr[], int a) {
  if(a == 1) {
    for(int i = 0; i < 8; i++) {
      lc.setRow(0, i, arr[i]);
    }
  } else {
    for(int i = 0; i < 8; i++) {
      lc.setRow(0, i, B00000000);
   }
  }
}
