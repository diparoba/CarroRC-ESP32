#include <Arduino.h>
#include <BluetoothSerial.h>
#include "esp_bt.h"

BluetoothSerial SerialBT;

// ==========================
// PINES TB6612FNG
// ==========================

// Motor A (izquierdo)
const int AIN1 = 12;
const int AIN2 = 14;
const int PWMA = 32;  // PWM para controlar velocidad Motor A

// Motor B (derecho)
const int BIN1 = 27;
const int BIN2 = 26;
const int PWMB = 33;  // PWM para controlar velocidad Motor B

// ==========================
// CONFIGURACIÓN PWM
// ==========================
const int PWM_FREQ = 5000;      // Frecuencia PWM 5kHz
const int PWM_RESOLUTION = 8;   // Resolución 8 bits (0-255)
const int CHANNEL_A = 0;        // Canal PWM para Motor A
const int CHANNEL_B = 1;        // Canal PWM para Motor B

// Variables de velocidad
int velocidad_A = 200;  // Velocidad Motor A (0-255)
int velocidad_B = 200;  // Velocidad Motor B (0-255)



// ==========================
// FUNCIONES MOTOR A
// ==========================
void motorA_forward() {
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  ledcWrite(CHANNEL_A, velocidad_A);
}

void motorA_backward() {
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  ledcWrite(CHANNEL_A, velocidad_A);
}

void motorA_stop() {
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, LOW);
  ledcWrite(CHANNEL_A, 0);
}

// ==========================
// FUNCIONES MOTOR B
// ==========================
void motorB_forward() {
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
  ledcWrite(CHANNEL_B, velocidad_B);
}

void motorB_backward() {
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);
  ledcWrite(CHANNEL_B, velocidad_B);
}

void motorB_stop() {
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, LOW);
  ledcWrite(CHANNEL_B, 0);
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
// CONTROL DE VELOCIDAD
// ==========================
void setVelocidad(int vel) {
  // Limitar velocidad entre 0 y 255
  if (vel < 0) vel = 0;
  if (vel > 255) vel = 255;
  
  velocidad_A = vel;
  velocidad_B = vel;
}

void setVelocidadNivel(int nivel) {
  // Mapear nivel 1-10 a velocidad 0-255
  // 1 = 25, 2 = 51, 3 = 76, ..., 10 = 255
  if (nivel < 1) nivel = 1;
  if (nivel > 10) nivel = 10;
  
  int vel = (nivel * 255) / 10;
  setVelocidad(vel);
  
  Serial.print("Nivel de velocidad: ");
  Serial.print(nivel);
  Serial.print("/10 - Velocidad PWM: ");
  Serial.println(vel);
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

  // Configurar pines de dirección como salidas
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);

  // Configurar canales PWM
  ledcSetup(CHANNEL_A, PWM_FREQ, PWM_RESOLUTION);
  ledcSetup(CHANNEL_B, PWM_FREQ, PWM_RESOLUTION);
  
  // Asignar pines PWM a los canales
  ledcAttachPin(PWMA, CHANNEL_A);
  ledcAttachPin(PWMB, CHANNEL_B);

  stopAll();
  Serial.println("Sistema listo - Control PWM activado");
  Serial.println("Comandos: F(adelante) B(atrás) L(izq) R(der) S(parar)");
  Serial.println("Velocidad: 1-9 (niveles) 0 (máximo)");
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
      case 'F': forward();            break;
      case 'B': backward();           break;
      case 'L': left();               break;
      case 'R': right();              break;
      case 'S': stopAll();            break;
      case '0': setVelocidadNivel(1); break;
      case '1': setVelocidadNivel(2); break;
      case '2': setVelocidadNivel(3); break;
      case '3': setVelocidadNivel(4); break;
      case '4': setVelocidadNivel(5); break;
      case '5': setVelocidadNivel(6); break;
      case '6': setVelocidadNivel(7); break;
      case '7': setVelocidadNivel(8); break;
      case '8': setVelocidadNivel(9); break;
      case '9': setVelocidadNivel(10); break; // 0 = Velocidad máxima (nivel 10)
    }
  }
}
