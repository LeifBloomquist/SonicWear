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
#define RX    MOSI   // 17 for SOMOv5   MOSI for SOMOv4
#define TX    4   

unsigned long counter=0;

SoftwareSerial radioSerial(RX, TX);

void setup() 
{
    // Soft Serial for XBee Radio
    radioSerial.begin(57600);
    
    // join I2C bus (I2Cdev library doesn't do this automatically)
    Wire.begin();

    // initialize device
    radioSerial.println("\n------------------------------------------------------");
    radioSerial.println("SoMo V5 Test Program");
    radioSerial.println("Initializing I2C devices...");
    accelgyro.initialize();

    // verify connection
    radioSerial.println("Testing device connections...");
    radioSerial.println(accelgyro.testConnection() ? "MPU9150 connection successful" : "MPU9150 connection failed");
        
    // Turn on SoMo Power LED
    pinMode(POWER, OUTPUT);
    digitalWrite(POWER, HIGH);   
}

char buf[50];

void loop() 
{
    digitalWrite(POWER, HIGH);   
  
    // read raw accel/gyro measurements from device
    accelgyro.getMotion9(&ax, &ay, &az, &gx, &gy, &gz, &mx, &my, &mz);
    
    sprintf(buf, "Hello - I am SOMO #4.   Count=%d", counter);    
    radioSerial.println(buf); 
    radioSerial.flush();
    
    digitalWrite(POWER, LOW);   
    delay(10);  // Milliseconds
    
    counter++;
}
