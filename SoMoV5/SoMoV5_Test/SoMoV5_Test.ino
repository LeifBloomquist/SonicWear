
// Based on:
// I2C device class (I2Cdev) demonstration Arduino sketch for MPU9150
// 1/4/2013 original by Jeff Rowberg <jeff@rowberg.net> at https://github.com/jrowberg/i2cdevlib
//          modified by Aaron Weiss <aaron@sparkfun.com>
//          modified by Leif Bloomquist <leif@sonicwear.ca>
//
// Changelog:
//     2011-10-07 - initial release
//     2013-1-4   - added raw magnetometer output
//     2014-02-17 - modified to demonstrate SoMo v4.0 board (LB)
//     2014-03-11 - modified to demonstrate SoMo v5.0 board (LB)


#include <SoftwareSerial.h>

// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
#include "Wire.h"

// I2Cdev and MPU6050 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include "I2Cdev.h"
#include "MPU6050.h"

// class default I2C address is 0x68
// specific I2C addresses may be passed as a parameter here
// AD0 low = 0x68 (default for InvenSense evaluation board)
// AD0 high = 0x69
MPU6050 accelgyro;

int16_t ax, ay, az;
int16_t gx, gy, gz;
int16_t mx, my, mz;

#define POWER 1
#define RX    17
#define TX    4


SoftwareSerial radioSerial(RX, TX);

void setup() {
    // join I2C bus (I2Cdev library doesn't do this automatically)
    Wire.begin();

    // initialize serial communication
    // (38400 chosen because it works as well at 8MHz as it does at 16MHz, but
    // it's really up to you depending on your project)
    Serial.begin(38400);

    // initialize device
    Serial.println("Initializing I2C devices...");
    accelgyro.initialize();

    // verify connection
    Serial.println("Testing device connections...");
    Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
    
    // Soft Serial for XBee Radio
    radioSerial.begin(38400);
    
    // Turn on SoMo Power LED
    pinMode(POWER, OUTPUT);
    digitalWrite(POWER, HIGH);   
}

void loop() {
    // read raw accel/gyro measurements from device
    accelgyro.getMotion9(&ax, &ay, &az, &gx, &gy, &gz, &mx, &my, &mz);

    // these methods (and a few others) are also available
    //accelgyro.getAcceleration(&ax, &ay, &az);
    //accelgyro.getRotation(&gx, &gy, &gz);


    // display tab-separated accel/gyro x/y/z values  
    Serial.print("a/g/m:\t");
    Serial.print(ax); Serial.print("\t");
    Serial.print(ay); Serial.print("\t");
    Serial.print(az); Serial.print(" |\t");
    Serial.print(gx); Serial.print("\t");
    Serial.print(gy); Serial.print("\t");
    Serial.print(gz); Serial.print(" |\t");
    Serial.print(mx); Serial.print("\t");
    Serial.print(my); Serial.print("\t");
    Serial.println(mz);


    // Convert data to MIDI CC's
    byte axcc = AccelToCC(ax);
    byte aycc = AccelToCC(ay);
    byte azcc = AccelToCC(az);
    byte amcc = AccelMagnitudeToCC(ax,ay,az);
    byte gxcc = GyroToCC(gx);
    byte gycc = GyroToCC(gy);
    byte gzcc = GyroToCC(gz);
    byte mxcc = MagToCC(mx);
    byte mycc = MagToCC(my);
    byte mzcc = MagToCC(mz);
     
    // Transmit     
    sendMIDI(0xB0,20,axcc);   // X = Channel 1, CC#20
    sendMIDI(0xB0,21,aycc);   // Y = Channel 1, CC#21
    sendMIDI(0xB0,22,azcc);   // Z = Channel 1, CC#22
    sendMIDI(0xB0,23,amcc);   // M = Channel 1, CC#23
    sendMIDI(0xB0,24,gxcc);   // X = Channel 1, CC#24
    sendMIDI(0xB0,25,gycc);   // Y = Channel 1, CC#25
    sendMIDI(0xB0,26,gzcc);   // Z = Channel 1, CC#26
    sendMIDI(0xB0,27,mxcc);   // X = Channel 1, CC#27
    sendMIDI(0xB0,28,mycc);   // Y = Channel 1, CC#28
    sendMIDI(0xB0,29,mzcc);   // Z = Channel 1, CC#29
    
    delay(50);  // Milliseconds
}

