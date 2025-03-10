#define BLYNK_TEMPLATE_ID           "*******"
#define BLYNK_TEMPLATE_NAME         "******"
#define BLYNK_AUTH_TOKEN            "*****" 

#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// Your WiFi credentials
char ssid[] = "*******";
char pass[] = "*******";

// Motor control pins
const int motor1Pin1 = D1; // IN1
const int motor1Pin2 = D2; // IN2
const int motor2Pin1 = D3; // IN3
const int motor2Pin2 = D4; // IN4
const int enableA = D5;     // ENA
const int enableB = D6;     // ENB

BlynkTimer timer;

// Variables to track button states
bool isForwardPressed = false;
bool isBackwardPressed = false;
bool isLeftPressed = false;
bool isRightPressed = false;

void setup() {
  // Debug console
  Serial.begin(115200);

  // Set motor control pins as outputs
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);
  pinMode(enableA, OUTPUT);
  pinMode(enableB, OUTPUT);
  
  // Enable motors
  digitalWrite(enableA, HIGH);
  digitalWrite(enableB, HIGH);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // Set a timer to update motor states
  timer.setInterval(50, updateMotors); // Faster update interval for smoother control
}

void loop() {
  Blynk.run();
  timer.run();
}

// Motor control functions
void moveMotors(int motor1Direction, int motor2Direction) {
  // motor1Direction: 1 for forward, -1 for backward, 0 for stop
  // motor2Direction: 1 for forward, -1 for backward, 0 for stop

  digitalWrite(motor1Pin1, motor1Direction == 1);
  digitalWrite(motor1Pin2, motor1Direction == -1);
  digitalWrite(motor2Pin1, motor2Direction == 1);
  digitalWrite(motor2Pin2, motor2Direction == -1);
}

void stopMotors() {
  moveMotors(0, 0); // Stop both motors
}

// Update motor states based on button inputs
void updateMotors() {
  if (isForwardPressed && isLeftPressed) {
    moveMotors(1, 0); // Forward-right: Left motor forward, right motor stopped
  } else if (isForwardPressed && isRightPressed) {
    moveMotors(0, 1); // Forward-left: Right motor forward, left motor stopped
  } else if (isBackwardPressed && isLeftPressed) {
    moveMotors(-1, 0); // Backward-right: Left motor backward, right motor stopped
  } else if (isBackwardPressed && isRightPressed) {
    moveMotors(0, -1); // Backward-left: Right motor backward, left motor stopped
  } else if (isForwardPressed) {
    moveMotors(1, 1); // Forward
  } else if (isBackwardPressed) {
    moveMotors(-1, -1); // Backward
  } else if (isLeftPressed) {
    moveMotors(-1, 1); // Left turn: Left motor backward, right motor forward
  } else if (isRightPressed) {
    moveMotors(1, -1); // Right turn: Left motor forward, right motor backward
  } else {
    stopMotors(); // No button pressed
  }
}

// Blynk button handlers
BLYNK_WRITE(V0) { // Forward button
  isForwardPressed = param.asInt();
}

BLYNK_WRITE(V1) { // Backward button
  isBackwardPressed = param.asInt();
}

BLYNK_WRITE(V5) { // Left button
  isLeftPressed = param.asInt();
}

BLYNK_WRITE(V6) { // Right button
  isRightPressed = param.asInt();
}

BLYNK_WRITE(V4) { // Stop button
  isForwardPressed = false;
  isBackwardPressed = false;
  isLeftPressed = false;
  isRightPressed = false;
  stopMotors();
}