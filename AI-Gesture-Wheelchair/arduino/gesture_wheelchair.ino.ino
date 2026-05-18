/*
 * ============================================================
 *   AI Gesture Wheelchair — Arduino Nano Firmware
 * ============================================================
 *
 *   Description:
 *     Receives head gesture commands (L/R/S) from a browser
 *     dashboard via Web Serial API (USB). Controls a servo
 *     motor to steer the wheelchair left, right, or straight.
 *
 *     Ultrasonic sensors on both sides act as a safety override:
 *     if an obstacle is detected within 10cm, the servo is
 *     forcefully returned to 90° (straight/neutral) and LEDs alert.
 *
 *   Gesture → Command → Servo Angle:
 *     Head Straight → 'S' → 90°  (Forward / Neutral)
 *     Head Left     → 'L' → 45°  (Steer Left)
 *     Head Right    → 'R' → 135° (Steer Right)
 *
 *   Pin Map:
 *     D2  → Ultrasonic LEFT  Trig
 *     D3  → Ultrasonic LEFT  Echo
 *     D4  → Ultrasonic RIGHT Trig
 *     D5  → Ultrasonic RIGHT Echo
 *     D6  → LED Left
 *     D7  → LED Right
 *     D9  → Servo Signal
 * ============================================================
 */

#include <Servo.h>

// --- Pin Definitions ---
const int trigL   = 2,  echoL   = 3;   // Left Ultrasonic
const int trigR   = 4,  echoR   = 5;   // Right Ultrasonic
const int ledL    = 6,  ledR    = 7;   // Direction LEDs
const int servoPin = 9;                 // Steering Servo

// --- Servo Angles ---
const int SERVO_CENTER = 90;   // Straight / Forward
const int SERVO_LEFT   = 45;   // Steer Left
const int SERVO_RIGHT  = 135;  // Steer Right

// --- Safety Distance (cm) ---
const int SAFE_DIST = 10;

Servo steeringServo;

// ============================================================
void setup() {
  Serial.begin(9600);
  steeringServo.attach(servoPin);

  pinMode(trigL, OUTPUT); pinMode(echoL, INPUT);
  pinMode(trigR, OUTPUT); pinMode(echoR, INPUT);
  pinMode(ledL,  OUTPUT); pinMode(ledR,  OUTPUT);

  // Start centered
  steeringServo.write(SERVO_CENTER);
  digitalWrite(ledL, LOW);
  digitalWrite(ledR, LOW);

  Serial.println("SYSTEM: AI Gesture Wheelchair Online.");
  Serial.println("SYSTEM: Ultrasonic Sensors Active. Awaiting head gestures...");
}

// ============================================================
// Measure distance in cm using HC-SR04
// Returns 100 as default if no echo (open space)
// ============================================================
long getDistance(int trig, int echo) {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  long duration = pulseIn(echo, HIGH, 30000); // 30ms timeout
  if (duration == 0) return 100;
  return duration * 0.034 / 2;
}

// ============================================================
void loop() {
  long distL = getDistance(trigL, echoL);
  long distR = getDistance(trigR, echoR);

  char cmd = 'S'; // Default: Straight

  // Read head gesture command from browser dashboard
  if (Serial.available() > 0) {
    cmd = Serial.read();
  }

  // --- Print live data to browser serial monitor ---
  Serial.print("LEFT: ");  Serial.print(distL);
  Serial.print("cm | RIGHT: "); Serial.print(distR);
  Serial.print("cm | GESTURE: "); Serial.println(cmd);

  // ============================================================
  //  SAFETY OVERRIDE
  //  If obstacle detected within SAFE_DIST on either side,
  //  ignore gesture and force servo back to center (straight).
  // ============================================================
  if ((distL < SAFE_DIST && distL > 0) || (distR < SAFE_DIST && distR > 0)) {
    steeringServo.write(SERVO_CENTER);
    digitalWrite(ledL, HIGH);
    digitalWrite(ledR, HIGH); // Both LEDs on = Warning
    Serial.println(">>> SAFETY OVERRIDE: Obstacle detected! Forcing STRAIGHT.");
  }

  // ============================================================
  //  GESTURE CONTROL
  //  Path is clear — follow head gesture command
  // ============================================================
  else {
    if (cmd == 'L') {
      steeringServo.write(SERVO_LEFT);
      digitalWrite(ledL, HIGH);
      digitalWrite(ledR, LOW);
      Serial.println("ACTION: Head LEFT → Servo 45°");
    }
    else if (cmd == 'R') {
      steeringServo.write(SERVO_RIGHT);
      digitalWrite(ledR, HIGH);
      digitalWrite(ledL, LOW);
      Serial.println("ACTION: Head RIGHT → Servo 135°");
    }
    else { // 'S' — Head Straight or neutral
      steeringServo.write(SERVO_CENTER);
      digitalWrite(ledL, LOW);
      digitalWrite(ledR, LOW);
    }
  }

  delay(150); // Balance responsiveness with serial readability
}