void sendMIDI(byte cmd, byte data1, byte data2)
{
   radioSerial.write(cmd);
   radioSerial.write(data1);
   radioSerial.write(data2);
}


#define ACCEL_CENTER 0         // Assume same for x, y, and z
#define ACCEL_RANGE  16000
#define ACCEL_MAX    (ACCEL_CENTER+ACCEL_RANGE)
#define ACCEL_MIN    (ACCEL_CENTER-ACCEL_RANGE)

// Map an input between MIN and MAX to 0 to 127
byte AccelToCC(long in)
{   
  float temp = Scale(in, ACCEL_MIN, ACCEL_MAX);
  return temp*127;
}

// Return a value 0 to 127 representing the total magnitude force
byte AccelMagnitudeToCC(long x, long y, long z)
{   
  float tempx = ScaleAbs(x, ACCEL_CENTER, ACCEL_RANGE);
  float tempy = ScaleAbs(y, ACCEL_CENTER, ACCEL_RANGE);
  float tempz = ScaleAbs(z, ACCEL_CENTER, ACCEL_RANGE);
    
  float temp1 = sq(tempx) + sq(tempy) + sq(tempz);
  float magnitudein = sqrt(temp1);
 
  // Extremely naive, and probably wrong, gravity removal (but it works)
  float magnitudeout = magnitudein-1.0;
  
  // Lower bound here
  if (magnitudeout < 0.0) magnitudeout = 0.0;
  
  // Scale the output to make it less sensitive.  **Adjust this as needed**
  float magnitudescaled = magnitudeout / 2.5;
  
  // Upper Bound
  if (magnitudescaled > 1.0) magnitudescaled = 1.0;
  
  // Determine CC value
  byte cc = magnitudescaled*127;  
  
/*
  Serial.print("X: ");
  Serial.print(tempx);
  Serial.print("\tY: ");
  Serial.print(tempy);
  Serial.print("\tZ: ");
  Serial.print(tempz);
  Serial.print("\tMagnitude (Gravity In): "); 
  Serial.print(magnitudein);
  Serial.print("\tMagnitude (Gravity Out): "); 
  Serial.print(magnitudeout);
  Serial.print("\tMagnitude (Scaled): "); 
  Serial.print(magnitudescaled);
  Serial.print("\tCC Value: "); 
  Serial.print(cc);
  Serial.println();
*/
  
  return cc;
}


#define GYRO_CENTER 0        // Assume same for x, y, and z
#define GYRO_RANGE  32000
#define GYRO_MAX    (GYRO_CENTER+GYRO_RANGE)
#define GYRO_MIN    (GYRO_CENTER-GYRO_RANGE)

// Map an input between MIN and MAX to 0 to 127
byte GyroToCC(long in)
{   
  float temp = Scale(in, GYRO_MIN, GYRO_MAX);
  return temp*127;
}

#define MAG_CENTER 0        // Assume same for x, y, and z
#define MAG_RANGE  32000
#define MAG_MAX    (MAG_CENTER+MAG_RANGE)
#define MAG_MIN    (MAG_CENTER-MAG_RANGE)

// Map an input between MIN and MAX to 0 to 127
byte MagToCC(long in)
{   
  float temp = Scale(in, MAG_MIN, MAG_MAX);
  return temp*127;
}


// Scale one of the inputs to between 0.0 and 1.0
float Scale(long in, long smin, long smax)
{
  // Bound
  if (in > smax) in=smax;
  if (in < smin) in=smin;
  
  // Change zero-offset
  in = in-smin;
  
  // Scale between 0.0 and 1.0    (0.5 would be halfway)
  float temp = (float)in/((float)smax-(float)smin);
  return temp;   
}


// Scale one of the inputs to return distance from center (0.0=centered, 1.0=min or max, may exceed 1.0)
float ScaleAbs(long in, long center, long range)
{
  // Get the absolute distance from center
  long temp = in-center;
  long val = abs(temp);
  
  // Scale
  float out = (float)val/(float)range;  
  
  // *Don't* Bound.
  
  return out;
}



