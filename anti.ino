#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <I2Cdev.h>

#include <MPU6050_6Axis_MotionApps20.h>
#include "EvacuationDetector.h"
struct Vector6d {
  int16_t gx,gy,gz,ax,ay,az;
};

struct VectorUnsigned6d
{
  unsigned int gx,gy,gz,ax,ay, az;
};

struct VectorFloat4d {
   float gx,gy,gz,az;
};
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
}

bool dmpReady = false;
uint8_t mpuIntStatus;
uint8_t devStatus;
uint16_t packetSize;
uint16_t fifoCount;
uint8_t fifoBuffer[64];

Quaternion q;
VectorInt16 aa;
VectorInt16 aaReal;
VectorInt16 aaWorld;
VectorFloat gravity;
VectorFloat world;
VectorFloat accel_summ(0,0,0);
float s = 0;
unsigned long accel_count = 0;
bool calibrated = false;



volatile bool mpuInterrupt = false;
void dmpDataReady() {
    mpuInterrupt = true;
}

float roundStrong(float f, unsigned int digs) {
   double k = pow(10, digs);
   return round(f * k) / k ;
}

void setup() {

  Wire.begin();
  Serial.begin(115200);
  pinMode(13,OUTPUT);
  //SD.begin(4);
  Serial.println("Begin");
  while (!Serial);
  mpu.initialize();

  if (!mpu.dmpInitialize()) {
    Serial.print("Calibrating...");
    mpu.setXGyroOffset(40);
    mpu.setYGyroOffset(70);
    mpu.setZGyroOffset(-12);
    mpu.setXAccelOffset(-2145);
    mpu.setYAccelOffset(-345);
    mpu.setZAccelOffset(1020);
    Serial.println("OK");

    Serial.print("DMP Initializing...");
    mpu.setDMPEnabled(true);
    attachInterrupt(0, dmpDataReady, RISING);
    mpuIntStatus = mpu.getIntStatus();
    dmpReady = true;
    packetSize = mpu.dmpGetFIFOPacketSize();
    Serial.println("OK");
  }
}



unsigned int measures_index = 0;
unsigned int measures_count = 100;
float measures[100];
float last_rms = 0;

int raise_max_count = 10;
int raise_count = 0;
EvacuationDetector ed(1000)

bool measuring = false;
unsigned long last_time = micros() + 1000000;
void loop() {

    if (!dmpReady || (!mpuInterrupt && fifoCount < packetSize)) return;

    mpuInterrupt = false;
    mpuIntStatus = mpu.getIntStatus();
    fifoCount = mpu.getFIFOCount();
    if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
        mpu.resetFIFO();
    } else if (mpuIntStatus & 0x02) {
        while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();
        mpu.getFIFOBytes(fifoBuffer, packetSize);
        fifoCount -= packetSize;
        mpu.dmpGetQuaternion(&q, fifoBuffer);
        mpu.dmpGetAccel(&aa, fifoBuffer);
        mpu.dmpGetGravity(&gravity, &q);
        mpu.dmpGetLinearAccel(&aaReal, &aa, &gravity);
        mpu.dmpGetLinearAccelInWorld(&aaWorld, &aaReal, &q);

        world.x = 1.0 * aaWorld.x / 8192;
        world.y = 1.0 * aaWorld.y / 8192;
        world.z = 1.0 * aaWorld.z / 8192;

        Serial.print(++measures_index);

        Serial.print('\t');
        Serial.print(world.x);
        Serial.print('\t');
        Serial.print(world.y);
        Serial.print('\t');
        Serial.print(world.z);
        Serial.print('\t');
        Serial.print(q.w);
        Serial.print('\t');
        Serial.print(q.x);
        Serial.print('\t');
        Serial.print(q.y);
        Serial.print('\t');
        Serial.println(q.z);

        if (ed.process(world.z)) {
          Serial.println("Evacuation Detected\n");
          ed.clear();
        }
    }
}
