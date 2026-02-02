/*
  Project : Automated Wheelchair with Bluetooth & Gesture Control
  Author  : Navee
  Platform: Arduino
  Language: C++
*/

// ===================== Motor Pins =====================
int motorRightA = 8;     // Right Motor - Clockwise
int motorRightB = 9;     // Right Motor - Anti-clockwise
int motorLeftA  = 11;    // Left Motor - Clockwise
int motorLeftB  = 10;    // Left Motor - Anti-clockwise

// ===================== Ultrasonic Sensor 1 (Right) =====================
int trigPin1 = 12;
int echoPin1 = 13;
long duration1;
int distance1;

// ===================== Ultrasonic Sensor 2 (Left) =====================
int trigPin2 = 7;
int echoPin2 = 6;
long duration2;
int distance2;

// ===================== Other Components =====================
int light = 5;
int buzzer = 4;
int pushButton = 3;

// ===================== Bluetooth =====================
char data_in = 0;

// ===================== Timing =====================
int update_interval = 100;
unsigned long last_time = 0;

// ======================================================
void setup()
{
  // Motor Pins
  pinMode(motorRightA, OUTPUT);
  pinMode(motorRightB, OUTPUT);
  pinMode(motorLeftA, OUTPUT);
  pinMode(motorLeftB, OUTPUT);

  // Ultrasonic Sensors
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);

  // Output Devices
  pinMode(light, OUTPUT);
  pinMode(buzzer, OUTPUT);

  // Panic Button
  pinMode(pushButton, INPUT_PULLUP);

  // Bluetooth Communication
  Serial.begin(9600);

  // App Panel Configuration
  Serial.println("*.kwl");
  Serial.println("clear_panel()");
  Serial.println("set_grid_size(14,7)");
  Serial.println("add_text_box(3,6,3,L,wheel chair ,245,240,245,)");
  Serial.println("add_text_box(8,4,3,L,led,245,240,245,)");
  Serial.println("add_text_box(11,4,3,L,alarm ,245,240,245,)");
  Serial.println("add_button(4,1,2,C,c)");
  Serial.println("add_button(6,2,11,F,f)");
  Serial.println("add_button(6,4,5,R,r)");
  Serial.println("add_button(4,5,3,D,d)");
  Serial.println("add_button(2,4,12,G,g)");
  Serial.println("add_button(2,2,4,K,k)");
  Serial.println("add_button(4,3,14,S,s)");
  Serial.println("add_switch(8,2,4,A,a,0,0)");
  Serial.println("add_switch(11,2,4,B,b,0,0)");
  Serial.println("set_panel_notes(,,,)");
  Serial.println("run()");
  Serial.println("*");
}

// ======================================================
void loop()
{
  // Light Control
  lightOnOff();

  // Panic Button
  panicSound();

  // -------- Right Sensor --------
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);

  duration1 = pulseIn(echoPin1, HIGH);
  distance1 = duration1 * 0.034 / 2;

  Serial.print("Right Distance: ");
  Serial.println(distance1);

  // -------- Left Sensor --------
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);

  duration2 = pulseIn(echoPin2, HIGH);
  distance2 = duration2 * 0.034 / 2;

  Serial.print("Left Distance: ");
  Serial.println(distance2);

  // -------- Obstacle Detection --------
  if (distance1 <= 20 || distance2 <= 20)
  {
    // Stop Wheelchair
    digitalWrite(motorRightA, LOW);
    digitalWrite(motorRightB, LOW);
    digitalWrite(motorLeftA, LOW);
    digitalWrite(motorLeftB, LOW);

    control();
  }
  else
  {
    control();
  }
}

// ======================================================
// Wheelchair Control Logic (Bluetooth Commands)
void control()
{
  if (Serial.available() > 0)
  {
    data_in = Serial.read();

    if (data_in == 'C')          // Forward
    {
      digitalWrite(motorRightA, HIGH);
      digitalWrite(motorLeftA, HIGH);
    }
    else if (data_in == 'c')     // Stop
    {
      digitalWrite(motorRightA, LOW);
      digitalWrite(motorLeftA, LOW);
    }

    if (data_in == 'D')          // Backward
    {
      digitalWrite(motorRightB, HIGH);
      digitalWrite(motorLeftB, HIGH);
    }
    else if (data_in == 'd')     // Stop
    {
      digitalWrite(motorRightB, LOW);
      digitalWrite(motorLeftB, LOW);
    }

    else if (data_in == 'R')     // Right
    {
      digitalWrite(motorRightA, LOW);
      digitalWrite(motorRightB, LOW);
      digitalWrite(motorLeftA, HIGH);
      digitalWrite(motorLeftB, LOW);
    }
    else if (data_in == 'r')     // Stop
    {
      digitalWrite(motorRightA, LOW);
      digitalWrite(motorRightB, LOW);
      digitalWrite(motorLeftA, LOW);
      digitalWrite(motorLeftB, LOW);
    }

    else if (data_in == 'K')     // Left
    {
      digitalWrite(motorRightA, HIGH);
      digitalWrite(motorRightB, LOW);
      digitalWrite(motorLeftA, LOW);
      digitalWrite(motorLeftB, LOW);
    }
    else if (data_in == 'k')     // Stop
    {
      digitalWrite(motorRightA, LOW);
      digitalWrite(motorRightB, LOW);
      digitalWrite(motorLeftA, LOW);
      digitalWrite(motorLeftB, LOW);
    }

    else if (data_in == 'F')     // Forward Right
    {
      digitalWrite(motorRightA, HIGH);
      digitalWrite(motorRightB, LOW);
      digitalWrite(motorLeftA, LOW);
      digitalWrite(motorLeftB, HIGH);
    }
    else if (data_in == 'f')     // Stop
    {
      digitalWrite(motorRightA, LOW);
      digitalWrite(motorRightB, LOW);
      digitalWrite(motorLeftA, LOW);
      digitalWrite(motorLeftB, LOW);
    }

    else if (data_in == 'G')     // Forward Left
    {
      digitalWrite(motorRightA, LOW);
      digitalWrite(motorRightB, HIGH);
      digitalWrite(motorLeftA, HIGH);
      digitalWrite(motorLeftB, LOW);
    }
    else if (data_in == 'g')     // Stop
    {
      digitalWrite(motorRightA, LOW);
      digitalWrite(motorRightB, LOW);
      digitalWrite(motorLeftA, LOW);
      digitalWrite(motorLeftB, LOW);
    }
  }
}

// ======================================================
void lightOnOff()
{
  if (data_in == 'A')
  {
    digitalWrite(light, HIGH);
  }
  else if (data_in == 'a')
  {
    digitalWrite(light, LOW);
  }
}

// ======================================================
void panicSound()
{
  int val = digitalRead(pushButton);

  if (val == LOW)
  {
    digitalWrite(buzzer, HIGH);
  }
  else
  {
    digitalWrite(buzzer, LOW);
  }
}
