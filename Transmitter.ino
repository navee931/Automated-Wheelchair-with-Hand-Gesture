/*
  Project : Automated Wheelchair – RF Transmitter Module
  Author  : Navee
  Platform: Arduino
  Language: C++

  Description:
  - Reads joystick X and Y axis values
  - Transmits data wirelessly using nRF24L01 module
*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// ===================== RF Configuration =====================
RF24 radio(8, 9);                     // CE, CSN pins
const uint64_t pipeOut = 0xF9E8F0F0E1LL;

// ===================== Joystick Pins =====================
int xAxisPin = A0;
int yAxisPin = A1;

// ===================== Data Structure =====================
struct PacketData
{
  byte xAxisValue;
  byte yAxisValue;
} transmitterData;

// ======================================================
void setup()
{
  // Initialize RF Module
  radio.begin();
  radio.setDataRate(RF24_250KBPS);
  radio.openWritingPipe(pipeOut);
  radio.stopListening();
}

// ======================================================
void loop()
{
  // Read joystick values
  transmitterData.xAxisValue = map(analogRead(xAxisPin), 0, 1023, 0, 254);
  transmitterData.yAxisValue = map(analogRead(yAxisPin), 0, 1023, 0, 254);

  // Send data wirelessly
  radio.write(&transmitterData, sizeof(PacketData));

  delay(10);   // Small delay for stable transmission
}
