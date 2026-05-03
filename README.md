# 🤖 ART Competition Robot — 1st Place 🥇

> **ART** (Arduino Robotic Training) — organized by **Wameedh Scientific Club**

A dual-mode Arduino robot built for the ART competition. It operates in two modes: an **autonomous line-following + obstacle avoidance** mode, and a **Bluetooth-controlled manual mode** with a custom KODULAR APP Controller remote and a fire-extinguishing pump.

---

## 📁 Repository Structure

```
├── src/
│   ├── autonomous_mode.ino       # Line follower + obstacle avoidance
│   └── manual_mode.ino           # Bluetooth control + fire pump
├── app/
│   ├── controller.aia            # KODULAR source project
│   └── controller.apk            # Ready-to-install Android APK
├── diagram/
│   └── schematic.png             # Full circuit schematic
└── doc/
    └── (robot photos)
```

---

## ⚙️ Modes

### 🔁 Autonomous Mode
The robot follows a black line on a white surface using **3 analog IR sensors** (left, mid, right). It reads sensor values and steers accordingly — forward when the center sees the line, left/right corrections when side sensors pick it up.

If the **HC-SR04 ultrasonic sensor** detects an obstacle closer than 5 cm, the robot stops for 5 seconds then resumes. Sensor readings are logged over Serial for debugging.

**Logic summary:**
- `mid > 750` → go forward
- `left > 750` → turn left (with speed scaling)
- `right > 750` → turn right (with speed scaling)
- Obstacle ≤ 5 cm → stop for 5 seconds

### 🕹️ Manual Mode (Bluetooth + Fire Pump)
Controlled via a custom **KODULAR** Android app over an **HC-05 Bluetooth module**. Commands are single characters sent over serial:

| Command | Action |
|---------|--------|
| `F` | Forward |
| `B` | Backward |
| `L` | Turn Left |
| `R` | Turn Right |
| `S` | Stop |
| `V{0-255}` | Set motor speed |

Additional features in manual mode:
- **IR sensor** detects a black line and blinks the onboard LED as an indicator
- **Ultrasonic sensor** acts as a safety override — if an obstacle is detected within 15 cm, the robot stops and sounds a buzzer regardless of Bluetooth input
- **Water pump** can be activated from the app to extinguish fire

---

## 🔩 Hardware

| Component | Purpose |
|-----------|---------|
| Arduino Uno | Main microcontroller |
| L298N Motor Driver | Controls 4 DC motors |
| HC-SR04 Ultrasonic | Obstacle detection |
| HC-05 Bluetooth Module | Wireless communication (manual mode) |
| IR Sensors (×3) | Line detection (autonomous) / boundary sensing (manual) |
| Water Pump + Relay | Fire extinguishing (manual mode) |
| Buzzer | Obstacle alert |
| LED | Line detection indicator |

---

## 📐 Pin Mapping

### Autonomous Mode
| Pin | Component |
|-----|-----------|
| A0 | Left IR sensor |
| A1 | Mid IR sensor |
| A2 | Right IR sensor |
| 2, 3, 4, 7 | Motor direction pins |
| 5 (ENA), 6 (ENB) | Motor speed (PWM) |
| 8 (TRIG), 9 (ECHO) | Ultrasonic sensor |

### Manual Mode
| Pin | Component |
|-----|-----------|
| 2, 3, 4, 7 | Motor direction pins |
| 5 (ENA), 6 (ENB) | Motor speed (PWM) |
| 8 (TRIG), 9 (ECHO) | Ultrasonic sensor |
| 10 (TX), 12 (RX) | HC-05 Bluetooth |
| 13 | LED indicator |
| A0 | Buzzer |
| A1 | IR sensor |

---

## 📱 KODULAR Controller

The Android app (`.apk` in `/app`) connects via Bluetooth and sends movement commands to the robot. It also includes a button to activate the water pump for fire extinguishing.

To modify the app, open the `.aia` file in [KODULAR](https://www.kodular.io/).

---

## 🚀 Getting Started

1. Wire the circuit according to the schematic in `/diagram`
2. Upload the appropriate `.ino` file using the [Arduino IDE](https://www.arduino.cc/en/softwar)
3. For manual mode: install the APK from `/app` on an Android device and pair with the HC-05 module (default PIN: `1234`)
4. Power on the robot

---

## 🏆 Competition Result

**1st Place — ART (Arduino Robotic Training)**
Organized by Wameedh Scientific Club · Year 1, University

---

## REMARK
This project was done by the help of my mates 
BELLILI ABDELAZIZ
BENHLIMA AMINE 
& BOUCETTA SLIMANE
