
// 360 DEGREE SAFETY SHIELD
// ESP32 + BLYNK + SIM800L GSM SMART HOME SYSTEM
// BLYNK CREDENTIALS
#define BLYNK_TEMPLATE_ID "TMPL62XIo-7f_"
#define BLYNK_TEMPLATE_NAME "Smart home security and automation system "
#define BLYNK_AUTH_TOKEN "80BlLAwelkMCY54TWy_0hOemFwVE6qJd"

//  LIBRARIES 
#define TINY_GSM_MODEM_SIM800

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <ESP32Servo.h>
#include <TinyGsmClient.h>

// WIFI CREDENTIALS 
char ssid[] = "MARUF";
char pass[] = "@#01828485065#@@";

// GSM CONFIGURATION 
#define SerialMon Serial
#define SerialAT Serial1

#define MODEM_RX 16
#define MODEM_TX 17

#define ADMIN_NUMBER "+8801828485065"

TinyGsm modem(SerialAT);
bool gsmReady = false;

// SENSOR PINS
const int IR_PIN = 13;
const int MQ2_PIN = 34;
const int FLAME_PIN = 14;
const int RAIN_PIN = 27;

const int BUZZER_PIN = 25;

const int TRIG_PIN = 5;
const int ECHO_PIN = 18;

//  SERVO PINS 
const int SERVO1_PIN = 4;   // rain servo
const int SERVO2_PIN = 2;   // door servo

Servo myServo1;
Servo myServo2;

//  MQ2 THRESHOLDS 
const int GAS_THRESHOLD = 1500;     // GAS LEAK
const int SMOKE_THRESHOLD = 2500;   // SMOKE 
const int MQ2_HYSTERESIS = 200;     // alert 

// BLYNK TERMINAL
WidgetTerminal terminal(V1);

//  ALERT FLAGS 
bool intruderTriggered = false;
bool fireTriggered = false;
bool gasTriggered = false;
bool smokeTriggered = false;
bool doorTriggered = false;

// COUNTERS (false trigger ray)
int nearCount = 0;
int farCount = 0;
int rainCount = 0;
int dryCount = 0;

//  TIMERS 
unsigned long lastGSMCheck = 0;
unsigned long lastWifiTry = 0;
unsigned long lastBlynkTry = 0;


