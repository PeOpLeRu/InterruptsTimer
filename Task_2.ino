// Написать свою реализацию функции millis()

volatile uint32_t counter = 0;

uint32_t my_millis()
{
  return counter;
}

ISR(TIMER1_COMPA_vect)   // Прерывание на переполнение 1 таймера
{
    ++counter;
}

void setup()
{
    Serial.begin(9600);

    // Инициализация Таймера 1 (16 бит)
     TCCR1A = 0;
     TCCR1B = 0;

     OCR1A = 1999;    // Output Compare Register A

    // reference -> datasheet

    TIMSK1 |= 0x2; // Timer/Counter1, Output Compare A Match Interrupt Enable
    TCCR1B |= 0x2; // 8 prescaller for B channel;
    TCNT1 = 0;  //Timer/Counter 1 Counter Value
    TCCR1B |= 0x8;  // Clear Timer on Compare Match (CTC) mode
    
    SREG |= 0x80;   // Global Interrupt Enable
}

void loop()
{
    Serial.print("millis -> ");
    Serial.print(millis());
    Serial.print(" <---> my_millis -> ");
    Serial.print(my_millis());
    Serial.print(" <---> delta -> ");
    Serial.println(signed(my_millis() - millis()));
    delay(1000);
}