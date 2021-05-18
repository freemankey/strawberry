#include "HX711.h"

//DT,SCKは実際の配線に応じる
const int DT_PIN = 8;
const int SCK_PIN = 9;
const int DIN_PIN = 7;
//キャリブレーション用変数。ロードセルに依存
const int calibrate_dec = -685;
float b_weight = -1.0; //基準値_base
float p_weight = -1.0; //前回値_primary
float n_weight = -1.0; //現在値_now
float ichigo_weight = 0;
float error = 0.1; //誤差をどれだけ許容するか
int btn;

HX711 scale;

void setup()
{
    Serial.begin(9600);
    pinMode(DIN_PIN, INPUT_PULLUP);
    scale.begin(DT_PIN, SCK_PIN);
    scale.set_scale(calibrate_dec);
    scale.tare();
}

void loop()
{
    btn = digitalRead(DIN_PIN);
    n_weight = scale.get_units(10);
    if (btn == 0) //リセット時
    {
        Serial.println("reset!!");
        b_weight = n_weight;
    }
    else
    {
        n_weight = scale.get_units(10);
        if (abs(n_weight - p_weight) < error && n_weight - b_weight >= 5) /*現在値が前回値よりerror値よりも小さくなっていればイチゴが取られたと判定する*/
        {
            ichigo_weight = n_weight - b_weight;
            　 //イチゴの値を更新＊ただし，この値は数ループの間しか保持できない
                b_weight = n_weight;
            　 //基準値をリセット
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
        Serial.println(n_weight - b_weight);
        　 //現在値と基準値の誤差を表示する
            Serial.print("     weight:  ");
        Serial.println(ichigo_weight); //

        scale.power_down();
        delay(100);
        scale.power_up();
    }
}
