#include <Servo.h>

const byte TRIG_PIN = 7;
const byte ECHO_PIN = 6;
const byte SERVO_PIN = 9;

const byte EXIT_BUTTON = 12;

// 7-Segment
const byte SEG_A = 2;
const byte SEG_B = 3;
const byte SEG_C = 4;
const byte SEG_D = 5;
const byte SEG_E = 8;
const byte SEG_F = 10;
const byte SEG_G = 11;

const byte MAX_CAPACITY = 4;
byte capacity = MAX_CAPACITY;

const int CAR_DISTANCE = 15;

const int GATE_CLOSED = 90;
const int GATE_OPEN   = 180;

const unsigned long EXIT_OPEN_TIME = 3000;

Servo gateServo;

const byte numbers[10][7] = {
  {1, 1, 1, 1, 1, 1, 0}, // 0
  {0, 1, 1, 0, 0, 0, 0}, // 1
  {1, 1, 0, 1, 1, 0, 1}, // 2
  {1, 1, 1, 1, 0, 0, 1}, // 3
  {0, 1, 1, 0, 0, 1, 1}, // 4
  {1, 0, 1, 1, 0, 1, 1}, // 5
  {1, 0, 1, 1, 1, 1, 1}, // 6
  {1, 1, 1, 0, 0, 0, 0}, // 7
  {1, 1, 1, 1, 1, 1, 1}, // 8
  {1, 1, 1, 1, 0, 1, 1}  // 9
};

void showNumber(byte number) {

  if (number > 9)
    return;

  digitalWrite(SEG_A, !numbers[number][0]);
  digitalWrite(SEG_B, !numbers[number][1]);
  digitalWrite(SEG_C, !numbers[number][2]);
  digitalWrite(SEG_D, !numbers[number][3]);
  digitalWrite(SEG_E, !numbers[number][4]);
  digitalWrite(SEG_F, !numbers[number][5]);
  digitalWrite(SEG_G, !numbers[number][6]);
}

long getDistance() {

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);

  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000);

  if (duration == 0)
    return 999;

  return duration * 0.0343 / 2;
}

void waitForCarToPass() {

  while (getDistance() <= CAR_DISTANCE) {
    delay(100);
  }

  delay(500);
}

void exitCar() {

  if (capacity == MAX_CAPACITY) {
    Serial.println("Parking is already empty.");
    return;
  }

  Serial.println("Exit button pressed.");
  Serial.println("Opening gate for EXIT...");

  gateServo.write(GATE_OPEN);

  delay(EXIT_OPEN_TIME);

  capacity++;

  showNumber(capacity);

  Serial.print("Car exited! Capacity: ");
  Serial.println(capacity);

  gateServo.write(GATE_CLOSED);

  Serial.println("Gate closed.");

  delay(500);
}

void setup() {

  // HC-SR04
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  // EXIT_BUTTON
  pinMode(EXIT_BUTTON, INPUT_PULLUP);

  // 7-Segment
  pinMode(SEG_A, OUTPUT);
  pinMode(SEG_B, OUTPUT);
  pinMode(SEG_C, OUTPUT);
  pinMode(SEG_D, OUTPUT);
  pinMode(SEG_E, OUTPUT);
  pinMode(SEG_F, OUTPUT);
  pinMode(SEG_G, OUTPUT);

  // Servo
  gateServo.attach(SERVO_PIN);

  gateServo.write(GATE_CLOSED);

  showNumber(capacity);

  Serial.begin(9600);

  Serial.println("================================");
  Serial.println("     SMART PARKING SYSTEM");
  Serial.println("================================");
  Serial.print("Capacity: ");
  Serial.println(capacity);
}

void loop() {

  if (digitalRead(EXIT_BUTTON) == LOW) {

    delay(30);

    if (digitalRead(EXIT_BUTTON) == LOW) {

      exitCar();

      while (digitalRead(EXIT_BUTTON) == LOW) {
        delay(10);
      }
    }
  }

  long distance = getDistance();

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print(" cm | Capacity: ");
  Serial.println(capacity);

  if (distance <= CAR_DISTANCE) {

    if (capacity == 0) {

      gateServo.write(GATE_CLOSED);

      Serial.println("PARKING FULL!");

      delay(1000);
      return;
    }

    Serial.println("Car detected!");
    Serial.println("Opening gate...");

    gateServo.write(GATE_OPEN);

    delay(700);

    waitForCarToPass();

    if (capacity > 0) {

      capacity--;

      showNumber(capacity);

      Serial.print("Car entered! Capacity: ");
      Serial.println(capacity);
    }

    delay(500);

    Serial.println("Closing gate...");

    gateServo.write(GATE_CLOSED);

    delay(1500);
  }

  delay(100);
}
