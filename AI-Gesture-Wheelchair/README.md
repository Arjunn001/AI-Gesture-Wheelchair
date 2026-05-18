# 🦽 AI Gesture Wheelchair

<div align="center">

![AI](https://img.shields.io/badge/AI-Head%20Gesture%20Control-6366f1?style=for-the-badge&logo=tensorflow&logoColor=white)
![Arduino](https://img.shields.io/badge/Arduino-Nano-00979D?style=for-the-badge&logo=arduino&logoColor=white)
![TensorFlow](https://img.shields.io/badge/TensorFlow.js-Pose%20Detection-FF6F00?style=for-the-badge&logo=tensorflow&logoColor=white)
![WebSerial](https://img.shields.io/badge/Web%20Serial-API-38bdf8?style=for-the-badge&logo=googlechrome&logoColor=white)
![License](https://img.shields.io/badge/License-MIT-10b981?style=for-the-badge)

### Hands-free wheelchair steering powered by head gestures and real-time AI

[✨ Features](#-features) · [🔧 Hardware](#-hardware-required) · [🚀 Setup](#-setup) · [📁 Structure](#-project-structure) · [🎮 How It Works](#-how-it-works)

</div>

---

## 💡 Project Overview

**AI Gesture Wheelchair** is an assistive technology project that empowers people with limited hand mobility to control a motorized wheelchair using **head turn gestures** — no hands, no joystick, no button.

A browser dashboard uses **Google Teachable Machine** pose detection to classify the user's head position in real time via webcam. The recognized gesture is sent as a serial command to an **Arduino Nano** via the **Web Serial API**. The Arduino drives a **servo motor** that physically steers the wheelchair mechanism:

- Turn head **Left** → Servo rotates to **45°** (Left)
- Turn head **Right** → Servo rotates to **135°** (Right)
- Head **Straight** → Servo returns to **90°** (Center / Forward)

Two **HC-SR04 ultrasonic sensors** mounted on the left and right sides act as a **collision safety layer** — if an obstacle is detected within 10 cm, the servo is **forcefully returned to center (90°)** regardless of head pose, and LEDs alert the user.

```
[Webcam] ──► [Browser AI: Head Pose Detection]
                        │
                        ▼
              [Web Serial API - USB]
                        │
                        ▼
               [Arduino Nano]
              /            \
      [Servo Motor]     [LEDs L/R]
                ▲
   [Ultrasonic L]  [Ultrasonic R]
   If obstacle < 10cm → Force Servo to 90° (Straight)
```

---

## 🎮 How It Works

| Head Position | AI Class | Serial Command | Servo Angle | Action |
|---------------|----------|----------------|-------------|--------|
| 😐 Straight / Neutral | `STRAIGHT` | `S` | 90° | Center — move forward |
| 👈 Turn Left | `LEFT` | `L` | 45° | Steer Left |
| 👉 Turn Right | `RIGHT` | `R` | 135° | Steer Right |
| 🚨 Obstacle on Left | *(sensor override)* | — | 90° | Force straight + LED alert |
| 🚨 Obstacle on Right | *(sensor override)* | — | 90° | Force straight + LED alert |

> AI commands are sent **only when prediction confidence > 90%** to prevent accidental triggers from subtle movements.

---

## ✨ Features

| Feature | Description |
|--------|-------------|
| 🧠 **Head Gesture Control** | Detects Left / Right / Straight head poses using Teachable Machine Pose model |
| 🤚 **Completely Hands-Free** | Designed for users with limited upper limb mobility |
| 🔌 **Web Serial API** | Direct browser → Arduino USB communication, no extra apps or drivers needed |
| 🛡️ **Obstacle Safety Override** | Ultrasonic sensors force servo to center if obstacle detected within 10 cm |
| 📟 **Live Serial Monitor** | Real-time Arduino sensor + command logs streamed into the browser |
| 💡 **LED Direction Indicators** | Visual left/right turn feedback via onboard LEDs |
| ⚡ **90% Confidence Threshold** | Prevents false triggers from minor unintentional head movements |

---

## 🔧 Hardware Required

| Component | Qty | Connected To |
|-----------|-----|-------------|
| Arduino Nano | 1 | USB to PC/Laptop |
| Servo Motor (SG90 or MG996R) | 1 | Pin D9 |
| HC-SR04 Ultrasonic Sensor | 2 | Left: D2/D3 · Right: D4/D5 |
| LED | 2 | Left: D6 · Right: D7 |
| Webcam | 1 | PC/Laptop (built-in works) |
| USB Cable | 1 | Arduino ↔ PC |
| Jumper Wires + Breadboard | — | Wiring |
| Wheelchair / chassis frame | 1 | Mount all components |

---

## 🔌 Wiring Diagram

```
Arduino Nano
├── D2  → Ultrasonic LEFT  (Trig)
├── D3  → Ultrasonic LEFT  (Echo)
├── D4  → Ultrasonic RIGHT (Trig)
├── D5  → Ultrasonic RIGHT (Echo)
├── D6  → LED Left  (+ 220Ω resistor → GND)
├── D7  → LED Right (+ 220Ω resistor → GND)
├── D9  → Servo Signal Wire
├── 5V  → Servo VCC + Ultrasonic VCC
└── GND → Common Ground
```

---

## 📁 Project Structure

```
AI-Gesture-Wheelchair/
│
├── 📄 README.md                        ← Project documentation (this file)
├── 📄 LICENSE                          ← MIT License
├── 📄 .gitignore
│
├── 🌐 dashboard/
│   └── index.html                      ← Browser AI Dashboard (open in Chrome)
│
└── 🤖 arduino/
    └── gesture_wheelchair.ino          ← Upload this to Arduino Nano
```

---

## 🚀 Setup

### Step 1 — Upload Arduino Code

1. Open **Arduino IDE**
2. Go to `Sketch → Include Library → Manage Libraries` → install **Servo** (if not already installed)
3. Open `arduino/gesture_wheelchair.ino`
4. Select board: `Tools → Board → Arduino Nano`
5. Select port: `Tools → Port → COMxx` (your Arduino's port)
6. Click **Upload ✓**
7. Open Serial Monitor at **9600 baud** to verify: you should see sensor readings printing

### Step 2 — Open the Dashboard

1. Open **Google Chrome** (Web Serial API is required — Firefox won't work)
2. Open `dashboard/index.html` in Chrome
   > 💡 Tip: Use VS Code with the **Live Server** extension for best results

### Step 3 — Connect & Control

1. Click **"START CAMERA"** — loads the AI pose model and activates webcam
2. Click **"CONNECT ARDUINO"** — select your Arduino port from the browser popup
3. Position yourself in front of the webcam so your head and shoulders are visible
4. Control the wheelchair:

```
😐 Face the camera straight   →  Servo stays at 90° (Forward / Neutral)
👈 Turn your head LEFT         →  Servo moves to 45° (Wheelchair steers Left)
👉 Turn your head RIGHT        →  Servo moves to 135° (Wheelchair steers Right)
🚨 Obstacle detected < 10cm   →  Servo forced to 90° (Safety stop)
```

---

## 🖥️ Browser Compatibility

| Browser | Web Serial API | Works? |
|---------|---------------|--------|
| Google Chrome 89+ | ✅ | ✅ Yes |
| Microsoft Edge 89+ | ✅ | ✅ Yes |
| Firefox | ❌ | ❌ No |
| Safari | ❌ | ❌ No |

---

## 🧠 AI Model

The pose detection model is trained using **Google Teachable Machine**:

- **Model URL:** `https://teachablemachine.withgoogle.com/models/fTto7tVwT/`
- **Framework:** TensorFlow.js + Teachable Machine Pose (`@teachablemachine/pose`)
- **Confidence threshold:** 90% — command only sent above this value
- **Classes detected:** `LEFT`, `RIGHT`, `STRAIGHT` (or neutral)

To retrain with your own gestures:
1. Visit [teachablemachine.withgoogle.com](https://teachablemachine.withgoogle.com)
2. Choose **Pose Project**
3. Record samples for: Left Turn, Right Turn, Straight
4. Export and replace the model URL in `dashboard/index.html`

---

## 🤝 Contributing

Pull requests are welcome! For major changes, please open an issue first to discuss what you'd like to change.

1. Fork the repo
2. Create your branch: `git checkout -b feature/your-feature`
3. Commit your changes: `git commit -m 'Add your feature'`
4. Push: `git push origin feature/your-feature`
5. Open a Pull Request

---

## 📜 License

Distributed under the MIT License. See `LICENSE` for details.

---

## 🙌 Acknowledgements

- [Google Teachable Machine](https://teachablemachine.withgoogle.com/) — No-code AI pose model training
- [TensorFlow.js](https://www.tensorflow.org/js) — In-browser machine learning inference
- [Web Serial API](https://developer.mozilla.org/en-US/docs/Web/API/Web_Serial_API) — Direct browser-to-hardware USB communication

---

<div align="center">

Built with purpose — making mobility accessible through AI 🦽✨

</div>
