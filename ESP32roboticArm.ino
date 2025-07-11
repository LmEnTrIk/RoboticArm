#include <ESP32Servo.h>

Servo servos[5];
const int servoPins[5] = {19, 18, 17, 16, 15};
const int fingerButtons[5] = {34, 35, 14, 33, 25};
bool fingerStates[5] = {false, false, false, false, false};
bool lastFingerButtons[5] = {true, true, true, true, true};

const int rockBtn     = 26;
const int scissorsBtn = 27;
const int paperBtn    = 12;
bool lastRockState     = true;
bool lastScissorsState = true;
bool lastPaperState    = true;

const int resetBtn = 13;
bool lastResetState = true;

void setup() {
  Serial.begin(115200);
  for (int i = 0; i < 5; i++) {
    servos[i].attach(servoPins[i]);
    servos[i].write(0);
    pinMode(fingerButtons[i], INPUT_PULLUP);
  }

  pinMode(rockBtn, INPUT_PULLUP);
  pinMode(scissorsBtn, INPUT_PULLUP);
  pinMode(paperBtn, INPUT_PULLUP);
  pinMode(resetBtn, INPUT_PULLUP);
}

void loop() {
  // Діагностика стану кнопок
  for (int i = 0; i < 5; i++) {
    Serial.print("B"); Serial.print(i); Serial.print(": ");
    Serial.print(digitalRead(fingerButtons[i]));
    Serial.print("  ");
  }
  Serial.println();
  delay(300);

  //Кнопки пальців
  for (int i = 0; i < 5; i++) {
    bool currentState = digitalRead(fingerButtons[i]);

    if (currentState == LOW && lastFingerButtons[i] == HIGH) {
      fingerStates[i] = !fingerStates[i];
      int angle = fingerStates[i] ? 90 : 0;
      servos[i].write(angle);
      delay(200);
    }

    lastFingerButtons[i] = currentState;
  }

  //Камінь
  bool rockState = digitalRead(rockBtn);
  if (rockState == LOW && lastRockState == HIGH) {
    for (int i = 0; i < 5; i++) servos[i].write(90);
    delay(500);
  }
  lastRockState = rockState;

  //Ножиці
  bool scissorsState = digitalRead(scissorsBtn);
  if (scissorsState == LOW && lastScissorsState == HIGH) {
    servos[0].write(0);
    servos[1].write(0);
    servos[2].write(90);
    servos[3].write(90);
    servos[4].write(90);
    delay(500);
  }
  lastScissorsState = scissorsState;

  //Папір
  bool paperState = digitalRead(paperBtn);
  if (paperState == LOW && lastPaperState == HIGH) {
    for (int i = 0; i < 5; i++) servos[i].write(0);
    delay(500);
  }
  lastPaperState = paperState;

  //Reset
  bool resetState = digitalRead(resetBtn);
  if (resetState == LOW && lastResetState == HIGH) {
    for (int i = 0; i < 5; i++) {
      servos[i].write(0);
      fingerStates[i] = false;
    }
    delay(500);
  }
  lastResetState = resetState;
}
