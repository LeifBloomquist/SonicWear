////////////////////////////////////////////////////////////////////////////
//
//  This file is part of RTIMULib-Arduino
//
//  Copyright (c) 2014-2015, richards-tech
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy of 
//  this software and associated documentation files (the "Software"), to deal in 
//  the Software without restriction, including without limitation the rights to use, 
//  copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the 
//  Software, and to permit persons to whom the Software is furnished to do so, 
//  subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in all 
//  copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
//  INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
//  PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT 
//  HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION 
//  OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
//  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#include <Wire.h>
#include "I2Cdev.h"
#include "RTIMUSettings.h"
#include "RTIMU.h"
#include "RTFusionRTQF.h" 
#include "CalLib.h"
#include <EEPROM.h>

#include <SoftwareSerial.h>

// SoMo V5
#define POWER 1
#define RX    17
#define TX    4
SoftwareSerial radioSerial(RX, TX);

RTIMU *imu;                                           // the IMU object
RTFusionRTQF fusion;                                  // the fusion object
RTIMUSettings settings;                               // the settings object

//  DISPLAY_INTERVAL sets the rate at which results are displayed

#define DISPLAY_INTERVAL  10                         // interval between pose displays

//  SERIAL_PORT_SPEED defines the speed to use for the debug serial port

#define  SERIAL_PORT_SPEED  115200

unsigned long lastDisplay;
unsigned long lastRate;
int sampleCount;

void setup()
{ 
    int errcode;
  
    Serial.begin(SERIAL_PORT_SPEED);
    Wire.begin();
    imu = RTIMU::createIMU(&settings);                        // create the imu object
  
    Serial.print("ArduinoIMU starting using device "); Serial.println(imu->IMUName());
    if ((errcode = imu->IMUInit()) < 0) {
        Serial.print("Failed to init IMU: "); Serial.println(errcode);
    }
  
    if (imu->getCalibrationValid())
        Serial.println("Using compass calibration");
    else
        Serial.println("No valid compass calibration data");

    lastDisplay = lastRate = millis();
    sampleCount = 0;

    // Slerp power controls the fusion and can be between 0 and 1
    // 0 means that only gyros are used, 1 means that only accels/compass are used
    // In-between gives the fusion mix.
    
    fusion.setSlerpPower(0.02);
    
    // use of sensors in the fusion algorithm can be controlled here
    // change any of these to false to disable that sensor
    
    fusion.setGyroEnable(true);
    fusion.setAccelEnable(true);
    fusion.setCompassEnable(true);
    
    // SoMo-Specific
    
    // Soft Serial for XBee Radio
    radioSerial.begin(57600);
    
    // Turn on SoMo Power LED
    pinMode(POWER, OUTPUT);
    digitalWrite(POWER, HIGH);   
}

void loop()
{  
    digitalWrite(POWER, LOW);
  
    unsigned long now = millis();
    unsigned long delta;
    int loopCount = 1;
  
    while (imu->IMURead()) {                                // get the latest data if ready yet
        // this flushes remaining data in case we are falling behind
        if (++loopCount >= 10)
            continue;
        fusion.newIMUData(imu->getGyro(), imu->getAccel(), imu->getCompass(), imu->getTimestamp());
        sampleCount++;
        if ((delta = now - lastRate) >= 1000) {
            Serial.print("Sample rate: "); Serial.print(sampleCount);
            if (imu->IMUGyroBiasValid())
                Serial.println(", gyro bias valid");
            else
                Serial.println(", calculating gyro bias");
        
            sampleCount = 0;
            lastRate = now;
        }
        if ((now - lastDisplay) >= DISPLAY_INTERVAL) 
        {
             
          digitalWrite(POWER, HIGH);
          
            lastDisplay = now;
//          RTMath::display("Gyro:", (RTVector3&)imu->getGyro());                // gyro data
//          RTMath::display("Accel:", (RTVector3&)imu->getAccel());              // accel data
//          RTMath::display("Mag:", (RTVector3&)imu->getCompass());              // compass data
//            RTMath::displayRollPitchYaw("Pose:", (RTVector3&)fusion.getFusionPose()); // fused output
//            Serial.println();
            
            float roll = fusion.getFusionPose().x() * RTMATH_RAD_TO_DEGREE;
            float pitch = fusion.getFusionPose().y() * RTMATH_RAD_TO_DEGREE;
            float yaw = fusion.getFusionPose().z() * RTMATH_RAD_TO_DEGREE;
            
            float qx = fusion.getFusionQPose().x();
            float qy = fusion.getFusionQPose().y();
            float qz = fusion.getFusionQPose().z();
            float qw = fusion.getFusionQPose().scalar();
           
            // SoMo-Specific
/*            
            byte pitch_cc = ValToCC(pitch);
            byte yaw_cc = ValToCC(yaw);
            byte roll_cc = ValToCC(roll);

            sendMIDI(0xB0,30, roll_cc);    // Z = Channel 1, CC#30           
            sendMIDI(0xB0,31, pitch_cc);   // X = Channel 1, CC#30
            sendMIDI(0xB0,32, yaw_cc);     // Y = Channel 1, CC#32
*/            
            radioSerial.print(roll);
            radioSerial.print(",");
            radioSerial.print(pitch);
            radioSerial.print(",");
            radioSerial.print(yaw);
            radioSerial.print(",");

            radioSerial.print(qx);
            radioSerial.print(",");
            radioSerial.print(qy);
            radioSerial.print(",");
            radioSerial.print(qz);
            radioSerial.print(",");
            radioSerial.print(qw); 
            radioSerial.println();
      }
    }
}

// Map an input between MIN and MAX to 0 to 127
byte ValToCC(float in)
{   
  float temp = Scale(in, -180, 180);
  return temp*127;
}

// Scale one of the inputs to between 0.0 and 1.0
float Scale(float in, long smin, long smax)
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

void sendMIDI(byte cmd, byte data1, byte data2)
{
   radioSerial.write(cmd);
   radioSerial.write(data1);
   radioSerial.write(data2);
}
