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
//
//  Note that the SoMo appears as an Arduino Leonardo


#include <SoftwareSerial.h>
#include <Time.h>

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

unsigned long counter=0;

SoftwareSerial radioSerial(RX, TX);

void setup() 
{
    // Soft Serial for XBee Radio
    radioSerial.begin(57600);
    
    // join I2C bus (I2Cdev library doesn't do this automatically)
    Wire.begin();

    // initialize serial communication
    // (38400 chosen because it works as well at 8MHz as it does at 16MHz, but
    // it's really up to you depending on your project)
    Serial.begin(38400);

    // initialize device
    radioSerial.println("\n------------------------------------------------------");
    radioSerial.println("SoMo V5 Logger");
    radioSerial.println("Initializing I2C devices...");
    accelgyro.initialize();

    // verify connection
    radioSerial.println("Testing device connections...");
    radioSerial.println(accelgyro.testConnection() ? "MPU9150 connection successful" : "MPU9150 connection failed");
        
    // Turn on SoMo Power LED
    pinMode(POWER, OUTPUT);
    digitalWrite(POWER, HIGH);   
}

void loop() 
{
    // read raw accel/gyro measurements from device
    accelgyro.getMotion9(&ax, &ay, &az, &gx, &gy, &gz, &mx, &my, &mz);

    // transmit comma-separated accel/gyro x/y/z values    
    digitalWrite(POWER, HIGH);   
    radioSerial.print(counter); radioSerial.print(",");
    print_time();          radioSerial.print(",");
    radioSerial.print(ax); radioSerial.print(",");
    radioSerial.print(ay); radioSerial.print(",");
    radioSerial.print(az); radioSerial.print(",");
    radioSerial.print(gx); radioSerial.print(",");
    radioSerial.print(gy); radioSerial.print(",");
    radioSerial.print(gz); radioSerial.print(",");
    radioSerial.print(mx); radioSerial.print(",");
    radioSerial.print(my); radioSerial.print(",");
    radioSerial.println(mz);

    digitalWrite(POWER, LOW);   
    delay(1);  // Milliseconds
    counter++;
}


void print_time()
{
   long t_milli = millis();
  
   char buffer[20];
   int days, hours, mins, secs;
   int fractime;
   unsigned long inttime;

   inttime  = t_milli / 1000;
   fractime = t_milli % 1000;
   // inttime is the total number of number of seconds
   // fractimeis the number of thousandths of a second

   // number of days is total number of seconds divided by 24 divided by 3600
   days     = inttime / (24*3600);
   inttime  = inttime % (24*3600);

   // Now, inttime is the remainder after subtracting the number of seconds
   // in the number of days
   hours    = inttime / 3600;
   inttime  = inttime % 3600;

   // Now, inttime is the remainder after subtracting the number of seconds
   // in the number of days and hours
   mins     = inttime / 60;
   inttime  = inttime % 60;

   // Now inttime is the number of seconds left after subtracting the number
   // in the number of days, hours and minutes. In other words, it is the
   // number of seconds.
   secs = inttime;

   // Don't bother to print days
   sprintf(buffer, "%02d:%02d:%02d.%03d", hours, mins, secs, fractime);
   radioSerial.print(buffer);
}
