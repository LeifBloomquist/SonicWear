//SoMo Backup Code for three remote devices sending to Max (Series 1 Radios)
/*  Unit ID for each remote device needs to be set in the global declarations using the keys below:
 
 Unit 0: 0x30
 Unit 1: 0x31
 Unit 2: 0x32
 Unit 3: 0x33
 Unit 4: 0x34
 Unit 5: 0x35
 Unit 6: 0x36
 Unit 7: 0x37
 Unit 8: 0x38
 Unit 9: 0x39
 */

#include <SoftwareSerial.h>
#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050.h"
MPU6050 accelgyro; 

int16_t ax, ay, az;
int16_t gx, gy, gz;
int16_t mx, my, mz;


SoftwareSerial mySerial(MOSI, 4); // RX, TX

char analogValue[20];               //array of analog values *in byte format for MAX* plus some buffer

char current=0;//current position of analog value in array
int digVal;//digital pins bits are packed into a single variable
char imask = 128;//index bytes start with 1
char theEnd = 255;//byte to signal message end to Max patch
//SET UNIT ID HERE
//*******************************
byte unitID = 0x30;
//*******************************


void setup(void) {
  Wire.begin();
  Serial.begin(38400);  

  // initialize device
  Serial.println("Initializing I2C devices...");
  accelgyro.initialize();

  // verify connection
  Serial.println("Testing device connections...");
  Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

  mySerial.begin(9600);  //this serial corresponds to MAX PATCH
  digVal=0;
  for (int i = 2;i<14;i++){
    digitalWrite(i,HIGH);//enable pullups
  }

  while (establishContact()==0){
    Serial.println("Waiting for connection from MAX...");
    delay(100);
  }  //wait for 99 byte
}

//uses serial.write() to avoid needless symbol creation in MaxMSP  
void loop() {
  // read raw accel/gyro measurements from device
  accelgyro.getMotion9(&ax, &ay, &az, &gx, &gy, &gz, &mx, &my, &mz);

  // display tab-separated accel/gyro x/y/z values
/*
Serial.print("a/g/m:\t");
  Serial.print(ax); 
  Serial.print("\t");
  Serial.print(ay); 
  Serial.print("\t");
  Serial.print(az); 
  Serial.print("\t");
  Serial.print(gx); 
  Serial.print("\t");
  Serial.print(gy); 
  Serial.print("\t");
  Serial.print(gz); 
  Serial.print("\t");
  Serial.print(mx); 
  Serial.print("\t");
  Serial.print(my); 
  Serial.print("\t");
  Serial.println(mz);
*/
  
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
  
   // display SCALED tab-separated accel/gyro x/y/z values
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
  Serial.println(scaled_mz);

  // pack the data from the MPU9150 into the SensorBox numbers to transmit
  // currently only transmitting 6, but there are 9 of them in total
  // the accelerometer (ax, ay, ax) and gyroscope (gx, gy, gz) are 16 bit 
  // the magnetometer (mx, my, mz) are 13 bit
  // SO, we remove bottom 2 bits, since we can only transmit 14
//  packValueMPU9150(ax>>2);     //Eric added the <<2 because of the above note re: bits
//  packValueMPU9150(ay>>2);
//  packValueMPU9150(az>>2);
//  packValueMPU9150(mx);
//  packValueMPU9150(my);
//  packValueMPU9150(mz);
//  packValueMPU9150(gx>>2);
//  packValueMPU9150(gy>>2);
//  packValueMPU9150(gz>>2);

  current=0;//reset analog value counter
  
  packValueMPU9150(scaled_ax); 
  packValueMPU9150(scaled_ay);
  packValueMPU9150(scaled_az);
  packValueMPU9150(scaled_mx);
  packValueMPU9150(scaled_my);
  packValueMPU9150(scaled_mz);
  packValueMPU9150(scaled_gx);
  packValueMPU9150(scaled_gy);
  packValueMPU9150(scaled_gz);

  char total = current; //+1;
  
  sendOFF(total);//send everything to Max 
  
  delay(10);//wait 10 milliseconds
  //see if someone tried to turn us off:
  if(establishContact()==1) {
    while(establishContact()==0) {
      delay(100);
    }//go into idle mode
  }
}

void sendOFF(char total){
  //Send analog values in the format 0x81 a1 a2.....0xFF
  //Max wants 81 (start), FF is the end of the message
  mySerial.write(imask|1); //this is 0x81
  //Write UnitID to MAX
  //**********************************
  // Turn On/Off Serial ID
  //mySerial.write(unitID);
 //***********************************
  for (int i = 0;i<total;i++){
    mySerial.write(analogValue[i]);   
  }
  mySerial.write(theEnd);//ends analog stream with 255

/*  mySerial.write((imask|2));
  mySerial.write((digVal&127));
  mySerial.write(digVal>>7);
  mySerial.write(theEnd);//ends digital message with 255
*/
}

//read an analog pin and then pack into low/high bytes
/*
void packValue(int index) {
  int tempA = analogRead(index);
  analogValue[current]=tempA & 127;
  current++;
  analogValue[current] = (tempA>>7);
  current++;
}
*/

void packValueMPU9150(int16_t value){
  int16_t tempA = value; 
  analogValue[current] = tempA & 127;
  current++;
  analogValue[current] = (tempA >> 7);
  current++;
}

/////////////////////////////////////////
//This looks for connection to MaxMSP
char establishContact(void){
  if (mySerial.available() > 0) {
    char checkup = mySerial.read();
    if (checkup==99) return 1;  //looking for you to click 99 in Max to run the data -- maybe comment this out
    else return 0;
  }
  else return 0;
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
