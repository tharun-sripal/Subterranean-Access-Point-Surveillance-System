#define BLYNK_TEMPLATE_ID "TMPL31gzcANZS"
#define BLYNK_TEMPLATE_NAME "switch"
#define BLYNK_AUTH_TOKEN "vMIPjBuucds7rAzSEpPiiZmNClpl7vJC"
#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <ESP32Servo.h>
Servo myservo; 
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "POCO M3";
char pass[] = "allthebests";

const int trigPin = 5;
const int echoPin = 18;
const int gasPin = 21; // digital gas
const int analogPin = 14; 

#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

long duration;
float distanceCm;
float distanceInch;

BLYNK_WRITE(V0){
  int pin=param.asInt();
  digitalWrite(2,pin);
  

  }

 /* BLYNK_WRITE(V2){
  int pin2=param.asInt();
  digitalWrite(V2,HIGH);
  

  }*/

WidgetLCD lcd2(V1);

void setup() {
  Blynk.begin(auth, ssid, pass);
  pinMode(2,OUTPUT);

  Serial.begin(9600); 
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT); 
  // put your setup code here, to run once:
  pinMode(gasPin, INPUT);//digital
  pinMode(23 , INPUT);//vibration
  pinMode(22 , OUTPUT);//buzzer
  myservo.attach(26);
  myservo.write(0);

}
int a,b,c;
void loop() {
  a=digitalRead(23);
  if(a==1){
    digitalWrite(22,HIGH);
    Serial.println("vibration detected ");
    Blynk.virtualWrite(V2, HIGH);
  }
  else{
    digitalWrite(22,LOW);  
    Serial.println("vibration not detected");
    Blynk.virtualWrite(V2,LOW);
  }
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distanceCm = duration * SOUND_SPEED/2;
  distanceInch = distanceCm * CM_TO_INCH;
  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);
  lcd2.print(0,0,"Distance (cm):");
  lcd2.print(0,1,distanceCm);
  if(distanceCm<=5){
    myservo.write(113);
    delay(1000);
    }
  else{
    myservo.write(0);
    }
  b=digitalRead(21);
  if(b==0){
    digitalWrite(21,HIGH);
    Serial.println("fire detected");
    Blynk.virtualWrite(V3, HIGH);
  }
  else{
    digitalWrite(21,LOW);  
    Serial.println("fire not detected"); 
    Blynk.virtualWrite(V3,LOW);
  }
  //cb=map(c,0,4095,0,255);
   c = analogRead(analogPin);
  Serial.print("Analog value: ");
  Serial.println(c);
  Blynk.virtualWrite(V4,c);
  Blynk.run();
}
