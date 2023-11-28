//Arduino PWM Speed Control：
int E1 = 5;
int M1 = 4;
int E2 = 6;
int M2 = 7;

void setup()
{
    pinMode(9, OUTPUT);
    pinMode(10, OUTPUT);
    pinMode(10, OUTPUT);
    pinMode(M1, OUTPUT);
    pinMode(M2, OUTPUT);
    pinMode(E1, INPUT);
    pinMode(E2, INPUT);
        // Signal de sortie 30,5 Hz
    OCR1A= 105; // sert à régler le -alpha
    // non-inverted fast PWM on OC1A with prescalar of 1
    TCCR1A = (1<<COM1A0) | (1<<COM1A1) | (1<<WGM11) | (1<<WGM10); // Fast PWM inverting mode
    TCCR1B = (1<<WGM12) | (1<<CS12) | (1<<CS10); // Fast PMW 10 bits - prescaler max (1024)
}

void loop()
{
}