// ALERT FUNCTION
void sendAlert(String eventCode, String message) {

  Serial.println();
  Serial.println("========== ALERT ==========");
  Serial.println(message);
  //BLYNK NOTIFICATION 
  if (WiFi.status() == WL_CONNECTED && !Blynk.connected()) {
    Blynk.connect(3000);
  }

  if (Blynk.connected()) {
    Blynk.logEvent(eventCode, message);

    terminal.println("--- ALERT ---");
    terminal.println(message);
    terminal.flush();

    Serial.println("Blynk Notification Sent!");
  } else {
    Serial.println("Blynk Not Connected! (SMS only)");
  }
  //GSM SMS 
  if (gsmReady && modem.isNetworkConnected()) {
    bool smsStatus = modem.sendSMS(ADMIN_NUMBER, message);

    if (smsStatus) {
      Serial.println("SMS SENT SUCCESSFULLY!");
    } else {
      Serial.println("SMS SENDING FAILED!");
    }
  } else {
    Serial.println("GSM NOT CONNECTED!");
    gsmReady = false;
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
  // SENSOR INITIALIZATION
  pinMode(IR_PIN, INPUT_PULLUP);
  pinMode(FLAME_PIN, INPUT_PULLUP);
  pinMode(RAIN_PIN, INPUT_PULLUP);

  pinMode(MQ2_PIN, INPUT);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  //SERVO INITIALIZATION 
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

  // WIFI & BLYNK (NON-BLOCKING)
  Serial.println("Connecting to WiFi...");

  WiFi.mode(WIFI_STA);
  WiFi.setAutoReconnect(true);
  WiFi.begin(ssid, pass);

  unsigned long t = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - t < 10000) {
    delay(250);
  }

  Blynk.config(BLYNK_AUTH_TOKEN);

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("WIFI CONNECTED!");
    if (Blynk.connect(5000)) {
      Serial.println("BLYNK CONNECTED!");
    } else {
      Serial.println("BLYNK NOT CONNECTED YET (will retry)");
    }
  } else {
    Serial.println("WIFI NOT AVAILABLE - running offline (SMS only)");
  }

  Serial.println();
  Serial.println("ALL SYSTEMS INITIALIZED!");
}
//                     MAIN LOOP
void loop() {
  // WIFI / BLYNK HANDLING
  if (WiFi.status() == WL_CONNECTED) {

    if (Blynk.connected()) {
      Blynk.run();
    } else if (millis() - lastBlynkTry > 10000) {
      lastBlynkTry = millis();
      Serial.println("Retrying Blynk...");
      Blynk.connect(3000);
    }

  } else if (millis() - lastWifiTry > 15000) {
    lastWifiTry = millis();
    Serial.println("Retrying WiFi...");
    WiFi.disconnect();
    WiFi.begin(ssid, pass);
  }

  // GSM RECONNECTION 
  if (!gsmReady && millis() - lastGSMCheck >= 30000) {

    lastGSMCheck = millis();

    Serial.println("Retrying GSM Network...");

    if (modem.waitForNetwork(3000L)) {
      gsmReady = true;
      Serial.println("GSM RECONNECTED!");
    }
  }

  bool servoShouldOpenR = false;
  bool servoShouldOpenU = false;

  bool buzzerActive = false;

  //            ULTRASONIC SENSOR (DOOR + SERVO2)

  long distance = getDistance();

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  if (distance > 0 && distance < 20) {
    nearCount++;
    farCount = 0;
  } else {
    nearCount = 0;
    farCount++;
  }

  if (nearCount >= 2) {
    servoShouldOpenU = true;

    if (!doorTriggered) {
      Serial.println("DOOR OPENED!");
      sendAlert("door_alert", "NOTICE! DOOR OPENED (Someone is at the door)");
      doorTriggered = true;
    }
  }

  if (farCount >= 10) {
    doorTriggered = false;
  }

  //            RAIN SENSOR (SERVO1 ONLY, NO ALERT)

  if (digitalRead(RAIN_PIN) == LOW) {
    rainCount++;
    dryCount = 0;
  } else {
    rainCount = 0;
    dryCount++;
  }


  if (rainCount >= 3) {
    servoShouldOpenR = true;
  }
  //  SERVO CONTROL

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
      sendAlert("intruder_alert", "WARNING! INTRUDER DETECTED!");
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
      sendAlert("fire_alert", "DANGER! FIRE DETECTED!");
      fireTriggered = true;
    }

    buzzerActive = true;

  } else {
    fireTriggered = false;
  }

  //   MQ2 SENSOR (GAS + SMOKE )

  int gas = analogRead(MQ2_PIN);

  Serial.print("GAS VALUE: ");
  Serial.println(gas);

  if (gas >= SMOKE_THRESHOLD && gas < 4095) {


    if (!smokeTriggered) {
      Serial.println("SMOKE DETECTED!");
      sendAlert("smoke_alert", "DANGER! SMOKE DETECTED!");
      smokeTriggered = true;
      gasTriggered = true;  
    }

    buzzerActive = true;

  } else if (gas >= GAS_THRESHOLD && gas < 4095) {

    // GAS LEAK
    if (!gasTriggered) {
      Serial.println("GAS LEAK DETECTED!");
      sendAlert("gas_alert", "WARNING! GAS LEAK DETECTED!");
      gasTriggered = true;
    }

    buzzerActive = true;
  }


  if (gas < (GAS_THRESHOLD - MQ2_HYSTERESIS)) {
    gasTriggered = false;
    smokeTriggered = false;
  }
 //   BUZZER CONTROL

  if (buzzerActive) {
    ledcWriteTone(BUZZER_PIN, 2000);
  } else {
    ledcWriteTone(BUZZER_PIN, 0);
  }

  delay(100);
}