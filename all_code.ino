#include "Seeed_LDC1612.h"
#include <SoftwareSerial.h>

LDC1612 sensor;

SoftwareSerial bluetooth(8, 9); // Ne pas utiliser le 8 en Rx (Rx / Tx)

String message;

const u32 Vmin = 61391000;
const u32 Vmax = 79337742;
const float Dmax = 15.0;

//identification des broches
const int E1 = 3; ///vitesse moteur avant gauche MOTEUR1
const int E3 = 6; ///vitesse moteur arriÃ¨re gauche MOTEUR3

const int M1 = 4; /// Direction du MOTEUR1 avance si "HIGH"___ recule si "LOW"
const int M3 = 7; ///Direction du MOTEUR3 avance si "HIGH"___ recule si "LOW"


const int RIGHT = 0;
const int LEFT = 1;
//permet de rÃ©gler la vitesse 0=arrÃ©tÃ© et 255= vitesse max

void setup() {
  Serial.begin(9600);
  bluetooth.begin(9600);
  // met les broches de 3 Ã  9 en sortie
  pinMode(E1, OUTPUT);
  pinMode(E3, OUTPUT);
  pinMode(M1, OUTPUT);
  pinMode(M3, OUTPUT);

  sensor.init();

  if (sensor.single_channel_config(CHANNEL_0))
  {
    Serial.println("can't detect sensor!");
    while (1);
  }
}

void loop() {

  if (bluetooth.available()) {

    message = bluetooth.read();

    Serial.println(message);

    u32 result_channel1 = 0;

    /*shows the status of sensor.*/
    //sensor.get_sensor_status();

    /*Get channel 0 result and parse it.*/
    sensor.get_channel_result(CHANNEL_0, &result_channel1);


    float result = ((float)result_channel1 - (float)Vmin) / ((float)Vmax - (float)Vmin);

    if (result >= 0.1) {
      analogWrite(E1, 0);
      analogWrite(E3, 0);
      return;
    }

    if (message.toInt() == 53) { // Stop btn
      analogWrite(E1, 0);
      analogWrite(E3, 0);
    } else if (message.toInt() == 49) { // Avancée

      int speed = 255 / 2;
      d(1, speed);

    } else if (message.toInt() == 52) { // Reculer

      int speed = 255 / 2;
      d(0, speed);

    }

    if (message.toInt() == 50) {
      turn(RIGHT);

    } else if (message.toInt() == 51) {
      turn(LEFT);

    }
    
  } else {
    
    Serial.println("non");  
  }

  delay(300);
  
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
  Serial.println(t);

  digitalWrite(M1, !t);
  digitalWrite(M3, t);
}
