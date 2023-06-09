#include "controller.hpp"

#include <Arduino.h>

void setup_controller() {
  pinMode(LEFT_FORWARD, OUTPUT);
  pinMode(RIGHT_FORWARD, OUTPUT);
  pinMode(LEFT_BACKWARD, OUTPUT);
  pinMode(RIGHT_BACKWARD, OUTPUT);
}

void move_forward() {
  digitalWrite(LEFT_FORWARD, HIGH);
  digitalWrite(RIGHT_FORWARD, HIGH);
}

void move_backward() {
  digitalWrite(LEFT_BACKWARD, HIGH);
  digitalWrite(RIGHT_BACKWARD, HIGH);
}

void turn_left() {
  digitalWrite(LEFT_BACKWARD, HIGH);
  digitalWrite(RIGHT_FORWARD, HIGH);
  delay(500);
  stop();
}

void turn_right() {
  digitalWrite(LEFT_FORWARD, HIGH);
  digitalWrite(RIGHT_BACKWARD, HIGH);
  delay(500);
  stop();
}

void stop() {
  digitalWrite(LEFT_FORWARD, LOW);
  digitalWrite(RIGHT_FORWARD, LOW);
  digitalWrite(LEFT_BACKWARD, LOW);
  digitalWrite(RIGHT_BACKWARD, LOW);
}