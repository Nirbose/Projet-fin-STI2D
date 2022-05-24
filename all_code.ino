#include "Seeed_LDC1612.h"
#include <SoftwareSerial.h>

LDC1612 sensor;

// Rx 8 Tx 7
SoftwareSerial bluetooth(8, 9); // Ne pas utiliser le 8 en Rx (Rx / Tx)

String message;

const u32 Vmin = 61391000;
const u32 Vmax = 79337742;
const float Dmax = 15.0;

//identification des broches
const int E1 = 3; ///vitesse moteur avant gauche MOTEUR1
const int E3 = 11; ///vitesse moteur arriÃ¨re gauche MOTEUR3

const int M1 = 4; /// Direction du MOTEUR1 avance si "HIGH"___ recule si "LOW"
const int M3 = 12; ///Direction du MOTEUR3 avance si "HIGH"___ recule si "LOW"


const int RIGHT = 0;
const int LEFT = 1;

const int SPEED = 100;

const int encodeur = 2;
int pulseCount = 0;

int danger = 0;

void setup() {
  Serial.begin(115200);
  bluetooth.begin(115200);
  // met les broches de 3 Ã  9 en sortie
  pinMode(E1, OUTPUT);
  pinMode(E3, OUTPUT);
  pinMode(M1, OUTPUT);
  pinMode(M3, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(encodeur), blink, CHANGE);

  sensor.init();

  if (sensor.single_channel_config(CHANNEL_0))
  {
    Serial.println("can't detect sensor!");
    while (1);
  }
}

void loop() {
  
  u32 result_channel1 = 0;

  /*shows the status of sensor.*/
  //sensor.get_sensor_status();

  /*Get channel 0 result and parse it.*/
  sensor.get_channel_result(CHANNEL_0, &result_channel1);


  float result = ((float)result_channel1 - (float)Vmin) / ((float)Vmax - (float)Vmin); // in %

  if (result >= 0.1) {
    turn(RIGHT);
    danger = 1;
    return;
  }

  if (bluetooth.available()) {

    message = bluetooth.read();

    if (message.toInt() == 4) { // Stop btn
      d(1, 0);
    } else if (message.toInt() == 1) { // Avancée 49

      d(1, SPEED);

    } else if (message.toInt() == 0) { // Reculer

      d(0, SPEED);

    }

    if (message.toInt() == 2) {
      turn(RIGHT);

    } else if (message.toInt() == 3) {
      turn(LEFT);

    }

  }

}

void d(int d, int speed)
{
  
  digitalWrite(M1, d);
  digitalWrite(M3, d);
  analogWrite(E1, speed);
  analogWrite(E3, speed);
}

void turn(int t)
{
  d(0, SPEED);

  digitalWrite(M1, !t);
  digitalWrite(M3, t);
}

void blink()
{
  if (danger) {
    ++pulseCount;

    if (pulseCount == 250) {
      d(1, SPEED);
    }

    if (pulseCount == 750) {
      turn(LEFT);
    }

    if (pulseCount == 1000) {
      d(1, SPEED);
    }

    if (pulseCount == 1500) {
       turn(LEFT);
    }

    if (pulseCount == 1750) {
      d(1, SPEED);
      danger = 0;
      pulseCount = 0;
    }
  }
}
