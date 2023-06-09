#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <NewPing.h>
#include <Servo.h>
#include <WebSocketsClient.h>

#include "controller.hpp"

#define SSID "Galaxy M13 5G"
#define PASSWD "littleb0is"

#define LED D0
#define TRIGGER_PIN D1
#define ECHO_PIN D2
#define SERVO D3
#define MAX_DISTANCE 200
#define SERVER_IP "192.168.176.206"
#define SERVER_PORT 8080
#define DISTANCE_THRESHOLD 40

Servo servo;
WebSocketsClient webSocketClient;
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
char state = 'S';

void state_update(char s) {
  if (s == 'F') {
    move_forward();
    state = 'F';
  }
  if (s == 'B') {
    move_backward();
    state = 'B';
  }
  if (s == 'S') {
    stop();
    state = 'S';
  }
  if (s == 'L') {
    turn_left();
    state = 'L';
  }
  if (s == 'R') {
    turn_right();
    state = 'R';
  }
}

void webSocketEvent(WStype_t type, uint8_t* payload, size_t length) {
  if (type == WStype_CONNECTED) {
    Serial.println("[WebSocket] Connected");
    webSocketClient.sendTXT("NODE");
  } else if (type == WStype_DISCONNECTED) {
    Serial.println("[WebSocket] Disconnected");
  } else if (type == WStype_TEXT) {
    Serial.print("[WebSocket] Message received: ");
    Serial.println((char*)payload);
    state_update(payload[0]);
  }
}

void setup() {
  Serial.begin(115200);
  setup_controller();
  stop();

  WiFi.begin(SSID, PASSWD);
  pinMode(LED, OUTPUT);
  servo.attach(SERVO);

  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED, HIGH);
    delay(500);
    digitalWrite(LED, LOW);
    delay(500);
    Serial.println("Connecting...");
  }

  webSocketClient.begin(SERVER_IP, SERVER_PORT, "/");
  webSocketClient.onEvent(webSocketEvent);

  digitalWrite(LED, HIGH);
}

void loop() {
  webSocketClient.loop();
  delay(100);
  unsigned int distance = sonar.ping_cm();
  Serial.println(distance);
  if (distance > DISTANCE_THRESHOLD || distance == 0) {
    ;
  } else {
    // object exists
    stop();
    servo.write(0);
    delay(1000);
    distance = sonar.ping_cm();
    if (distance > DISTANCE_THRESHOLD) {
      turn_left();
      move_forward();
      delay(100);
      turn_right();
      state_update(state);
      return;
    }
    servo.write(180);
    delay(1000);
    distance = sonar.ping_cm();
    if (distance > DISTANCE_THRESHOLD) {
      turn_right();
      move_forward();
      delay(300);
      turn_left();
      state_update(state);
      return;
    }

    stop();
    for (int i = 0; i < 1000; ++i) {
      digitalWrite(LED, HIGH);
      delay(200);
      digitalWrite(LED, LOW);
      delay(200);
    }
    return;
  }
}