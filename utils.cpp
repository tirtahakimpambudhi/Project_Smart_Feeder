#include "wl_definitions.h"
#include "utils.h"
//define sound velocity in cm/uS
#define SOUND_VELOCITY 0.034
#define CM_TO_INCH 0.393701

void wifiConnect(const char* ssid, const char* password) {
  Serial.println("Connecting to WiFi :");
  Serial.print(ssid);
  Serial.println("");
  
  WiFi.begin(ssid,password);
  if (WiFi.status() != WL_CONNECTED){
    for (int i = 0; i > 3; i++) {
      delay(500);
      Serial.print(".");
    }
  }
}

void loading(SmoothProgressBar spb,LiquidCrystal_I2C lcd,int maxLoading) {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Loading");
  for (int i = 0; i <= 100; i++) {
    lcd.setCursor(1, 0);  
    spb.showProgressPct(i);
    lcd.print(" ");
    lcd.print(i);                         // print the string on the display
    lcd.print('%');
    delay(25);
  }
}

long duration(int echoPin) {
  return pulseIn(echoPin, HIGH);
}

float toCm(long duration) {
  return duration * SOUND_VELOCITY/2;
}

float toInch(float cm) {
  return cm * CM_TO_INCH;
}

float percentageFeed(int trigPin,int echoPin,float maxFeedDistance) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long measuredDistance = pulseIn(echoPin, HIGH); 
  float distanceCm = toCm(measuredDistance);

  distanceCm = (distanceCm >= maxFeedDistance) ? maxFeedDistance : distanceCm;

  float result = ((distanceCm == maxFeedDistance) ? 100 : (1 - (distanceCm / maxFeedDistance)) * 100);

  return result;
}

void printPercentageFeed(SmoothProgressBar spb,LiquidCrystal_I2C lcd,float percentage) {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Pakan : ");
      lcd.print(percentage);
      lcd.print(" %");
      spb.showProgressPct(percentage);
      delay(1000);
}

void servoOnOff(Servo servo,LiquidCrystal_I2C lcd){
    for(int posDegrees = 0; posDegrees <= 180; posDegrees++) {
    servo.write(posDegrees);
    Serial.println(posDegrees);
    delay(10);
  }
  for(int posDegrees = 180; posDegrees >= 0; posDegrees--) {
    servo.write(posDegrees);
    Serial.println(posDegrees);
    delay(10);
  }
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Pakan");
  lcd.setCursor(0,1);
  lcd.print("Berhasil di isi");
  delay(2000);
} 

void printSSIDPWD(LiquidCrystal_I2C lcd,const char* ssid, const char* password){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("SSID");
    lcd.setCursor(0,1);
    lcd.print("PASSWORD");
    delay(3000);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(ssid);
    lcd.setCursor(0,1);
    lcd.print(password);
    delay(5000);
}

const char* wlStatusToString(wl_status_t status) {
  switch (status) {
    case WL_NO_SHIELD: return "Tanpa Perisai";
    case WL_IDLE_STATUS: return "Status Siaga";
    case WL_NO_SSID_AVAIL: return "SSID Tidak Tersedia";
    case WL_SCAN_COMPLETED: return "Pemindaian Selesai";
    case WL_CONNECTED: return "Terhubung";
    case WL_CONNECT_FAILED: return "Gagal Terhubung";
    case WL_CONNECTION_LOST: return "Koneksi Terputus";
    case WL_DISCONNECTED: return "Terputus";
    case WL_WRONG_PASSWORD: return "Kata Sandi Salah";
    default: return "Status Tidak Dikenal"; 
  }
}

void printStatusWiFi(LiquidCrystal_I2C lcd,wl_status_t status) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Status WiFi");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  if (status == WL_CONNECTED) {
    lcd.print("Mode Online");
  } else {
    lcd.print("Mode Offline");
  }
  lcd.setCursor(0, 1);
  lcd.print(wlStatusToString(status));
  delay(5000);
}

