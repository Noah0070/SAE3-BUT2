#include <Wire.h>
#include <DFRobot_ADS1115.h>

DFRobot_ADS1115 ads(&Wire);
int16_t adc0, adc1, adc2, adc3;
int i = 1;

int a0_line = 3400;
int a1_line = 4050;
int a2_line = 3900;

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


void readCAN() { // Fonction qui enregistre les valeurs du CAN dans les variables dédiées
  adc0 = ads.readVoltage(0);
  adc1 = ads.readVoltage(1);
  adc2 = ads.readVoltage(2);
  adc3 = ads.readVoltage(3);
}


void loop(void) 
{
    if (ads.checkADS1115())
    {   
        readCAN();
        
        Serial.print("A0:");
        Serial.print(adc0);
        Serial.print("mV,  ");
        
        Serial.print("A1:");
        Serial.print(adc1);
        Serial.print("mV,  ");
        
        Serial.print("A2:");
        Serial.print(adc2);
        Serial.print("mV,  ");
        
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

    if (detect_a0()) {
      Serial.println("a0 -> ligne noire !");                    
    }
    if (detect_a1()) {
      Serial.println("a1 -> ligne noire !");                    
    }
    if (detect_a2()) {
      Serial.println("a2 -> ligne noire !");                    
    }
    delay(500);
}



bool detect_a0(){
  if (adc0 < a0_line) {
    return true;
  }
  else {
    return false;
  }
}

bool detect_a1(){
  if (adc1 < a1_line) {
    return true;
  }
  else {
    return false;
  }
}

bool detect_a2(){
  if (adc2 < a2_line) { // Si la ligne est détectée
    return true;
  }
  else {
    return false;
  }
}
