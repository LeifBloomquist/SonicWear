
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
//     2015-03-11 - modified to demonstrate SoMo v5.0 board (LB)


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

#define ACCEL_CENTER 0         // Assume same for x, y, and z
#define ACCEL_RANGE  16000
#define ACCEL_MAX    (ACCEL_CENTER+ACCEL_RANGE)
#define ACCEL_MIN    (ACCEL_CENTER-ACCEL_RANGE)

SoftwareSerial radioSerial(RX, TX);

// MIDI note values for the C Major Pentatonic Scale.
byte pentatonic[] = { C   D  E  G  A 
                  //  0,  2, 4, 7, 9,       // Octave 0
                  //  12,14,16,19,21,       // Octave 1
                  //  24,26,28,31,33,       // Octave 2
                      36,38,40,43,45,       // Octave 3
                      48,50,52,55,57,       // Octave 4
                      60,62,64,67,69,       // Octave 5
                      72,74,76,79,81,       // Octave 6
                      84,86,88,91,93        // Octave 7
                  //  96,98,100,103,105,    // Octave 8
                  //  108,110,112,115,117,  // Octave 9
                  //  120,122,124,127,      // Octave 10
                   };

int arraylength = sizeof(pentatonic);

void setup() 
{
    // join I2C bus (I2Cdev library doesn't do this automatically)
    Wire.begin();

    // initialize serial communication
    Serial.begin(38400);

    // initialize device
    Serial.println("Initializing I2C devices...");
    accelgyro.initialize();

    // verify connection
    Serial.println("Testing device connections...");
    Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
    
    // Soft Serial for XBee Radio
    radioSerial.begin(57600);
    
    // Turn on SoMo Power LED
    pinMode(POWER, OUTPUT);
    digitalWrite(POWER, HIGH);   
}

void loop() 
{
    digitalWrite(POWER, HIGH);
  
    // read raw accel/gyro measurements from device
    accelgyro.getMotion9(&ax, &ay, &az, &gx, &gy, &gz, &mx, &my, &mz);

    // Scale Data
    float angle = Scale(ax, ACCEL_MIN, ACCEL_MAX);
    float acceleration = ScaleAccelMagnitude(ax, ay, az);
    
    // Map to a note
    byte note = pentatonic[round(angle*(arraylength-1))];
    
    // Determine volume (velocity)
    byte velocity = round(acceleration*127);
    
    Serial.print("Angle = ");
    Serial.print(angle);
    Serial.print("\tNote = ");
    Serial.print(note);
    Serial.print("\tVelocity = ");
    Serial.println(velocity);

     
    digitalWrite(POWER, LOW);
     
    if (velocity > 20)
    {
      // Transmit     
      sendMIDI(0x90,note,velocity);   // Channel 1, Note On
      delay(10);
      sendMIDI(0x80,note,0);          // Channel 1, Note Off
    }
        
    delay(10);  // Milliseconds
}

void sendMIDI(byte cmd, byte data1, byte data2)
{
   radioSerial.write(cmd);
   radioSerial.write(data1);
   radioSerial.write(data2);
}


// Return a value 0.0 to 1.0 representing the magnitude of x/y/z acceleration with gravity removed
float ScaleAccelMagnitude(long x, long y, long z)
{   
  float tempx = ScaleAbs(x, ACCEL_CENTER, ACCEL_RANGE);
  float tempy = ScaleAbs(y, ACCEL_CENTER, ACCEL_RANGE);
  float tempz = ScaleAbs(z, ACCEL_CENTER, ACCEL_RANGE);
    
  float temp1 = sq(tempx) + sq(tempy) + sq(tempz);
  float magnitudein = sqrt(temp1);
 
  // Extremely naive gravity removal
  float magnitudeout = magnitudein-1.0;
  
  // Lower bound here
  if (magnitudeout < 0.0) magnitudeout = 0.0;
  
  // Scale the output to make it less sensitive.  **Adjust this as needed**
  float magnitudescaled = magnitudeout / 2.5;
  
  // Upper Bound
  if (magnitudescaled > 1.0) magnitudescaled = 1.0;
  
  return magnitudescaled;
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



