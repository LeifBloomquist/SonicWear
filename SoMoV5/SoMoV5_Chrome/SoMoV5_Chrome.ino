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
//     2016-05-27 - modified to use new Chrome app protocol (LB)

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

#define BATTERY 1000  // Fake battery reading (not available on SOMO V5)

#define POWER 1
#define RX    17
#define TX    4

#define RATE  50  // Every xx milliseconds.  Do not set to less than 20 as to not swamp receiver XBee

//SET UNIT ID HERE
//*******************************
byte unitID = 0x31;
//*******************************

char analogValue[20];   // array of analog values *in byte format for MAX* plus some buffer
char current = 0;       // current position of analog value in array
int digVal = 0;         // battery voltage into a single variable
char imask = 128;       // index bytes start with 0x80
char theEnd = 255;      // byte to signal message end

SoftwareSerial radioSerial(RX, TX);

void setup() 
{
    // join I2C bus (I2Cdev library doesn't do this automatically)
    Wire.begin();

    // initialize serial communication for debug messages
    Serial.begin(38400);

    // initialize device
    Serial.println("Initializing I2C devices...");
    accelgyro.initialize();

    // verify connection
    Serial.println("Testing device connections...");
    Serial.println(accelgyro.testConnection() ? "MPU9150 connection successful" : "MPU9150 connection failed");
    
    // Soft Serial for XBee Radio
    radioSerial.begin(38400);
    
    // Turn on SoMo Power LED
    pinMode(POWER, OUTPUT);
    digitalWrite(POWER, HIGH);   
}

void loop() 
{
    digitalWrite(POWER, HIGH);
  
    // read raw accel/gyro measurements from device
    accelgyro.getMotion9(&ax, &ay, &az, &gx, &gy, &gz, &mx, &my, &mz);

/*
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
*/

	// Scale
	int16_t scaled_ax = ScaleA(ax);
	int16_t scaled_ay = ScaleA(ay);
	int16_t scaled_az = ScaleA(az);
	int16_t scaled_gx = ScaleG(gx);
	int16_t scaled_gy = ScaleG(gy);
	int16_t scaled_gz = ScaleG(gz);
	int16_t scaled_mx = ScaleM(mx);
	int16_t scaled_my = ScaleM(my);
	int16_t scaled_mz = ScaleM(mz);
        
	// Pack
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

	// Transmit 
	sendMessage(current);   // Transmit
   
	digitalWrite(POWER, LOW);

	// Delay
    delay(RATE);  // Milliseconds
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

// Map an input between MIN and MAX to 0 to 1024
int16_t ScaleA(long in)
{   
  float temp = Scale(in, ACCEL_MIN, ACCEL_MAX);
  return temp * 1024;
}

#define GYRO_CENTER 0        // Assume same for x, y, and z
#define GYRO_RANGE  32000
#define GYRO_MAX    (GYRO_CENTER+GYRO_RANGE)
#define GYRO_MIN    (GYRO_CENTER-GYRO_RANGE)

// Map an input between MIN and MAX to 0 to 1024
int16_t ScaleG(long in)
{   
  float temp = Scale(in, GYRO_MIN, GYRO_MAX);
  return temp * 1024;
}

#define MAG_CENTER 0        // Assume same for x, y, and z
#define MAG_RANGE  600
#define MAG_MAX    (MAG_CENTER+MAG_RANGE)
#define MAG_MIN    (MAG_CENTER-MAG_RANGE)

// Map an input between MIN and MAX to 0 to 1024
int16_t ScaleM(long in)
{   
  float temp = Scale(in, MAG_MIN, MAG_MAX);
  return temp * 1024;
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


void packValueMPU9150(int16_t value)
{
	int16_t tempA = value;
	analogValue[current] = tempA & 127;
	current++;
	analogValue[current] = (tempA >> 7);
	current++;
}


void sendMessage(char total)
{
	radioSerial.write(imask | 0xF); //this is 0x8F

	// Send identifier phrase
	radioSerial.print("SOMOV5");

	// Send UnitID
	radioSerial.write(unitID);

	// Send Analog Values
	for (int i = 0; i<total; i++)
	{
		radioSerial.write(analogValue[i]);
	}

	// Fake Battery Value
	radioSerial.write((BATTERY & 127));
	radioSerial.write(BATTERY >> 7);

	radioSerial.write(theEnd);   //ends digital message with 255
}