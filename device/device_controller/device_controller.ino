#include <Servo.h>
Servo uper;
Servo downer;
void setup()
{
  pinMode(2, OUTPUT);
  digitalWrite(2, LOW); 
  pinMode(3, OUTPUT);
  digitalWrite(3, HIGH); 
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  digitalWrite(5, LOW); 
  pinMode(6, OUTPUT);
  digitalWrite(6, HIGH); 
  uper.attach(7);
  downer.attach(6);
  Serial.begin(9600);
}
int ros=90;
void loop()
{
  while (Serial.available() > 0) {
    ros=Serial.parseInt();
    uper.write(ros);
    Serial.print(ros);
    Serial.flush();
  }
}




