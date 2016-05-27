// Code for Getting all the Arduino inputs into MaxMSP
//Andrew Benson
//http://pixlpa.com

#include <SoftwareSerial.h>

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
char analogValue[12];//array of analog values
char current=0;//current position of analog value in array
int digVal;//digital pins bits are packed into a single variable
char imask = 128;//index bytes start with 1
char theEnd = 255;//byte to signal message end to Max patch

void setup(void) {
      // join I2C bus (I2Cdev library doesn't do this automatically)
    Wire.begin();

    // initialize serial communication
    // (38400 chosen because it works as well at 8MHz as it does at 16MHz, but
    // it's really up to you depending on your project)
    Serial.begin(38400);
    while (!Serial) {
      ; // wait for serial port to connect. Needed for Leonardo only
    }

    // initialize device
    Serial.println("Initializing I2C devices...");
    accelgyro.initialize();

    // verify connection
    Serial.println("Testing device connections...");
    Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

  mySerial.begin(9600);
  digVal=0;
  for (int i = 2;i<14;i++){
    digitalWrite(i,HIGH);//enable pullups
  }
  //while (establishContact()==0){delay(100);}  //wait for 99 byte
}
  
//uses serial.write() to avoid needless symbol creation in MaxMSP  
void loop() {
  
      // read raw accel/gyro measurements from device
    accelgyro.getMotion9(&ax, &ay, &az, &gx, &gy, &gz, &mx, &my, &mz);

    // these methods (and a few others) are also available
    //accelgyro.getAcceleration(&ax, &ay, &az);
    //accelgyro.getRotation(&gx, &gy, &gz);

    // display tab-separated accel/gyro x/y/z values
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

  // pack the data from the MPU9150 into the SensorBox numbers to transmit
  // currently only transmitting 6, but there are 9 of them in total
  // the accelerometer (ax, ay, ax) and gyroscope (gx, gy, gz) are 16 bit 
  // the magnetometer (mx, my, mz) are 13 bit
   // SO, we remove bottom 2 bits, since we can only transmit 14
  packValueMPU9150(ax>>2);
  packValueMPU9150(ay>>2);
  //packValueMPU9150(az>>2);
  packValueMPU9150(mx);
  packValueMPU9150(my);
  //packValueMPU9150(mz);
  packValueMPU9150(gx>>2);
  packValueMPU9150(gy>>2);
  //packValueMPU9150(gz>>2);

  char total = current+1;
  sendOFF(total);//send everything to Max
  current=0;//reset analog value counter
  delay(10);//wait 10 milliseconds
  //see if someone tried to turn us off:
  if(establishContact()==1) {
    while(establishContact()==0) {delay(100);}//go into idle mode
  }
}

void sendOFF(char total){
  //Send analog values in the format 0x81 a1 a2.....0xFF
 mySerial.write(imask|1);
  for (int i = 0;i<total;i++){
      mySerial.write(analogValue[i]);   
  }
  mySerial.write(theEnd);//ends analog stream
  
  mySerial.write((imask|2));
  mySerial.write((digVal&127));
  mySerial.write(digVal>>7);
  mySerial.write(theEnd);//ends digital message
}

//read an analog pin and then pack into low/high bytes
void packValue(int index) {
      int tempA = analogRead(index);
      analogValue[current]=tempA & 127;
      current++;
      analogValue[current] = (tempA>>7);
      current++;
}

void packValueMPU9150(int16_t value){
  int16_t tempA = value; 
  analogValue[current] = tempA & 127;
  current++;
  analogValue[current] = (tempA >> 7);
  current++;
}

char establishContact(void){
    if (mySerial.available() > 0) {
      char checkup = mySerial.read();
      if (checkup==99) return 1;
      else return 0;
    }
    else return 0;
}




