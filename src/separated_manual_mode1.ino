#include <SoftwareSerial.h>

// Motor driver pins
#define ENA 5
#define IN1 4
#define IN2 3
#define ENB 6
#define IN3 2
#define IN4 7

// Bluetooth pins
#define BT_RX 12
#define BT_TX 10

// Ultrasonic pins
#define TRIG 8
#define ECHO 9

// Buzzer, LED, IR sensor
#define LED_PIN 13
#define BUZZER_PIN A0
#define IR_SENSOR_PIN A1

SoftwareSerial BT(BT_RX, BT_TX);

int motorSpeed = 120;
int avoidThreshold = 15;
bool obstacleDetected = false;
bool ledShouldBlink = false;
bool ledState = false;

unsigned long lastCheck = 0;
unsigned long lastBlink = 0;

void setup() {
  // Motor pins
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Ultrasonic pins
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  // Others
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(IR_SENSOR_PIN, INPUT);

  BT.begin(9600);
  stopMotors();
}

void loop() {
  // Check obstacle every 200ms
  if (millis() - lastCheck > 200) {
    lastCheck = millis();
    int dist = readDistance();
    if (dist > 0 && dist < avoidThreshold) {
      obstacleDetected = true;
      stopMotors();
      tone(BUZZER_PIN, 1000);
delay(2000);
noTone(BUZZER_PIN);
ledShouldBlink = false;         // Stop the blinking state
digitalWrite(LED_PIN, LOW);     // Ensure LED is off
// Turn off LED when obstacle detected
    } else {
      obstacleDetected = false;
    }
  }

  // Check if black line detected by IR sensor
  if (!obstacleDetected && digitalRead(IR_SENSOR_PIN) == HIGH) {
    ledShouldBlink = true; // Start blinking and keep blinking
  }

  // LED blink logic (after line detection, until obstacle appears)
  if (!obstacleDetected && ledShouldBlink) {
    if (millis() - lastBlink >= 200) {
      lastBlink = millis();
      ledState = !ledState;
      digitalWrite(LED_PIN, ledState);
    }
  } else {
    digitalWrite(LED_PIN, LOW); // Turn off LED if obstacle or never saw line
  }

  // Bluetooth command handling
  if (BT.available()) {
    String input = BT.readStringUntil('\n');
    processCommand(input);
  }
}

void processCommand(String inputString) {
  if (inputString.length() == 0) return;

  char command = inputString.charAt(0);

  if (obstacleDetected) {
    stopMotors(); // Always stop if obstacle is detected
    return;
  }

  if (command == 'F') forward();
  else if (command == 'B') backward();
  else if (command == 'L') turnLeft();
  else if (command == 'R') turnRight();
  else if (command == 'S') stopMotors();
  else if (command == 'V') {
    int speed = inputString.substring(1).toInt();
    motorSpeed = constrain(speed, 0, 255);
    if (isMoving()) {
      analogWrite(ENA, motorSpeed);
      analogWrite(ENB, motorSpeed);
    }
  }
}

bool isMoving() {
  return digitalRead(IN1) != digitalRead(IN2) || digitalRead(IN3) != digitalRead(IN4);
}

void forward() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  analogWrite(ENA, motorSpeed);
  analogWrite(ENB, motorSpeed);
}

void backward() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  analogWrite(ENA, motorSpeed);
  analogWrite(ENB, motorSpeed);
}

void turnLeft() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  analogWrite(ENA, motorSpeed);
  analogWrite(ENB, motorSpeed);
}

void turnRight() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  analogWrite(ENA, motorSpeed);
  analogWrite(ENB, motorSpeed);
}

void stopMotors() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

int readDistance() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  long duration = pulseIn(ECHO, HIGH, 20000); // timeout after 20ms
  int distance = duration * 0.034 / 2;
  if (distance == 0 || distance > 400) return -1; // out of range
  return distance;
}
