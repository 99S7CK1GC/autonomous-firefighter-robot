#define LEFT_IR A0
#define MID_IR A1
#define RIGHT_IR A2

#define LEFT_MOTOR_FWD 4
#define LEFT_MOTOR_REV 3
#define RIGHT_MOTOR_FWD 2
#define RIGHT_MOTOR_REV 7

#define ENA 5
#define ENB 6

#define TRIG 8
#define ECHO 9

int threshold = 600;
int speedmotor = 118;
unsigned long lastObstacleTime = 0;
bool isPaused = false;

void setup() {
  pinMode(LEFT_MOTOR_FWD, OUTPUT);
  pinMode(LEFT_MOTOR_REV, OUTPUT);
  pinMode(RIGHT_MOTOR_FWD, OUTPUT);
  pinMode(RIGHT_MOTOR_REV, OUTPUT);

  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  pinMode(MID_IR , INPUT);
  pinMode(RIGHT_IR , INPUT);
  pinMode(LEFT_IR, INPUT);

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  Serial.begin(9600);
}

void loop() {
  long duration, distance;

  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  duration = pulseIn(ECHO, HIGH);
  distance = duration * 0.034 / 2;

  if (!isPaused && distance > 0 && distance <= 5) {
    stopMotors();
    isPaused = true;
    lastObstacleTime = millis();
  }

  if (isPaused) {
    if (millis() - lastObstacleTime < 5000) {
      return;
    } else {
      isPaused = false;
    }
  }

  int left = analogRead(LEFT_IR);
  int mid = analogRead(MID_IR);
  int right = analogRead(RIGHT_IR);

  Serial.print("L:");
  Serial.print(left);
  Serial.print(" M:");
  Serial.print(mid);
  Serial.print(" R:");
  Serial.println(right);

  if (mid > 750) {
    forward();
  } 
  else if (left > 750) {
    turnLeft();
  } 
  else if (right > 750) {
    turnRight();
  } 
  else if (left > 750 && mid > 750 && right > 750) {
    stopMotors();
  }
}
void forward() {
  digitalWrite(LEFT_MOTOR_FWD, HIGH);
  digitalWrite(LEFT_MOTOR_REV, LOW);
  digitalWrite(RIGHT_MOTOR_FWD, HIGH);
  digitalWrite(RIGHT_MOTOR_REV, LOW);
  analogWrite(ENA, speedmotor);
  analogWrite(ENB, speedmotor);
}

void turnLeft() {
  digitalWrite(LEFT_MOTOR_FWD, LOW);
  digitalWrite(LEFT_MOTOR_REV, HIGH);
  digitalWrite(RIGHT_MOTOR_FWD, HIGH);
  digitalWrite(RIGHT_MOTOR_REV, LOW);
  analogWrite(ENA, speedmotor * 0.78);
  analogWrite(ENB, speedmotor);
}

void turnRight() {
  digitalWrite(LEFT_MOTOR_FWD, HIGH);
  digitalWrite(LEFT_MOTOR_REV, LOW);
  digitalWrite(RIGHT_MOTOR_FWD, LOW);
  digitalWrite(RIGHT_MOTOR_REV, HIGH);
  analogWrite(ENA, speedmotor);
  analogWrite(ENB, speedmotor * 0.78);
}

void stopMotors() {
  digitalWrite(LEFT_MOTOR_FWD, LOW);
  digitalWrite(LEFT_MOTOR_REV, LOW);
  digitalWrite(RIGHT_MOTOR_FWD, LOW);
  digitalWrite(RIGHT_MOTOR_REV, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}
