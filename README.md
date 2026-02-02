# ♿ Automated Wheelchair Prototype with Hand Gesture, Bluetooth & RF Control

📌 Overview

This project presents an Automated Wheelchair Prototype designed to improve mobility and independence for individuals with physical disabilities. The wheelchair supports multiple control methods, including Bluetooth-based smartphone control and wireless RF (nRF24L01) joystick control, making it flexible and user-friendly.

The system is built using Arduino and C++, integrating motors, sensors, wireless modules, and safety features to enable smooth and reliable wheelchair navigation.

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
✨ Key Features

📱 Bluetooth Control
- Control wheelchair movement using a smartphone via Bluetooth
- Button-based directional commands (Forward, Backward, Left, Right, Diagonal)

🎮 Wireless RF Control
- Joystick-based control using nRF24L01 transmitter & receiver
- Smooth differential motor control

🚧 Obstacle Detection
- Dual ultrasonic sensors for left and right obstacle monitoring
- Automatically stops the wheelchair when obstacles are detected within a safe range

🚨 Safety Features
- Panic button to activate a buzzer/alarm
- Signal-loss detection in RF mode (automatic motor stop)

💡 Auxiliary Controls
- Light ON/OFF control via Bluetooth
- 
⚙️ Embedded System Design
- Real-time motor control using Arduino
- Modular and expandable codebase

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
🛠️ Technologies & Components Used

Software
- Language: C++
- Platform: Arduino IDE
  
Hardware
- Arduino Board (Uno & Nano)
- DC Motors with Motor Driver
- Bluetooth Module (HC-05 / HC-06)
- nRF24L01 RF Modules (Transmitter & Receiver)
- Joystick Module
- Ultrasonic Sensors (2)
- Servo / DC Motors
- Buzzer
- Push Button (Panic Button)
- LEDs / Light Module
- Power Supply (Battery)

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
🧠 System Architecture

1️⃣ Bluetooth Control Module
- Receives commands via Bluetooth from a mobile application
- Controls wheelchair movement based on received characters
Handles:
- Motor movement
- Obstacle detection using ultrasonic sensors
- Light control
- Panic alarm activation

2️⃣ RF Transmitter Module
- Reads joystick X and Y axis values
- Maps joystick data into a compact data packet
- Transmits data wirelessly using nRF24L01

3️⃣ RF Receiver Module
- Receives joystick data from the transmitter
- Converts joystick values into motor speed and direction
- Implements differential drive logic
- Stops motors automatically if the RF signal is lost (safety timeout)

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
🎮 Control Logic Summary

Bluetooth Commands
- Command	Action
- C	Move Forward
- D	Move Backward
- R	Turn Right
- K	Turn Left
- F	Forward Right
- G	Forward Left
- S Stop
- A Light ON / OFF
  
RF Control
- Joystick Y-axis → Forward / Backward motion
- Joystick X-axis → Left / Right turning
- Automatic stop if no signal is received for a defined time

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
🔐 Safety & Reliability

- Obstacle detection prevents collision
- Panic button allows immediate alert
- RF signal timeout avoids uncontrolled motion
- Motors stop safely during errors or signal loss

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
🌟 Conclusion

This prototype highlights the potential of combining gesture-based interfaces with wireless technology to create smarter assistive devices. The project aims to empower wheelchair users by providing a more natural, flexible, and independent way to navigate their environment.

👤 Author - Navee
