////////////////////////////////////////////////////////////////////////////
//
// This code sends output from RTUIMULib to MAX in Modified SensorBox Format
//
// Packet format:  0x81 <unitid>  <packed value 0: 2 bytes>  <packed value 1: 2 bytes> ...  0xFF

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

#define  SERIAL_PORT_SPEED  38400

unsigned long lastDisplay;
unsigned long lastRate;
int sampleCount;

// MAX Stuff
char analogValue[30];  //array of analog values *in byte format for MAX* plus some buffer
char current = 0;//current position of analog value in array
int digVal;//digital pins bits are packed into a single variable
char imask = 128;//index bytes start with 1
char theEnd = 255;//byte to signal message end to Max patch


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

    fusion.setSlerpPower(0.02);   // Default

    // use of sensors in the fusion algorithm can be controlled here
    // change any of these to false to disable that sensor

    fusion.setGyroEnable(true);
    fusion.setAccelEnable(true);
    fusion.setCompassEnable(true);

    // SoMo-Specific

    // Soft Serial for XBee Radio
    radioSerial.begin(38400);

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

            // Raw values
            float ax = imu->getAccel().x();
            float ay = imu->getAccel().y();
            float az = imu->getAccel().z();
            float gx = imu->getGyro().x();
            float gy = imu->getGyro().y();
            float gz = imu->getGyro().z();
            float mx = imu->getCompass().x();
            float my = imu->getCompass().y();
            float mz = imu->getCompass().z();
            float roll = 180 + (fusion.getFusionPose().x() * RTMATH_RAD_TO_DEGREE);
            float pitch = 180 + (fusion.getFusionPose().y() * RTMATH_RAD_TO_DEGREE);
            float yaw = 180 + (fusion.getFusionPose().z() * RTMATH_RAD_TO_DEGREE);

            // Scale inputs to 0-1024 for backwards compatibility with old SensorBox code using pure analog inputs
            int scaled_ax = ScaleMAX(ax);
            int scaled_ay = ScaleMAX(ay);
            int scaled_az = ScaleMAX(az);
            int scaled_gx = ScaleMAX(gx);
            int scaled_gy = ScaleMAX(gy);
            int scaled_gz = ScaleMAX(gz);
            int scaled_mx = ScaleMAX_mag(mx);
            int scaled_my = ScaleMAX_mag(my);
            int scaled_mz = ScaleMAX_mag(mz);
            // Don't scale roll/pitch yaw, use as 0-360

            /*
            Serial.print("ax: ");    Serial.print(ax);
            Serial.print("\tay: ");  Serial.print(ay);
            Serial.print("\taz: ");  Serial.print(az);
            Serial.print("\tgx: ");  Serial.print(gx);
            Serial.print("\tgy: ");  Serial.print(gy);
            Serial.print("\tgz: ");  Serial.print(gz);
            Serial.print("\tmx: ");  Serial.print(mx);
            Serial.print("\tmy: ");  Serial.print(my);
            Serial.print("\tmz: ");  Serial.print(mz);
            Serial.print("\troll: ");  Serial.print(roll);
            Serial.print("\tpitch: ");  Serial.print(pitch);
            Serial.print("\tyaw: ");  Serial.println(yaw);
            */

            Serial.print("ax: ");    Serial.print(scaled_ax);
            Serial.print("\tay: ");  Serial.print(scaled_ay);
            Serial.print("\taz: ");  Serial.print(scaled_az);
            Serial.print("\tgx: ");  Serial.print(scaled_gx);
            Serial.print("\tgy: ");  Serial.print(scaled_gy);
            Serial.print("\tgz: ");  Serial.print(scaled_gz);
            Serial.print("\tmx: ");  Serial.print(scaled_mx);
            Serial.print("\tmy: ");  Serial.print(scaled_my);
            Serial.print("\tmz: ");  Serial.print(scaled_mz);
            Serial.print("\troll: ");  Serial.print(roll);
            Serial.print("\tpitch: ");  Serial.print(pitch);
            Serial.print("\tyaw: ");  Serial.println(yaw);


            // MAX Stuff
            current = 0;  //reset analog value counter

            packValueMPU9150(scaled_ax);
            packValueMPU9150(scaled_ay);
            packValueMPU9150(scaled_az);
            packValueMPU9150(scaled_mx);
            packValueMPU9150(scaled_my);
            packValueMPU9150(scaled_mz);
            packValueMPU9150(scaled_gx);
            packValueMPU9150(scaled_gy);
            packValueMPU9150(scaled_gz);
            packValueMPU9150(roll);
            packValueMPU9150(pitch);
            packValueMPU9150(yaw);

            char total = current; //+1;

            sendOFF(total);//send everything to Max 

        }
    }
}


// Scale one of the inputs to between 0.0 and 1.0
float Scale(float in, float smin, float smax)
{
    // Bound
    if (in > smax) in = smax;
    if (in < smin) in = smin;

    // Change zero-offset
    in = in - smin;

    // Scale between 0.0 and 1.0    (0.5 would be halfway)
    float temp = (float)in / ((float)smax - (float)smin);
    return temp;
}


void sendOFF(char total)
{
    //Send analog values in the format 0x81 a1 a2.....0xFF
    //Max wants 81 (start), FF is the end of the message
    radioSerial.write(imask | 1); //this is 0x81

    //Write UnitID to MAX
    //**********************************
    // Turn On/Off Serial ID
    radioSerial.write(0x01);
    //***********************************

    for (int i = 0; i<total; i++)
    {
        radioSerial.write(analogValue[i]);
    }
    radioSerial.write(theEnd);//ends analog stream with 255

    /*  
    mySerial.write((imask|2));
    mySerial.write((digVal&127));
    mySerial.write(digVal>>7);
    mySerial.write(theEnd);//ends digital message with 255
    */
}

void packValueMPU9150(int16_t value){
    int16_t tempA = value;
    analogValue[current] = tempA & 127;
    current++;
    analogValue[current] = (tempA >> 7);
    current++;
}


// Scale the inputs
int ScaleMAX(float raw)
{
    float temp = Scale(raw, -1.0, 1.0);

    return temp * 1024;
}

// Scale the inputs for magnetometer only
int ScaleMAX_mag(float raw)
{
    float temp = Scale(raw, -70, 70);

    return temp * 1024;
}
