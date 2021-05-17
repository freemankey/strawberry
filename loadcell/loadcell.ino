#include "HX711.h"
 
//DT,SCKは実際の配線に応じる
const int DT_PIN = 8;
const int SCK_PIN = 9;
//キャリブレーション用変数。ロードセルに依存
const int calibrate_dec = -685;
HX711 scale;
 
void setup() {
Serial.begin(9600);
scale.begin(DT_PIN, SCK_PIN);
// scale.set_scale();の(calibrate_dec)内にロードセルに応じた値を入れる
// 使用したキッチンスケールだと-944がいい感じの値
scale.set_scale(calibrate_dec);
scale.tare();
}
 
void loop() {
// scale.get_units(10)の値を小数点以下2桁まで表示
//scale.get_units(10), 1にすれば1桁
Serial.println(scale.get_units(10), 2);
scale.power_down();
delay(50);
scale.power_up();
}
