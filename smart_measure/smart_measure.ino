#include "HX711.h"
#include <Adafruit_NeoPixel.h>

//LEDによる点灯に関する変数
const int LED_PIN = 7;     // 制御するピン
const int LED_COUNT = 150; // LEDの数

//DT,SCKは実際の配線に応じる
const int DT_PIN = 4;
const int SCK_PIN = 2;
const int RST_PIN = 6;
int RST_val;

//キャリブレーション用変数。ロードセルに依存
const int calibrate_dec = -685;

//イチゴの重量を測定に関する変数
float b_weight = -1.0;   //基準値_base
float p_weight = -1.0;   //前回値_primary
float n_weight = -1.0;   //現在値_now
float ichigo_weight = 0; //イチゴの重量
float error = 0.1;       //誤差をどれだけ許容するか

HX711 scale;
Adafruit_NeoPixel pixels(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
void setup()
{
    Serial.begin(9600);
    pinMode(RST_PIN, INPUT_PULLUP);
    scale.begin(DT_PIN, SCK_PIN);
    scale.set_scale(calibrate_dec);
    scale.tare();
    pixels.begin();

    pixels.clear();
    pixels.setPixelColor(0, pixels.Color(0, 0, 0));
    pixels.show();
}

void lit_LED(float ichigo_weight)
{
    pixels.clear();
    if (15 <= ichigo_weight && ichigo_weight <= 19)       //Lサイズの場合
        pixels.setPixelColor(0, pixels.Color(255, 0, 0)); // 0番目の色を赤に変える
    else if (12 <= ichigo_weight && ichigo_weight <= 15)  //Mサイズの場合
        pixels.setPixelColor(0, pixels.Color(0, 255, 0)); // 0番目の色を緑に変える
    else if (9 <= ichigo_weight && ichigo_weight < 12)    //Sサイズの場合
        pixels.setPixelColor(0, pixels.Color(0, 0, 255)); // 0番目の色を青に変える
    else if (5 <= ichigo_weight && ichigo_weight <= 9)
        pixels.setPixelColor(0, pixels.Color(255, 255, 0)); // 0番目の色を黄色に変える
    else if (ichigo_weight > 0)
        pixels.setPixelColor(0, pixels.Color(255, 255, 255)); // 0番目の色を白に変える
    else
        pixels.setPixelColor(0, pixels.Color(0, 0, 0));
    pixels.show();
}
void loop()
{
    RST_val = digitalRead(RST_PIN);
    n_weight = scale.get_units(10);
    if (RST_val == 0) //リセット時
    {
        Serial.println("reset!!");
        b_weight = n_weight;
        ichigo_weight = 0;

        pixels.clear();
        pixels.setPixelColor(0, pixels.Color(0, 0, 0));
        pixels.show();
    }
    else
    {
        n_weight = scale.get_units(10);
        if (abs(n_weight - p_weight) < error && abs(n_weight - b_weight) >= 5) /*現在値が前回値よりerror値よりも小さくなっていればイチゴが取られたと判定する*/
        {
            ichigo_weight = -(n_weight - b_weight); //イチゴの値を更新＊ただし，この値は数ループの間しか保持できない
            b_weight = n_weight;                    //基準値をリセット
            lit_LED(ichigo_weight);
        }
        p_weight = n_weight;

        /*
        改案
        ・itigo_weight の値を5g以上の値に絞ってリストで保存（0g付近の値は無視）
        ・itigo_weightの値を次のイチゴが取り除かれるまで保持　ichigo_weigtの中身が再び一定値以上なるまで値を保持するとか
       　どっちかできません？
        */

        /*デバッグ用
        Serial.println(n_weight);
        Serial.println(b_weight);
        Serial.println(p_weight);
        */
        Serial.println(n_weight - b_weight); //現在値と基準値の誤差を表示する
        Serial.print("    ichigo weight:  ");
        Serial.println(ichigo_weight);
        Serial.println();

        scale.power_down();
        delay(100);
        scale.power_up();
    }
}
