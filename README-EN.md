🚗 Smart Obstacle Avoidance Car
Arduino-Based Autonomous Car System
📌 Project Description

This project implements a smart obstacle-avoidance car using an ultrasonic sensor to measure the distance between the car and obstacles in front of it.
Based on the measured distance, the car automatically decides how to move.

The car can:

Stop completely

Move slowly

Move at medium speed

Move at high speed

Reverse automatically when an obstacle is too close

The measured distance and current motion state are displayed on an LCD screen, while colored LEDs provide clear visual feedback.

🧠 System Logic

The Arduino sends a trigger pulse to the ultrasonic sensor.

The echo return time is measured.

Distance is calculated in centimeters.

The distance is compared against predefined thresholds.

The system determines:

Motor direction

Motor speed (PWM)

Active LED

Text displayed on the LCD

🚦 Motion States Based on Distance
Distance	Car Behavior	Motor State	LED
< 15 cm and decreasing	Reverse	Slow backward	Red (Blinking)
< 20 cm	Full stop	Stopped	Red
20–30 cm	Slow forward	Low speed	Yellow
30–40 cm	Medium forward	Medium speed	Green
> 40 cm	Fast forward	High speed	Blue

Note: Reverse motion only occurs if the distance is actually decreasing, preventing unnecessary backward movement.

🖥️ LCD Display

Line 1:
Static title

#The Distance :


Line 2:

Measured distance in centimeters

Current motion state (Stop / Slow / Medium / Fast / Reverse)

Example:

25 cm  Slow

⚙️ Motors

Motor 1:
The main active motor responsible for movement (forward / backward with PWM speed control).

Motor 2:
Currently unused and always stopped. It is included in the code and wiring for future expansion.

🚨 LED Indicators
LED Color	Meaning
Blue	Clear path – high speed
Green	Medium speed
Yellow	Slow movement
Red	Stop / danger
Red (Blinking)	Reverse
🔌 Hardware Components

Arduino Uno (or equivalent)

Ultrasonic Sensor (HC-SR04)

16x2 LCD

Motor Driver

DC Motor

LEDs (Blue, Green, Yellow, Red)

Resistors and jumper wires

🧪 Important Notes

Motor 2 is intentionally disabled.

Motor speed is controlled using PWM.

The system continuously reads distance and reacts in real time.

The code structure allows easy modification and extension.

🔮 Future Improvements

Enable the second motor (dual-motor drive).

Add Bluetooth or Wi-Fi remote control.

Implement a steering mechanism.

Log data to a PC or mobile application.

Improve decision logic using FSM or PID control.
