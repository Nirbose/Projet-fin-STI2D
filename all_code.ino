#include "Seeed_LDC1612.h"

LDC1612 sensor;

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
  Serial.begin(115200);
  // met les broches de 3 Ã  9 en sortie
  for (int i = 3; i < 9; i++)
    pinMode(i, OUTPUT);
  // met les broches de 11 Ã  13 en sortie
  for (int i = 11; i < 13; i++)
    pinMode(i, OUTPUT);

  pinMode(0, INPUT);
  pinMode(1, INPUT);

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


  float result = ((float)result_channel1 - (float)Vmin) / ((float)Vmax - (float)Vmin);

  Serial.println(result);

  if (result >= 0.1) {
    analogWrite(E1, 0);
    analogWrite(E3, 0);
    return;
  }

  int VD1 = map(analogRead(1), 0, 1023, 255, 0); // Avant Arrière
  int VD2 = map(analogRead(0), 0, 1023, 255, 0); // Droite, Gauche

  Serial.println(VD1);

  if (VD1 >= 115 && VD1 <= 135) {
    analogWrite(E1, 0);
    analogWrite(E3, 0);
  } else if (VD1 > 135) {

    // int speed = map(VD2, 135, 1023, 0, 255);
    int speed = 255 / 2;
    d(1, speed);

  } else {

    // int speed = map(VD2, 0, 115, 255, 0);
    int speed = 255 / 2;
    d(0, speed);

  }

  if (VD2 > 135) {
    turn(RIGHT);

  } else if (VD2 < 115) {
    turn(LEFT);

  }

  delay(500);
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
