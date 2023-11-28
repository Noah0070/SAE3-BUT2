#include <Wire.h>
#include <DFRobot_ADS1115.h>

#define bit_set(var, No_bit) ((var)|= (1<<No_bit)); // mise à 1
#define bit_clr(var, No_bit) ((var)&= ~(1<<No_bit)); // mise à 0

DFRobot_ADS1115 ads(&Wire);
int16_t adc0, adc1, adc2, adc3;
int i = 1;

//char sensors = 0x0;
int a0_line = 3400;
int a1_line = 4550;
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
        //checkSensors();
        
//        Serial.print("A0:");
//        Serial.print(adc0);
//        Serial.print("mV,  ");
//        
//        Serial.print("A1:");
//        Serial.print(adc1);
//        Serial.print("mV,  ");
//        
//        Serial.print("A2:");
//        Serial.print(adc2);
//        Serial.print("mV,  ");
//        
//        Serial.print("A3:");
//        Serial.print(adc3);
//        Serial.print("mV --> ");
//        Serial.println(i);
    }
    else
    {
        Serial.println("ADS1115 Disconnected!");
    }
    i++;

    char sensors = checkSensors();
    Serial.println(sensors);

    
    if ((sensors&(1<<0)) > 0) {
      Serial.println("a0 -> LIGNE NOIRE !");                    
    }
    else {
      Serial.println("a0 -> aucune ligne");
    }
    
    if ((sensors&(1<<1)) > 0) {
      Serial.println("a1 -> LIGNE NOIRE !");                    
    } else {
      Serial.println("a1 -> aucune ligne");
    }
    
    if ((sensors&(1<<2)) > 0) {
      Serial.println("a2 -> LIGNE NOIRE !");                    
    } else {
      Serial.println("a2 -> aucune ligne");
    }

    delay(500);
}



//bool detect_a0(){
//  if (adc0 < a0_line) {
//    bit_set(sensors, 0);
//  }
//  else { 
//    bit_clr(sensors, 0);
//  }
//}
//
//bool detect_a1(){
//  if (adc1 < a1_line) {
//    bit_set(sensors, 1);
//  }
//  else {
//    bit_clr(sensors, 1);
//  }
//}
//
//bool detect_a2(){
//  if (adc2 < a2_line) { // Si la ligne est détectée
//    bit_set(sensors, 2);
//  }
//  else {
//    bit_clr(sensors, 2);
//  }
//}

char checkSensors(void) {
  char sensors = 0x0;
  adc0 = ads.readVoltage(0);
  adc1 = ads.readVoltage(1);
  adc2 = ads.readVoltage(2);
  adc3 = ads.readVoltage(3);

  // -------------------- détection capteur 0 -------------------------
  if (adc0 < a0_line) {
    bit_set(sensors, 0);
  }
  else { 
    bit_clr(sensors, 0);
  }

  // -------------------- détection capteur 1 ---------------------------------
  if (adc1 < a1_line) {
    bit_set(sensors, 1);
  }
  else {
    bit_clr(sensors, 1);
  }

  // -------------------- détection capteur 2 ------------------------------------------
  if (adc2 < a2_line) { // Si la ligne est détectée
    bit_set(sensors, 2);
  }
  else {
    bit_clr(sensors, 2);
  }
  return sensors;
}
