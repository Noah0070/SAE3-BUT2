#include <Wire.h>
#include <DFRobot_ADS1115.h>

DFRobot_ADS1115 ads(&Wire);
int16_t adc0, adc1, adc2, adc3;
int i = 1;

void setup(void) 
{
    Serial.begin(115200);
    ads.setAddr_ADS1115(ADS1115_IIC_ADDRESS1);   // 0x48
    ads.setGain(eGAIN_TWOTHIRDS);   // 2/3x gain
    ads.setMode(eMODE_SINGLE);       // single-shot mode
    ads.setRate(eRATE_128);          // 128SPS (default)
    ads.setOSMode(eOSMODE_SINGLE);   // Set to start a single-conversion
    ads.init();
}

void loop(void) 
{
    if (ads.checkADS1115())
    {
        adc0 = ads.readVoltage(0);
        Serial.print("A0:");
        Serial.print(adc0);
        Serial.print("mV,  ");
        adc1 = ads.readVoltage(1);
        Serial.print("A1:");
        Serial.print(adc1);
        Serial.print("mV,  ");
        adc2 = ads.readVoltage(2);
        //adc2 = readCAN();
        Serial.print("A2:");
        Serial.print(adc2);
        Serial.print("mV,  ");
        adc3 = ads.readVoltage(3);
        Serial.print("A3:");
        Serial.print(adc3);
        Serial.print("mV --> ");
        Serial.println(i);
    }
    else
    {
        Serial.println("ADS1115 Disconnected!");
    }
    i++;                   
    delay(500);
}

//int readCAN(void)
//{
//  bool first = true;
//  int j = 0;
//  adc2 = 0;
//
//  while (j < 50)  {
//    adc2 += ads.readVoltage(2);
//
//    if (first == true) {
//      first = false;
//    }
//    else {
//      adc2 /= 2;
//    }
//    j++;
//    delay(0.05);
//  }
//  return adc2;
//}
