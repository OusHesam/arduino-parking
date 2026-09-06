# 🚗 Smart Arduino Parking System

> An Arduino-based smart parking gate system with automatic vehicle detection, real-time parking capacity display, and controlled vehicle entry & exit.

![Arduino](https://img.shields.io/badge/Platform-Arduino-00979D?style=for-the-badge\&logo=arduino\&logoColor=white)
![Language](https://img.shields.io/badge/Language-C%2B%2B-00599C?style=for-the-badge\&logo=cplusplus\&logoColor=white)
![Servo](https://img.shields.io/badge/Servo-Control-orange?style=for-the-badge)
![Ultrasonic](https://img.shields.io/badge/HC--SR04-Ultrasonic-blue?style=for-the-badge)
![License](https://img.shields.io/badge/License-MIT-green?style=for-the-badge)

---

## 📌 About

**Smart Arduino Parking System** is a small-scale automated parking management project built with an Arduino.

The system detects approaching vehicles using an **HC-SR04 ultrasonic sensor**, automatically controls a **servo-powered gate**, keeps track of the number of available parking spaces, and displays the current capacity using a **7-segment display**.

A separate button is provided to manually control vehicle exits.

The project is designed as a simple but practical example of combining:

* 🔊 Ultrasonic distance sensing
* ⚙️ Servo motor control
* 🔢 7-segment display output
* 🔘 Physical button input
* 🧠 Basic parking-space state management
* 🖥️ Serial Monitor debugging

---

## ✨ Features

### 🚘 Automatic Vehicle Detection

The HC-SR04 ultrasonic sensor continuously measures the distance in front of the parking gate.

When a vehicle comes within the configured detection distance, the system:

1. Detects the vehicle.
2. Checks whether parking spaces are available.
3. Opens the gate.
4. Waits for the vehicle to pass.
5. Decreases the available capacity.
6. Updates the 7-segment display.
7. Closes the gate.

---

### 🅿️ Parking Capacity Management

The parking lot has a configurable maximum capacity.

By default:

```cpp
const byte MAX_CAPACITY = 4;
```

The system tracks the number of currently available spaces.

For example:

```text
Initial capacity: 4

Car enters → 3
Car enters → 2
Car enters → 1
Car enters → 0
```

When the capacity reaches `0`, the gate remains closed and the system reports:

```text
PARKING FULL!
```

---

### 🔢 7-Segment Capacity Display

A 7-segment display continuously shows the number of available parking spaces.

The project uses a **Common Anode** 7-segment display.

The displayed value changes automatically whenever a car enters or exits.

Example:

```text
4 → 3 → 2 → 1 → 0
```

---

### 🚪 Automatic Gate Control

A servo motor is used as the parking gate.

The gate has two configurable positions:

```cpp
const int GATE_CLOSED = 90;
const int GATE_OPEN   = 180;
```

When a vehicle is detected, the gate rotates to the open position.

After the vehicle passes, the gate returns to the closed position.

---

### 🔘 Manual Exit Control

A physical push button is used to trigger vehicle exits.

When the button is pressed:

1. The system checks whether the parking lot is already empty.
2. The gate opens.
3. The gate remains open for the configured exit time.
4. Available capacity increases.
5. The 7-segment display is updated.
6. The gate closes again.

Default exit gate-open time:

```cpp
const unsigned long EXIT_OPEN_TIME = 5000;
```

That means the gate remains open for **5 seconds** during a registered exit.

---

### 🧹 Button Debouncing

The exit button uses a simple software debounce mechanism to reduce accidental multiple detections caused by mechanical button bouncing.

The button is also configured using:

```cpp
INPUT_PULLUP
```

---

### 🖥️ Serial Monitor

The system outputs useful information through the Arduino Serial Monitor at:

```text
9600 baud
```

Example output:

```text
================================
     SMART PARKING SYSTEM
================================
Capacity: 4

Distance: 42 cm | Capacity: 4
Distance: 18 cm | Capacity: 4
Distance: 12 cm | Capacity: 4

Car detected!
Opening gate...

Car entered! Capacity: 3
Closing gate...
```

This makes debugging and testing the project much easier.

---

# 🧩 Hardware Requirements

The project can be built using the following components:

| Component         |    Quantity | Purpose                    |
| ----------------- | ----------: | -------------------------- |
| Arduino board     |           1 | Main controller            |
| HC-SR04           |           1 | Vehicle distance detection |
| Servo motor       |           1 | Automatic gate             |
| 7-Segment display |           1 | Available-space display    |
| Push button       |           1 | Manual exit control        |
| Resistors         | As required | 7-segment current limiting |
| Jumper wires      | As required | Connections                |
| Breadboard        |           1 | Prototyping                |
| USB cable         |           1 | Programming and power      |

---

# 🔌 Pin Configuration

## HC-SR04

| HC-SR04 | Arduino |
| ------- | ------- |
| TRIG    | D7      |
| ECHO    | D6      |

---

## Servo Motor

| Servo  | Arduino |
| ------ | ------- |
| Signal | D9      |

---

## Exit Button

| Button | Arduino        |
| ------ | -------------- |
| Signal | D12            |
| Mode   | `INPUT_PULLUP` |

The button is expected to pull the pin **LOW** when pressed.

---

## 7-Segment Display

The project uses the following pins:

| Segment | Arduino |
| ------- | ------- |
| A       | D2      |
| B       | D3      |
| C       | D4      |
| D       | D5      |
| E       | D8      |
| F       | D10     |
| G       | D11     |

Display type:

```text
Common Anode
```

The software accounts for the active-low behavior of the display.

---

# ⚙️ Configuration

Several parameters can easily be modified directly in the source code.

### Maximum Parking Capacity

```cpp
const byte MAX_CAPACITY = 4;
```

Change this value to increase or decrease the maximum number of cars.

Example:

```cpp
const byte MAX_CAPACITY = 9;
```

---

### Vehicle Detection Distance

```cpp
const int CAR_DISTANCE = 15;
```

The default detection threshold is approximately:

```text
15 cm
```

A vehicle closer than this distance is considered detected.

---

### Gate Angles

```cpp
const int GATE_CLOSED = 90;
const int GATE_OPEN   = 180;
```

These values depend on the physical installation and servo orientation.

You may need to adjust them depending on your mechanism.

---

### Exit Gate Duration

```cpp
const unsigned long EXIT_OPEN_TIME = 5000;
```

The value is measured in milliseconds.

For example:

```cpp
5000 = 5 seconds
3000 = 3 seconds
10000 = 10 seconds
```

---

# 🧠 How It Works

The system continuously performs two main tasks:

```text
                ┌─────────────────────┐
                │     Arduino UNO     │
                └──────────┬──────────┘
                           │
          ┌────────────────┼────────────────┐
          │                │                │
          ▼                ▼                ▼
     HC-SR04            Servo          7-Segment
   Car Detection      Gate Control    Capacity Display
          │
          │
          ▼
   Is a car detected?
          │
     ┌────┴────┐
     │         │
    YES        NO
     │         │
     ▼         │
  Capacity?    │
     │         │
  ┌──┴──┐      │
  │     │      │
 >0     0      │
  │     │      │
  ▼     ▼      │
Open   FULL     │
Gate   State    │
  │              │
  ▼              │
Car Passes       │
  │              │
  ▼              │
Capacity--       │
  │              │
  ▼              │
Close Gate ◄─────┘
```

---

# 🚗 Entry Process

When a car approaches the parking entrance:

### 1. Vehicle Detection

The HC-SR04 measures the distance.

```cpp
if (distance <= CAR_DISTANCE)
```

### 2. Capacity Check

The system checks whether available capacity is greater than zero.

### 3. Gate Opens

The servo moves to:

```cpp
GATE_OPEN
```

### 4. Vehicle Pass Detection

The controller waits until the vehicle is no longer detected in front of the ultrasonic sensor.

### 5. Capacity Update

The number of available spaces is decreased:

```cpp
capacity--;
```

### 6. Display Update

The new capacity is shown on the 7-segment display.

### 7. Gate Closes

The servo returns to:

```cpp
GATE_CLOSED
```

---

# 🚪 Exit Process

When the exit button is pressed:

### 1. Empty Parking Check

If there are no cars inside:

```text
Parking is already empty.
```

The gate will not open.

### 2. Gate Opens

The servo moves to the configured open angle.

### 3. Exit Timer

The gate remains open for:

```text
5 seconds
```

by default.

### 4. Capacity Update

The available capacity increases:

```cpp
capacity++;
```

### 5. Display Update

The new value is shown on the 7-segment display.

### 6. Gate Closes

The gate returns to the closed position.

---

# 📂 Project Structure

The main project file is:

```text
arduino-parking/
└── parking.ino
```

Additional documentation and supporting files can be added as the project evolves.

---

# 🛠️ Installation

## 1. Install Arduino IDE

Install the Arduino IDE on your computer.

---

## 2. Open the Project

Open:

```text
parking.ino
```

in Arduino IDE.

---

## 3. Install the Required Library

The project uses the standard Arduino Servo library:

```cpp
#include <Servo.h>
```

In most Arduino IDE installations, this library is already available.

---

## 4. Connect the Hardware

Wire the components according to the pin configuration described above.

---

## 5. Select Your Arduino Board

In Arduino IDE:

```text
Tools → Board
```

Select the board you are using.

---

## 6. Select the Correct Port

Go to:

```text
Tools → Port
```

and select your Arduino's serial port.

---

## 7. Upload

Compile and upload the sketch to the Arduino.

---

## 8. Open Serial Monitor

Set the Serial Monitor to:

```text
9600 baud
```

You should then see the parking system startup information.

---

# ⚠️ Important Notes

### 7-Segment Display

This project is written specifically for a **Common Anode** 7-segment display.

Using a Common Cathode display without modifying the output logic will produce incorrect results.

---

### Servo Power

Depending on the servo motor, powering it directly from the Arduino 5V rail may not be ideal.

Larger servos can require significantly more current than the Arduino should provide.

For larger servos, use an appropriate external power source and ensure that the grounds are connected.

---

### Ultrasonic Sensor Placement

For reliable detection:

* Mount the HC-SR04 at a consistent height.
* Point it toward the vehicle detection area.
* Avoid surfaces that create severe ultrasonic reflections.
* Adjust `CAR_DISTANCE` according to the physical setup.

---

### Parking Capacity

The current implementation is intended for a simple prototype.

It tracks available spaces using the system's internal state and does not individually detect every parking spot.

---

# 🔮 Possible Future Improvements

This project can be expanded significantly.

Potential future versions could include:

* 🚘 Separate entry and exit ultrasonic sensors
* 🅿️ Individual sensors for every parking space
* 📊 Larger capacity displays
* 🚦 Red/green status indicators
* 🔔 Buzzer alerts
* 📱 Bluetooth or Wi-Fi monitoring
* 🌐 Web dashboard
* 📡 ESP32 support
* 📷 License plate recognition
* 💾 EEPROM-based capacity persistence
* ⚡ Non-blocking timing using `millis()`
* 🔐 RFID-based parking access
* ☁️ IoT parking statistics
* 📈 Parking usage analytics

---

# 🧪 Project Status

```text
Status: Working Prototype
```

The current version focuses on the core functionality of:

```text
Vehicle Detection
        ↓
Capacity Check
        ↓
Automatic Gate
        ↓
Capacity Update
        ↓
Display
```

The project is intentionally simple enough to understand, modify, and extend.

---

# 🤝 Contributing

Contributions, improvements, bug fixes, and new ideas are welcome.

You can improve the project by:

1. Forking the repository.
2. Creating a new branch.
3. Making your changes.
4. Testing the hardware/software.
5. Opening a Pull Request.

---

# 📜 License

This project is licensed under the **MIT License**.

You are free to use, modify, distribute, and build upon the project according to the terms of the license.

---

# 👨‍💻 Author

Created by **OusHesam**.

GitHub:

**[@OusHesam](https://github.com/OusHesam)**

Project:

**[arduino-parking](https://github.com/OusHesam/arduino-parking)**

---

## ⭐ Support the Project

If this project helped you learn Arduino, embedded systems, automation, or electronics, consider giving the repository a ⭐ **Star**.

More improvements and hardware projects are planned.

