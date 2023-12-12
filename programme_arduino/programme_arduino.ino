#include <Wire.h>
#include <DFRobot_ADS1115.h>

#define bit_set(var, No_bit) ((var)|= (1<<No_bit)); // mise à 1
#define bit_clr(var, No_bit) ((var)&= ~(1<<No_bit)); // mise à 0

DFRobot_ADS1115 ads(&Wire);
int16_t adc0, adc1, adc2, adc3;
int i = 1;
char sensors;

int E1 = 5;
int M1 = 4;
int E2 = 6;
int M2 = 7;

// ----------- REGLAGES SENSIBILITE CAPTEURS
// VALEURS PLAQUE NON FIXEE
//int a0_line = 3470;
//int a1_line = 3850;
//int a2_line = 4100;
//int a3_line = 4250;

// VALEURS PLAQUE FIXEE
int a0_line = 3700;
int a1_line = 3700;
int a2_line = 4000;
int a3_line = 3850;

void setup(void) 
{   
    // ----------------- SETUP CAN -----------------------------------
    Serial.begin(115200);
    ads.setAddr_ADS1115(ADS1115_IIC_ADDRESS1);   // 0x48
    ads.setGain(eGAIN_TWOTHIRDS);   // 2/3x gain
    ads.setMode(eMODE_SINGLE);       // single-shot mode
    ads.setRate(eRATE_128);          // 128SPS (default)
    ads.setOSMode(eOSMODE_SINGLE);   // Set to start a single-conversion
    ads.init();

    // ----------------- SETUP PARTIE PUISSANCE ROBOT ----------------------------
    pinMode(9, OUTPUT);
    pinMode(10, OUTPUT);
    pinMode(M1, OUTPUT);
    pinMode(M2, OUTPUT);
    pinMode(E1, INPUT);
    pinMode(E2, INPUT);
    pinMode(13, OUTPUT);
        // Signal de periode 65ms
    OCR1A= 200;
    //ICR1 = 200;
    OCR1B= 255; 
    // non-inverted fast PWM on OC1A with prescalar of 1
    TCCR1A = (1<<COM1A0) | (1<<COM1A1) | (1<<COM1B0) | (1<<COM1B1) | (1<<WGM11) | (1<<WGM10); // Fast PWM inverting mode -- On paramètre COM1A:B pour gérer les sorties
    TCCR1B = (1<<WGM12) | (1<<CS12) | (1<<CS10); // Fast PMW 10 bits - prescaler max (1024)
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

    sensors = checkSensors();
    //Serial.println(i);

    
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

    if ((sensors&(1<<3)) > 0) {
      Serial.println("a3 -> LIGNE NOIRE !");                    
    } else {
      Serial.println("a3 -> aucune ligne");
    }

    Serial.println(" ");
    select_direction();
    delay(500);
}



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
  // -------------------- détection capteur 3 ------------------------------------------
  if (adc3 < a3_line) { // Si la ligne est détectée
    bit_set(sensors, 3);
  }
  else {
    bit_clr(sensors, 3);
  }
  return sensors;
}


// ----------------- DETECTIONS DE LIGNE --------------------------------------------
bool line_on_middle() {
    if ((sensors&(1<<1)) > 0 || (sensors&(1<<2)) > 0) { // Si la ligne est détectée sur l'un des capteurs du milieu
      return true;                    
    } 
    else {
      return false;
    }
}

bool line_on_right() {
  if (!(line_on_middle()) && ((sensors&(1<<0)) > 0)) { // Si la ligne n'est pas détectée au milieu mais uniquement à droite
    return true;
  }
  else {
    return false;
  }
}

bool line_on_left() {
  if (!(line_on_middle()) && ((sensors&(1<<3)) > 0)) {
    return true;
  }
  else {
    return false;
  }
}


// ----------------- DIRECTION DU ROBOT -------------------------------------
void go_forward(void) { // le robot avance tout droit (les deux moteurs fonctionnent)
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
}

void turn_right(void) { // le robot s'arrête et tourne sur lui même vers la droite (moteur gauche désactivé)
  pinMode(9, OUTPUT);
  pinMode(10, INPUT);
}

void turn_left(void) { // le robot s'arrête et tourne sur lui même vers la gauche (moteur droit désactivé)
  pinMode(9, INPUT);
  pinMode(10, OUTPUT);
}

void select_direction(void) {
  if (line_on_middle() || sensors == 0) {
    go_forward();
  }
  else if (line_on_right()) {
    turn_right();
  }
  else if (line_on_left()) {
    turn_left();
  }
}
