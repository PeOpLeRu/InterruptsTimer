// Реализовать опрос аналогового пина раз в 250 ms (датчик можно не подключать)

unsigned int timer = 0;
volatile int value = 0;
volatile bool is_changed = false;

ISR(TIMER1_COMPA_vect)   // Прерывание на сравнение 1 таймера со значением
{
    value = analogRead(A0);
    is_changed = true;
}

void setup()
{
    Serial.begin(9600);

    DDRC &= ~(1 << 0);  // set input A0

    // Инициализация Таймера 1 (16 бит)
     TCCR1A = 0;
     TCCR1B = 0;

     OCR1A = 15625;    // Output Compare Register A

    // reference -> datasheet
    TIMSK1 |= 0x02; //  Timer/Counter1, Output Compare A Match Interrupt Enable
    TCCR1B |= 0x04; // 256 prescaller for B channel;
    TCCR1B |= 0x8;  // Clear Timer on Compare Match (CTC) mode
    TCNT1 = 0;  //Timer/Counter 1 Counter Value

    SREG |= 0x80;   // Global Interrupt Enable
}

void loop()
{
    if (is_changed)
    {
        Serial.print("Elapsed: ");
        Serial.print(millis() - timer);
        Serial.print(" <---> value: ");
        Serial.println(value);
        is_changed = false;
        timer = millis();        
    }
}