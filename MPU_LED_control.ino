#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;
int L1 = 2;
void setup(void) {
  Serial.begin(9600);

  if(!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while(1){
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  // set accelerometer range to +-8G
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);

  // set gyro range to +- 500 deg/s
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);

  // set filter bandwidth to 21 Hz
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  
  pinMode(L1, OUTPUT);
  delay(100);
}

void loop() {
  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp; //a : acceleration g: gyro
  mpu.getEvent(&a, &g, &temp);


  
  if(a.acceleration.y <-8.00){
    Serial.print("LIGHT ON,");
    digitalWrite(L1, HIGH);
  } else {
    Serial.print("off,");
    digitalWrite(L1, LOW);
  }
  //  Serial.print("x: ");
  Serial.print(a.acceleration.x);
  Serial.print(",");
  Serial.print(a.acceleration.y);
  Serial.print(",");
  Serial.println(a.acceleration.z);
  delay(500);
}
