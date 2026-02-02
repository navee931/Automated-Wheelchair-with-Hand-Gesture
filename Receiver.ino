/*
  Project : Automated Wheelchair – RF Receiver Module
  Author  : Navee
  Platform: Arduino
  Language: C++

  Description:
  - Receives joystick data via nRF24L01
  - Controls left and right motors accordingly
  - Includes signal timeout safety feature
*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Uncomment to enable serial debugging
// #define PRINT_DEBUG

#define SIGNAL_TIMEOUT 500   // Signal timeout in milliseconds

// ===================== RF Configuration =====================
const uint64_t pipeIn = 0xF9E8F0F0E1LL;
RF24 radio(8, 9);              // CE, CSN pins
unsigned long lastRecvTime = 0;

// ===================== Data Structure =====================
struct PacketData
{
  byte xAxisValue;
  byte yAxisValue;
} receiverData;

// ===================== Motor Pins =====================
// Right Motor
int enableRightMotor = 5;
int rightMotorPin1   = 2;
int rightMotorPin2   = 3;

// Left Motor
int enableLeftMotor  = 6;
int leftMotorPin1    = 4;
int leftMotorPin2    = 7;

// ======================================================
void setup()
{
  // Motor Pin Configuration
  pinMode(enableRightMotor, OUTPUT);
  pinMode(rightMotorPin1, OUTPUT);
  pinMode(rightMotorPin2, OUTPUT);

  pinMode(enableLeftMotor, OUTPUT);
  pinMode(leftMotorPin1, OUTPUT);
  pinMode(leftMotorPin2, OUTPUT);

  // Stop motors initially
  rotateMotor(0, 0);

  // RF Module Setup
  radio.begin();
  radio.setDataRate(RF24_250KBPS);
  radio.openReadingPipe(1, pipeIn);
  radio.startListening();

  #ifdef PRINT_DEBUG
    Serial.begin(115200);
  #endif
}

// ======================================================
void loop()
{
  int rightMotorSpeed = 0;
  int leftMotorSpeed  = 0;

  // Check RF connection and availability
  if (radio.isChipConnected() && radio.available())
  {
    radio.read(&receiverData, sizeof(PacketData));

    int mappedYValue = map(receiverData.yAxisValue, 0, 254, -255, 255);
    int mappedXValue = map(receiverData.xAxisValue, 0, 254, -255, 255);

    int motorDirection = 1;
    if (mappedYValue < 0)
    {
      motorDirection = -1;
    }

    // Differential drive calculation
    rightMotorSpeed = abs(mappedYValue) - mappedXValue;
    leftMotorSpeed  = abs(mappedYValue) + mappedXValue;

    rightMotorSpeed = constrain(rightMotorSpeed, 0, 255);
    leftMotorSpeed  = constrain(leftMotorSpeed, 0, 255);

    rotateMotor(rightMotorSpeed * motorDirection,
                leftMotorSpeed * motorDirection);

    lastRecvTime = millis();

    #ifdef PRINT_DEBUG
      Serial.println(receiverData.xAxisValue);
      Serial.println(receiverData.yAxisValue);
    #endif
  }
  else
  {
    // Signal lost → stop motors
    unsigned long now = millis();
    if (now - lastRecvTime > SIGNAL_TIMEOUT)
    {
      rotateMotor(0, 0);
    }
  }
}

// ======================================================
// Motor Control Function
void rotateMotor(int rightMotorSpeed, int leftMotorSpeed)
{
  // Right Motor Direction
  if (rightMotorSpeed < 0)
  {
    digitalWrite(rightMotorPin1, LOW);
    digitalWrite(rightMotorPin2, HIGH);
  }
  else if (rightMotorSpeed > 0)
  {
    digitalWrite(rightMotorPin1, HIGH);
    digitalWrite(rightMotorPin2, LOW);
  }
  else
  {
    digitalWrite(rightMotorPin1, LOW);
    digitalWrite(rightMotorPin2, LOW);
  }

  // Left Motor Direction
  if (leftMotorSpeed < 0)
  {
    digitalWrite(leftMotorPin1, LOW);
    digitalWrite(leftMotorPin2, HIGH);
  }
  else if (leftMotorSpeed > 0)
  {
    digitalWrite(leftMotorPin1, HIGH);
    digitalWrite(leftMotorPin2, LOW);
  }
  else
  {
    digitalWrite(leftMotorPin1, LOW);
    digitalWrite(leftMotorPin2, LOW);
  }

  // Speed Control
  analogWrite(enableRightMotor, abs(rightMotorSpeed));
  analogWrite(enableLeftMotor, abs(leftMotorSpeed));
}
