
// C64 Joystick Emulator "Gaming Glove" for SoMo v4


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

// These are for SoMo v4 with Eric's mods 

int up    = 10;
int down  = 6;
int left  = 5;
int right = 13;
int fire  = 11;

#define FIREIN    8

void setup() 
{
    // join I2C bus (I2Cdev library doesn't do this automatically)
    Wire.begin();

    // initialize serial communication for debugging
    Serial.begin(38400);

    // initialize device
    Serial.println("Initializing I2C devices...");
    accelgyro.initialize();

    // verify connection
    Serial.println("Testing device connections...");
    Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
    
    // Joystick mode 
    pinMode(up, OUTPUT);
    digitalWrite(up, LOW);  
    
    pinMode(down, OUTPUT);
    digitalWrite(down, LOW);   
  
    pinMode(left, OUTPUT);
    digitalWrite(left, LOW);  
    
    pinMode(right, OUTPUT);
    digitalWrite(right, LOW);     
  
    pinMode(fire, OUTPUT);
    digitalWrite(fire, LOW);      
    
    pinMode(fire, OUTPUT);
    digitalWrite(fire, LOW);      

    pinMode(FIREIN,  INPUT_PULLUP);      
}


#define CENTER 0        // Assume same for x, y, and z
#define RANGE  20000
#define MAX    (CENTER+RANGE)
#define MIN    (CENTER-RANGE)

#define MIDDLE    0.5 
#define DEADBAND  0.1   // Tweak this to adjust sensitivity
#define JHIGH     (MIDDLE+DEADBAND)
#define JLOW      (MIDDLE-DEADBAND)


void loop() 
{
    // read raw accel/gyro measurements from device
    accelgyro.getMotion9(&ax, &ay, &az, &gx, &gy, &gz, &mx, &my, &mz);

    float xscaled = Scale(ax);
    float yscaled = 1-Scale(az);   // Note!  Using inverted Z scale because it is more comfortable to use the glove on its side.
      
    // Up/down  
    if (xscaled < JLOW)
    {
      digitalWrite(up, LOW);  
      digitalWrite(down, HIGH);   
    }
    else if (xscaled > JHIGH)
    {
      digitalWrite(down, LOW);
      digitalWrite(up, HIGH);  
    }
    else 
    {
      digitalWrite(down, LOW);
      digitalWrite(up, LOW);  
    }
    
    // Left/right
    if (yscaled < JLOW)
    {
      digitalWrite(left, LOW);  
      digitalWrite(right, HIGH);   
    }
    else if (yscaled > JHIGH)
    {
      digitalWrite(right, LOW);
      digitalWrite(left, HIGH);  
    }
    else 
    {
      digitalWrite(right, LOW);
      digitalWrite(left, LOW);  
    }
   
    // Fire button
    int val = !digitalRead(FIREIN);   // Read the input pin & Invert because of the pullup
    digitalWrite(fire, val);
    
    /*
    Serial.print("xscaled=");
    Serial.print(xscaled);
    Serial.print("\t yscaled=");
    Serial.print(yscaled);
    Serial.print("\tfire=");
    Serial.println(val); 
    */
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


