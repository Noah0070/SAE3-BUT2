
void setup() {
  // Pin 9(OC1A) is output
  pinMode(9, OUTPUT);
  // Signal de sortie 30,5 Hz
  OCR1A= 128;
  // non-inverted fast PWM on OC1A with prescalar of 1
  TCCR1A = (1<<COM1A1) | (1<<WGM11);
  TCCR1B = (1<<WGM12) | (1<<CS10) | (1<<CS12);
}

void loop() {
  // Le code du programme principal peut être ajouté ici
}
