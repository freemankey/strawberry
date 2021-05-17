#include "HX711.h"

//DT,SCKは実際の配線に応じる
const int DT_PIN = 8;
const int SCK_PIN = 9;
const int DIN_PIN = 7;
//キャリブレーション用変数。ロードセルに依存
const int calibrate_dec = -685;
float b_weight = -1.0;
float p_weight = -1.0;
float n_weight = -1.0;
float ichigo_weight = 0;
float error = 0.1;
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
        if (abs(n_weight - p_weight) < error)
        {
            ichigo_weight = n_weight - b_weight;
            b_weight = n_weight;
        }
        p_weight = n_weight;

        Serial.print(n_weight - b_weight);
        Serial.print("     weight:  ");
        Serial.println(ichigo_weight);

        scale.power_down();
        delay(100);
        scale.power_up();
    }
}
