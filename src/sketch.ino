#include <Servo.h>
#include <ShotPour.h>
#include <stdio.h>
#include <Wire.h>
#include <EasyTransferI2C.h>

EasyTransferI2C ET; 

struct RECEIVE_DATA_STRUCTURE{
  //put your variable definitions here for the data you want to receive
  //THIS MUST BE EXACTLY THE SAME ON THE OTHER ARDUINO
  int winner;
};

//give a name to the group of data
RECEIVE_DATA_STRUCTURE mydata;

//define slave i2c address
#define I2C_SLAVE_ADDRESS 9

const int singleButton = 2;

const int servoPin = 3;
const int blueLight = 4;
const int redLight = 8;
const int yellowLight = 6;
const int greenLight = 5;

ShotPour pourer(servoPin, blueLight, redLight, yellowLight, greenLight);

Servo susanServo;
const int susanPin = 9;

const int cupOne = 10;
const int cupTwo = 42;
const int cupThree = 73;
const int cupFour = 103;
const int cupFive = 135;
const int cupSix = 166;
const int numberOfCups = 6;

void setup()
{
  Wire.begin(I2C_SLAVE_ADDRESS);
  //start the library, pass in the data details and the name of the serial port.
  // Can be Serial, Serial1, Serial2, etc. 
  ET.begin(details(mydata), &Wire);
  //define handler function on receiving data
    Wire.onReceive(receive);

  pinMode(singleButton, INPUT);
  pourer.initialize();

  susanServo.attach(susanPin);
  susanServo.write(cupOne);
}

void loop()
{
  int buttonState;

  buttonState = digitalRead(singleButton);

  if (buttonState == LOW) {
    pourer.pour();
  }

  //check and see if a data packet has come in. 
  if(ET.receiveData()){
    int winner = mydata.winner;
    
    if (winner == 1) {
      moveTo(cupOne);
      pourer.quickPour();
      moveTo(cupTwo);
      pourer.quickPour();
    }
    if (winner == 2) {
      moveTo(cupThree);
      pourer.quickPour();
      moveTo(cupFour);
      pourer.quickPour();
    }
    if (winner == 3) {
      moveTo(cupFive);
      pourer.quickPour();
      moveTo(cupSix);
      pourer.quickPour();
    }
  }

}

void moveTo(int nextPosition) {
  int currentPosition = susanServo.read();

  if (currentPosition < nextPosition) {
    for(int position = currentPosition; position <= nextPosition; position += 1)
    {
      susanServo.write(position);
      delay(50);
    }
    delay(1200);
  }
  else {
    for(int position = currentPosition; position >= nextPosition; position -= 1)
    {
      susanServo.write(position);
      delay(50);
    }
    delay(1200);
  }
}

void receive(int numBytes) {}
