#define BLYNK_TEMPLATE_ID "TMPL31gzcANZS"
#define BLYNK_TEMPLATE_NAME "switch"
#define BLYNK_AUTH_TOKEN "vMIPjBuucds7rAzSEpPiiZmNClpl7vJC"
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <ESP32Servo.h>
#include <DHT.h>

// Servo objects
Servo myservo; 
Servo myservo2;

// Blynk authentication
char auth[] = BLYNK_AUTH_TOKEN;

// Wi-Fi credentials
char ssid[] = "POCO M3";
char pass[] = "allthebests";

// Sensor pins
const int trigPin = 5;
const int echoPin = 18;
const int gasPin = 21; // digital gas
const int analogPin = 14;
const int dhtPin = 19; // DHT sensor pin

#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

// DHT sensor type
#define DHTTYPE DHT22

long duration;
float distanceCm;
float distanceInch;

// Create DHT object
DHT dht(dhtPin, DHTTYPE);

// Blynk virtual pins
BLYNK_WRITE(V0){
  int pin = param.asInt();
  digitalWrite(2, pin);
}

// LCD widgets
WidgetLCD lcd2(V1);
WidgetLCD lcd5(V5);

void setup() {
  // Start Blynk
  Blynk.begin(auth, ssid, pass);

  // Initialize serial communication
  Serial.begin(9600);

  // Initialize pins
  pinMode(2, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(gasPin, INPUT); // digital
  pinMode(23, INPUT); // vibration
  pinMode(22, OUTPUT); // buzzer
  pinMode(analogPin, INPUT); // analog gas

  // Attach servos
  myservo.attach(26);
  myservo.write(113);
  myservo2.attach(27);
  myservo2.write(113);

  // Initialize DHT sensor
  dht.begin();
}

int a, b, c;

void loop() {
  // Read analog gas sensor
  c = analogRead(analogPin);
  Serial.print("Analog value: ");
  Serial.println(c);
  Blynk.virtualWrite(V4, c);

  // Read vibration sensor
  a = digitalRead(23);
  if (a == 1) {
    digitalWrite(22, HIGH);
    Serial.println("Vibration detected");
    Blynk.virtualWrite(V2, HIGH);
  } else {
    digitalWrite(22, LOW);
    Serial.println("Vibration not detected");
    Blynk.virtualWrite(V2, LOW);
  }

  // Measure distance using ultrasonic sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distanceCm = duration * SOUND_SPEED / 2;
  distanceInch = distanceCm * CM_TO_INCH;

  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);
  lcd2.print(0, 0, "Distance (cm):");
  lcd2.print(0, 1, distanceCm);

  if (distanceCm <= 8) {
    lcd5.print(0, 0, "Garbage detected");
    lcd5.print(0, 1, "Cleaning        ");
    myservo2.write(0);
    delay(1000);
    myservo.write(0);
    delay(500);
    myservo.write(113);
    delay(200);
  } else {
    lcd5.print(0, 0, "Valve closed");
    lcd5.print(0, 1, "Clean & Secure");
    myservo.write(113);
    myservo2.write(113);
  }

  // Read digital gas sensor
  b = digitalRead(gasPin);
  if (b == 0) {
    digitalWrite(22, HIGH);
    Serial.println("Fire detected");
    Blynk.virtualWrite(V3, HIGH);
  } else {
    digitalWrite(22, LOW);
    Serial.println("Fire not detected");
    Blynk.virtualWrite(V3, LOW);
  }

  // Read DHT sensor
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // Check if any reads failed and exit early (to try again)
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
  } else {
    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print(" %\t");
    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.println(" *C");

    // Send DHT values to Blynk
    Blynk.virtualWrite(V6, h);
    Blynk.virtualWrite(V7, t);
  }

  // Run Blynk
  Blynk.run();
}
