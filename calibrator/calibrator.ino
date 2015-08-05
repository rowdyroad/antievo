#include <I2Cdev.h>
#include <Wire.h>
#include <MPU6050_6Axis_MotionApps20.h>

MPU6050 mpu;
const int OUTPUT_PIN = 3;

void blink(unsigned int times)
{
    digitalWrite(OUTPUT_PIN, LOW);
    for (unsigned int i = 0; i < times; ++i) {
      digitalWrite(OUTPUT_PIN, HIGH);
      delay(100);
      digitalWrite(OUTPUT_PIN, LOW);
      delay(50);
    }
    delay(200);
}

void setup() {
 Wire.begin();
 Serial.begin(38400);
 while (!Serial);
 pinMode(OUTPUT_PIN, OUTPUT);
 mpu.initialize(); 
 if (mpu.testConnection()) {
   blink(1);
 } else {
   blink(2);  
 }
}


struct Vector6dUInt16 {
  uint16_t ax,ay,az,gx,gy,gz;
  static void print(const struct Vector6dUInt16& vector)
  {
     Serial.print(vector.ax);
     Serial.print('\t'); 
     Serial.print(vector.ay);
     Serial.print('\t'); 
     Serial.print(vector.az);
     Serial.print('\t'); 
     Serial.print(vector.gx);
     Serial.print('\t'); 
     Serial.print(vector.gy);
     Serial.print('\t'); 
     Serial.println(vector.gz);
  }
};

struct Vector6dInt16 {
  int16_t ax,ay,az,gx,gy,gz;
  static void print(const struct Vector6dInt16& vector)
  {
     Serial.print(vector.ax);
     Serial.print('\t'); 
     Serial.print(vector.ay);
     Serial.print('\t'); 
     Serial.print(vector.az);
     Serial.print('\t'); 
     Serial.print(vector.gx);
     Serial.print('\t'); 
     Serial.print(vector.gy);
     Serial.print('\t'); 
     Serial.println(vector.gz);
  }
};

const struct Vector6dUInt16 diff = {0,0,0,0,0,0};

void set(struct Vector6dUInt16& min, struct Vector6dInt16& offset, int i)
{  
    mpu.setXGyroOffset(offset.gx + i);
    mpu.setYGyroOffset(offset.gy + i);
    mpu.setZGyroOffset(offset.gz + i);
    mpu.setXAccelOffset(offset.ax + i);
    mpu.setYAccelOffset(offset.ay + i);
    mpu.setZAccelOffset(offset.az + i);
    Vector6dInt16 motion;
    mpu.getMotion6(&motion.ax,&motion.ay,&motion.az,&motion.gx,&motion.gy,&motion.gz);   
    for (int j = 0; j < 6; ++j) {
     int16_t* v = ((int16_t*)&motion) + j;
     uint16_t* m = ((uint16_t*)&min)  + j;
     uint16_t* d = ((uint16_t*)&diff) + j;     
     int16_t* o = ((int16_t*)&offset) + j;   
     if (*m > abs(*v) - *d) {
        Serial.print(*m);
        Serial.print('\t');
        Serial.println(abs(*v) - *d);
        *m = abs(*v) - *d;
        *o = i;
     }
    }   
}

Vector6dUInt16 min = {0xffff,0xffff,0xffff,0xffff,0xffff,0xffff};
Vector6dInt16 offset = {0,0,0,0,0,0};
     

int stage = 1;
int index = -32000;     
void loop() {  
   if (index <=32000) {
     set(min, offset, index);
     Serial.println(index);
     Serial.print("min:\t");
     Vector6dUInt16::print(min);
     Serial.print("offset:\t");
     Vector6dInt16::print(offset);
     index+=1000;
   }
}
