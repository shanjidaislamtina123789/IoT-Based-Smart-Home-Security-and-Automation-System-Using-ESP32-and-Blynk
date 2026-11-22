//       360 DEGREE SAFETY SHIELD
//   ESP32 + BLYNK + SIM800L GSM SMART HOME SYSTEM
// BLYNK CREDENTIALS
#define BLYNK_TEMPLATE_ID "TMPL62XIo-7f_"
#define BLYNK_TEMPLATE_NAME "Smart home security and automation system "
#define BLYNK_AUTH_TOKEN "YOUR TOKEN"
//  LIBRARIES 
#define TINY_GSM_MODEM_SIM800
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <ESP32Servo.h>
#include <TinyGsmClient.h>
// WIFI CREDENTIALS 
char ssid[] = "NAME";
char pass[] = "PASSWORD";
// GSM CONFIGURATION 
#define SerialMon Serial
#define SerialAT Serial1
#define MODEM_RX 16
#define MODEM_TX 17
#define ADMIN_NUMBER "YOUR NUMBER"
TinyGsm modem(SerialAT);
bool gsmReady = false;
// ENSOR PINS 
const int IR_PIN = 13;
const int MQ2_PIN = 34;
const int FLAME_PIN = 14;
const int RAIN_PIN = 27;
const int BUZZER_PIN = 25;
const int TRIG_PIN = 5;
const int ECHO_PIN = 18;
// SERVO PINS 
const int SERVO1_PIN = 4;
const int SERVO2_PIN = 2;
Servo myServo1;
Servo myServo2;
// BLYNK TERMINAL 
WidgetTerminal terminal(V1);
// ALERT FLAGS 
bool intruderTriggered = false;
bool fireTriggered = false;
bool gasTriggered = false;
// GSM RECONNECTION 
unsigned long lastGSMCheck = 0;
//                  ALERT FUNCTION
void sendAlert(String eventCode, String message) {
  Serial.println();
  Serial.println("========== ALERT ==========");
  Serial.println(message);
  // BLYNK NOTIFICATION 
  if (Blynk.connected()) {
    Blynk.logEvent(eventCode, message);
    terminal.println("--- ALERT ---");
    terminal.println(message);
    terminal.flush();
    Serial.println("Blynk Notification Sent!");
  } else {
    Serial.println("Blynk Not Connected!");
  }
  // GSM SMS 
  if (gsmReady && modem.isNetworkConnected()) {
    bool smsStatus = modem.sendSMS(ADMIN_NUMBER, message);
    if (smsStatus) {
      Serial.println("SMS SENT SUCCESSFULLY!");
    } else {
      Serial.println("SMS SENDING FAILED!");
    }
  } else {
    Serial.println("GSM NOT CONNECTED!");
  }
}
// ULTRASONIC FUNCTION
long getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  long distance = duration * 0.034 / 2;
  return distance;
}
// SETUP
void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("SMART HOME SECURITY SYSTEM STARTING...");
  //SENSOR INITIALIZATION
  pinMode(IR_PIN, INPUT_PULLUP);
  pinMode(FLAME_PIN, INPUT_PULLUP);
  pinMode(RAIN_PIN, INPUT_PULLUP);
  pinMode(MQ2_PIN, INPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  // SERVO INITIALIZATION 
  myServo1.attach(SERVO1_PIN);
  myServo2.attach(SERVO2_PIN);
  myServo1.write(0);
  myServo2.write(0);
  // BUZZER INITIALIZATION 
  ledcAttach(BUZZER_PIN, 2000, 8);
  // GSM INITIALIZATION 
  Serial.println("Initializing GSM Module...");
  SerialAT.begin(9600, SERIAL_8N1, MODEM_RX, MODEM_TX);
  delay(3000);
  modem.restart();
  Serial.println("Waiting for GSM Network...");
  if (modem.waitForNetwork(60000L)) {
    Serial.println("GSM NETWORK CONNECTED!");
    gsmReady = true;
  } else {
    Serial.println("GSM NETWORK FAILED!");
    gsmReady = false;
  }
  // WIFI & BLYNK 
  Serial.println("Connecting to WiFi and Blynk...");
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  Serial.println("BLYNK CONNECTED!");
  Serial.println();
  Serial.println("ALL SYSTEMS INITIALIZED!");
}
//MAIN LOOP
void loop() {
  Blynk.run();
  // GSM RECONNECTION 
  if (!gsmReady && millis() - lastGSMCheck >= 30000) {
    lastGSMCheck = millis();
    Serial.println("Retrying GSM Network...");
    if (modem.waitForNetwork(1000L)) {
      gsmReady = true;
      Serial.println("GSM RECONNECTED!");
    }
  }
  bool servoShouldOpenR = false;
  bool servoShouldOpenU = false;
  bool buzzerActive = false;
  // ULTRASONIC SENSOR
  long distance = getDistance();
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  if (distance > 0 && distance < 20) {
    servoShouldOpenU = true;
  }
  // RAIN SENSOR
  if (digitalRead(RAIN_PIN) == LOW) {
    servoShouldOpenR = true;
  }
  // SERVO CONTROL
  if (servoShouldOpenR) {
    myServo1.write(90);
  } else {
    myServo1.write(0);
  }
  if (servoShouldOpenU) {
    myServo2.write(90);
  } else {
    myServo2.write(0);
  }
  // IR SENSOR
  if (digitalRead(IR_PIN) == LOW) {
    if (!intruderTriggered) {
      Serial.println("INTRUDER DETECTED!");
      sendAlert(
        "intruder_alert",
        "WARNING! INTRUDER DETECTED!"
      );
      intruderTriggered = true;
    }
    buzzerActive = true;
  } else {
    intruderTriggered = false;
  }
  // FLAME SENSOR
  if (digitalRead(FLAME_PIN) == LOW) {
    if (!fireTriggered) {
      Serial.println("FIRE DETECTED!");
      sendAlert(
        "fire_alert",
        "DANGER! FIRE DETECTED!"
      );
      fireTriggered = true;
    }
    buzzerActive = true;
  } else {
    fireTriggered = false;
  }
  // MQ2 GAS SENSOR
  int gas = analogRead(MQ2_PIN);
  Serial.print("GAS VALUE: ");
  Serial.println(gas);
  if (gas > 1500 && gas < 4095) {
    if (!gasTriggered) {
      Serial.println("GAS / SMOKE DETECTED!");
      sendAlert(
        "gas_alert",
        "WARNING! GAS / SMOKE DETECTED!"
      );
      gasTriggered = true;
    }
    buzzerActive = true;
  } else {
    gasTriggered = false;
  }
  //BUZZER CONTROL
  if (buzzerActive) {
    ledcWriteTone(BUZZER_PIN, 2000);
  } else {
    ledcWriteTone(BUZZER_PIN, 0);
  }
  delay(100);
}
