#include <Adafruit_NeoPixel.h>

const int DIN_PIN = 2; // 制御するピン
const int LED_COUNT = 150; // LEDの数


const int pin = 4;//スイッチのピン
int value = 0;

Adafruit_NeoPixel pixels(LED_COUNT, DIN_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  pinMode(pin, INPUT);
  Serial.begin(9600);
  pixels.begin();
}

void loop() {
  value = digitalRead(pin);
  if(value == 0){
    pixels.clear();
    
    pixels.setPixelColor(0, pixels.Color(128, 0, 0)); // 0番目の色を変える
    pixels.setPixelColor(1, pixels.Color(0, 128, 0)); // 1番目の色を変える
    pixels.setPixelColor(2, pixels.Color(0, 0, 128)); // 1番目の色を変える
    pixels.show();
  }else{
    //何も点灯しない処理
    pixels.clear();
    pixels.setPixelColor(0, pixels.Color(0, 0, 0));
    pixels.show();  
  }
}
