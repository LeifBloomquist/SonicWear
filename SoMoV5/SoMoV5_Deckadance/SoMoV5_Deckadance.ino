// MIDI Pentatonic Scale demonstration sketch for SoMo V5
// 
// This sketch reads the IMU and calculates the absolute acceleration magnitude.
// If a threshold is crossed, the SoMo angle is mapped to a note in the lookup table (array)
// and this MIDI Note On/Off is sent.
//
// To program, ensure the Arduino IDE is set to "Arduino Leonardo" under Tools->Board

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
#define RX    8
#define TX    4

#define ACCEL_CENTER 0         // Assume same for x, y, and z
#define ACCEL_RANGE  16000
#define ACCEL_MAX    (ACCEL_CENTER+ACCEL_RANGE)
#define ACCEL_MIN    (ACCEL_CENTER-ACCEL_RANGE)

#define PLAY_NOTE 60
#define CHANNEL   2

SoftwareSerial radioSerial(RX, TX);

boolean playing=false; // Toggle for on/off
int stop_count=0;      // Counter for turning off (threshold set below)

void setup() 
{
    // join I2C bus (I2Cdev library doesn't do this automatically)
    Wire.begin();

    // Soft Serial for XBee Radio
    radioSerial.begin(38400);
    
    // Initialize USB serial communication
    Serial.begin(38400);

    // initialize device
    Serial.println("Initializing I2C devices...");
    accelgyro.initialize();

    // verify connection
    Serial.println("Testing device connections...");
    Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
    
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
    
    // Determine volume and angle
    byte accel_cc = round(acceleration*127);
    byte angle_cc = round(angle*127);
         
    // On/off control for Deckadance.
    if (accel_cc > 30)
    {
      if (playing == false)
      {
        // Transmit Note On     
        sendMIDI(0x90 | CHANNEL, PLAY_NOTE, 0x7F);  
        playing = true;
      }
    }
    else
    {
      if (playing == true)   // Was playing, turn off
      {
        stop_count++;
        
        if (stop_count > 7)
        {
          // Transmit Note Off 
          sendMIDI(0x80 + CHANNEL,PLAY_NOTE,0x7F);
          playing = false;
          stop_count=0;
        }
      }
    }
    
    // Background, always send angle for future use
    sendMIDI(0xB0 | CHANNEL, 20, angle_cc);  
    
    // Background, always send velocity for possible volume control etc.
    sendMIDI(0xB0 | CHANNEL, 23 ,accel_cc); 
        
    digitalWrite(POWER, LOW);
    delay(50);  // Milliseconds
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
 
  // Naive gravity removal
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
