// I2C device class (I2Cdev) demonstration Arduino sketch for MPU9150
// 1/4/2013 original by Jeff Rowberg <jeff@rowberg.net> at https://github.com/jrowberg/i2cdevlib
//          modified by Aaron Weiss <aaron@sparkfun.com>
//
// Changelog:
//     2011-10-07 - initial release
//     2013-1-4 - added raw magnetometer output

/* ============================================
I2Cdev device library code is placed under the MIT license

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
===============================================
*/

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

SoftwareSerial mySerial(MOSI, 4); // RX, TX

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
    mySerial.begin(38400);
}


#define CENTER 0        // Assume same for x, y, and z
#define RANGE  20000
#define MAX    (CENTER+RANGE)
#define MIN    (CENTER-RANGE)



void loop() {
    // read raw accel/gyro measurements from device
    accelgyro.getMotion9(&ax, &ay, &az, &gx, &gy, &gz, &mx, &my, &mz);

    // these methods (and a few others) are also available
    //accelgyro.getAcceleration(&ax, &ay, &az);
    //accelgyro.getRotation(&gx, &gy, &gz);

    // display tab-separated accel/gyro x/y/z values
    
   /*
    Serial.print("a/g/m:\t");
    Serial.print(ax); Serial.print("\t");
    Serial.print(ay); Serial.print("\t");
    Serial.print(az); Serial.print("\t");
    Serial.print(gx); Serial.print("\t");
    Serial.print(gy); Serial.print("\t");
    Serial.print(gz); Serial.print("\t");
    Serial.print(mx); Serial.print("\t");
    Serial.print(my); Serial.print("\t");
    Serial.println(mz);
    */
    
    
    // Convert acceleration to MIDI
    byte xcc = AccelToCC(ax);
    byte ycc = AccelToCC(ay);
    byte zcc = AccelToCC(az);
    
    // Transmit     
    sendMIDI(0xB0,20,xcc);   // X = Channel 1, CC#20
    sendMIDI(0xB0,21,ycc);   // Y = Channel 1, CC#21
    sendMIDI(0xB0,22,zcc);   // Z = Channel 1, CC#22
    
    delay(50);
}

void sendMIDI(byte cmd, byte data1, byte data2)
{
   mySerial.write(cmd);
   mySerial.write(data1);
   mySerial.write(data2);
}

// Map an input between MIN and MAX to 0 to 127
byte AccelToCC(long in)
{   
  float temp = Scale(in);
  return temp*127;
}


// Scale one of the inputs to between 0.0 and 1.0
float Scale(long in)
{
  // Bound
  if (in > MAX) in=MAX;
  if (in < MIN) in=MIN;
  
  // Change zero-offset
  in = in-MIN;
  
  // Scale between 0.0 and 1.0
  float temp = (float)in/((float)MAX-(float)MIN);
  return temp;   
}


    /*
    Serial.print("ax=");
    Serial.print(ax);
    Serial.print("\tScaled=");
    Serial.println(xscaled);
    Serial.print("\tCC=");
    Serial.println(xcc);
    */
