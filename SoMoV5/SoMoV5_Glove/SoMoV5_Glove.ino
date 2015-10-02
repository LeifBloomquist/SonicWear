// MaxMSP SensorBox demonstration sketch for SoMo V5
//
// This sketch reads the IMU, scales values to the range 0-1024,
// and sends them to MAX using the SensorBox protocol
// ( https://cycling74.com/toolbox/sensorbox/ )
//
// To program, ensure the Arduino IDE is set to "Arduino Leonardo" under Tools->Board
//
// This version used at Nuit Blanche 2015 - modded by Rhys and Leif


/*This code sits on the remote devices (what the dancers will wear)
  in a multiple dancer setup with XBee Series 1 Radios.
  Unit ID for each remote device needs to be set using the keys below:
    Unit 1: 0x01
    Unit 2: 0x02
    Unit 3: 0x03
    Unit 4: 0x04
    Unit 5: 0x05
    Unit 6: 0x06
    Unit 7: 0x07
    Unit 8: 0x08
    Unit 9: 0x09
*/

byte unitID = 0x01;  //Unit ID

#include <SoftwareSerial.h>
#include <Wire.h>
#include <I2Cdev.h>
#include <MPU6050.h>

MPU6050 accelgyro;

int16_t ax, ay, az;
int16_t gx, gy, gz;
int16_t mx, my, mz;

#define POWER 1
#define RX    8   //  Use 17 for SoMo V4
#define TX    4
#define glove 9

SoftwareSerial xbSerial(RX, TX); // RX, TX of the XBee

char analogValue[20]; //array of analog values *in byte format for MAX* plus some buffer
int digVal=0;         //digital pins bits are packed into a single variable
int pointer=0;        //global variable used by packValue__()
int gloveRead=0;      //global variable to read the voltage when the thumb connects with another pad
byte imask = 0x80;    //index bytes start with 1
byte theEnd = 0xFF;   //byte to signal message end to Max patch

void setup()
{
  // Configure serial ports
  Serial.begin(9600);      // USB
  xbSerial.begin(9600);    // XBee

  // initialize device
  Serial.println("Initializing I2C devices...");
  Wire.begin();
  accelgyro.initialize();

  // verify connection
  Serial.println("Testing device connections...");
  Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

  // setup digital inputs
  pinMode(glove, INPUT);

  // Turn on SoMo Power LED
  pinMode(POWER, OUTPUT);
  digitalWrite(POWER, HIGH);
}

void loop()
{
  // Read raw accel/gyro measurements from device
  accelgyro.getMotion9(&ax, &ay, &az, &gx, &gy, &gz, &mx, &my, &mz);
  //Read glove contact pins
  gloveRead = analogRead(glove);

/*
  // Debugging only:  Display tab-separated accel/gyro x/y/z values
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

  // scale inputs to 0-1024 for backwards compatibility with old SensorBox code using pure analog inputs
  int scaled_ax = ScaleMAX(ax);
  int scaled_ay = ScaleMAX(ay);
  int scaled_az = ScaleMAX(az);
  int scaled_gx = ScaleMAX(gx);
  int scaled_gy = ScaleMAX(gy);
  int scaled_gz = ScaleMAX(gz);
  int scaled_mx = ScaleMAX_mag(mx);
  int scaled_my = ScaleMAX_mag(my);
  int scaled_mz = ScaleMAX_mag(mz);


  // Debugging only: display SCALED tab-separated accel/gyro x/y/z values & glove reading
  Serial.print("Scaled:\t");
  Serial.print(scaled_ax);
  Serial.print("\t");
  Serial.print(scaled_ay);
  Serial.print("\t");
  Serial.print(scaled_az);
  Serial.print("\t");
  Serial.print(scaled_gx);
  Serial.print("\t");
  Serial.print(scaled_gy);
  Serial.print("\t");
  Serial.print(scaled_gz);
  Serial.print("\t");
  Serial.print(scaled_mx);
  Serial.print("\t");
  Serial.print(scaled_my);
  Serial.print("\t");
  Serial.print(scaled_mz);
  Serial.print("\t");
  Serial.println(gloveRead);

  // pack the data from the MPU9150 into the SensorBox numbers to transmit
  pointer=0;

  packValueMPU9150(scaled_ax);
  packValueMPU9150(scaled_ay);
  packValueMPU9150(scaled_az);
  packValueMPU9150(scaled_mx);
  packValueMPU9150(scaled_my);
  packValueMPU9150(scaled_mz);
  packValueMPU9150(scaled_gx);
  packValueMPU9150(scaled_gy);
  packValueMPU9150(scaled_gz);
  packValueMPU9150(gloveRead);

  /*
  // Read Digital values
  digVal =  0;
  digVal |= digitalRead(9)*0x01;
  digVal |= digitalRead(10)*0x02;
  */

  // Send the message
  sendMessage();
  delay(10);
}

void packValueMPU9150(int16_t value)
{
  int16_t tempA = value;
  analogValue[pointer] = tempA & 127;
  pointer++;
  analogValue[pointer] = (tempA >> 7);
  pointer++;
}

// Scale the inputs
int ScaleMAX(int raw)
{
   float temp = Scale(raw, -16000, 16000);
   return temp*1024;
}

// Scale the inputs for magnetometer only
int ScaleMAX_mag(int raw)
{
   float temp = Scale(raw, -32000, 32000);
   return temp*1024;
}

// Scale one of the inputs to between 0.0 and 1.0
float Scale(long in, long smin, long smax)
{
  // Bound
  if (in > smax) in=smax;
  if (in < smin) in=smin;

  // Change zero-offset
  in = in-smin;

  // Scale between 0.0 and 1.0 (0.5 would be halfway)
  float temp = (float)in/((float)smax-(float)smin);
  return temp;
}

//Calculates the checksum and sends the message
void sendMessage()
{
  // Send the Unit ID
  //xbSerial.write(unitID);
  // Send analog values in the format 0x81 a1 a2.....0xFF
  // Max wants 81 (start), FF is the end of the message
  xbSerial.write(imask|1); //this is 0x81


  //xbSerial.write((const uint8_t*)analogValue, sizeof(analogValue));

   for (int i = 0;i<pointer;i++)
   {
    xbSerial.write(analogValue[i]);
  }


//  xbSerial.write((imask|2));
//  xbSerial.write((digVal&127));
//  xbSerial.write(digVal>>7);
  xbSerial.write(theEnd);     //ends digital message with 255

/*
  //Print the packet we are sending (debug purposes)
  Serial.print('\n');
  for (pointer = 0; pointer != 37; pointer++) {
    if (data[pointer] < 0x10) {
      Serial.print("0");
    }
    Serial.print(data[pointer], HEX);
  }
   Serial.print('\n');
*/

}
