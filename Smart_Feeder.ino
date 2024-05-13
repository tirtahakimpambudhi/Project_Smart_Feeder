#define BLYNK_TEMPLATE_ID "YOUR_BLYNK_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "YOUR_BLYNK_TEMPLATE_NAME"
#define BLYNK_AUTH_TOKEN "YOUR_BLYNK_AUTH_TOKEN"

#include <BlynkSimpleEsp8266.h>  
#include <LiquidCrystal_I2C.h>
#include <SmoothProgress.h>
#include <BarStyle1.h>
#include <Servo.h>
#include "utils.h"



const int buttonPin = D4 ;
const int trigPin = D7;
const int echoPin = D5;
const int servoPin = D0;

const char* ssid = "YOUR_WiFi_SSID";
const char* password = "YOUR_WiFi_PASSWORD";

const int lcdColumns = 16;
const int lcdRows = 2;

const float maxFeedDistance = 10;
const int maxLoading = 100;

Servo servo;
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);
LCD dispA(lcd, barStyle1);
SmoothProgressBar spb(dispA, 11, 0, 1, 1);

void setup() {
  //Setup Statment
  pinMode(trigPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  pinMode(echoPin, INPUT);
  // Optional for set servo to degree 0
  servo.write(0);
  servo.attach(servoPin);
  Serial.begin(9600);
  lcd.init();
  lcd.backlight(); 
  dispA.begin();
  // wifi connect
  delay(200);
  printSSIDPWD(lcd,ssid,password);
  delay(200);
  wifiConnect(ssid,password);

  delay(500);
  loading(spb,lcd,maxLoading);
  delay(500);

  // status wifi
  printStatusWiFi(lcd,WiFi.status());
  if (WiFi.status() == WL_CONNECTED) {
      Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password, "blynk.cloud", 80);
  }
}

void loop()
{ 
float percentage = percentageFeed(trigPin,echoPin,maxFeedDistance);
if (WiFi.status() == WL_CONNECTED) {
  modeON(percentage);
} else {
  modeOFF(percentage);
}
 
}

void modeOFF(float percentage) {
  if (digitalRead(buttonPin) == LOW ){
      servoOnOff(servo,lcd);
      return;
  } else {
      printPercentageFeed(spb,lcd,percentage);
  }
}

void modeON(float percentage) {
  Blynk.run();
  if (percentage < 15) {
    Blynk.logEvent("alert","Pakan Hampir Habis Mohon Diisi Ulang");
  } 
  Blynk.virtualWrite(V1,percentage);
  modeOFF(percentage);
}

BLYNK_WRITE(V0) {
  int value = param.asInt();
  if (value == 1) {
    servoOnOff(servo,lcd);
    Blynk.virtualWrite(V0,0);
    Blynk.logEvent("success","Pakan Telah Berhasil Diisi");
  } 
}
