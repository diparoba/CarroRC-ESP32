#include <Arduino.h>
#include <BluetoothSerial.h>
#include "esp_bt.h"

BluetoothSerial SerialBT;

// ==========================
// PINES TB6612FNG
// ==========================

// Motor A (izquierdo)
//const int AIN1 = 27;
const int AIN2 = 33;
const int AIN1 = 25;  // PWM REAL

// Motor B (derecho)
const int BIN1 = 26;
const int BIN2 = 27;
//const int PWMB = 33; // PWM REAL



// ==========================
// FUNCIONES MOTOR A
// ==========================
void motorA_forward() {
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  
}

void motorA_backward() {
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  
}

void motorA_stop() {
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, LOW);
  
}

// ==========================
// FUNCIONES MOTOR B
// ==========================
void motorB_forward() {
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
  
}

void motorB_backward() {
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);
  
}

void motorB_stop() {
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, LOW);
  
}

// ==========================
// MOVIMIENTOS GENERALES
// ==========================
void forward() {
  motorA_forward();
  motorB_forward();
}

void backward() {
  motorA_backward();
  motorB_backward();
}

void left() {
  motorA_backward();
  motorB_forward();
}

void right() {
  motorA_forward();
  motorB_backward();
}

void stopAll() {
  motorA_stop();
  motorB_stop();
}

// ==========================
// SETUP
// ==========================
void setup() {
  Serial.begin(115200);
  delay(500);

  Serial.println("Iniciando Bluetooth...");

  esp_bt_controller_mem_release(ESP_BT_MODE_BLE);
  esp_bt_controller_enable(ESP_BT_MODE_CLASSIC_BT);

  SerialBT.begin("Sebas");
  Serial.println("Bluetooth iniciado correctamente");

  // Configurar control direccional
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);

  stopAll();
  Serial.println("Motores listos con PWM");
}

// ==========================
// LOOP
// ==========================
void loop() {

  if (SerialBT.available()) {
    char cmd = SerialBT.read();

    Serial.print("CMD: ");
    Serial.println(cmd);

    switch (cmd) {
      case 'F': forward();  break;
      case 'B': backward(); break;
      case 'L': left();     break;
      case 'R': right();    break;
      case 'S': stopAll();  break;


    }
  }
}
