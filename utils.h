#ifndef UTILS_H
#define UTILS_H
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <LiquidCrystal_I2C.h>  
#include <SmoothProgress.h>
#include <Servo.h>
void wifiConnect(const char* ssid, const char* password);
void loading(SmoothProgressBar spb,LiquidCrystal_I2C lcd,int maxLoading);
long duration(int echoPin) ;
float toCm(long duration) ;
float toInch(float cm) ;
float percentageFeed(int trigPin,int echoPin,float maxFeedDistance);
void servoOnOff(Servo servo,LiquidCrystal_I2C lcd);
void printSSIDPWD(LiquidCrystal_I2C lcd,const char* ssid, const char* password);
void printStatusWiFi(LiquidCrystal_I2C lcd,wl_status_t status);
const char* wlStatusToString(wl_status_t status);
void printPercentageFeed(SmoothProgressBar spb,LiquidCrystal_I2C lcd,float percentage);
#endif