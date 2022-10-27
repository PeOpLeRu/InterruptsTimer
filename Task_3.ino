// Реализуйте три вида blink в одной программе. Blink по прерыванию работает всегда, остальные два режима переключаются по команде получаемой по UART (Serial)

uint8_t data = 0;
volatile int counter = 0;
int input = 0;
int temp = 0;

ISR(TIMER1_COMPA_vect)   // Прерывание на сравнение 2 таймера со значением
{
    ++counter;
    
    data = PORTB ^ 0x20;    // Инвертируем значение с пина D13 (6 бит)
    PORTB = data;   // digitalWrite(13, !digitalRead(13));
}

void setup()
{
    Serial.begin(9600);
    
    DDRB |= 1 << 5; // Пин D13 output
    DDRB |= 1 << 4; // Пин D12 output
    DDRD |= 1 << 4; // Пин D4 output

    // Инициализация Таймера 1 (16 бит)
     TCCR1A = 0;
     TCCR1B = 0;

     OCR1A = 62500;    // Output Compare Register A

    // reference -> datasheet
    TIMSK1 |= 0x02; //  Timer/Counter1, Output Compare A Match Interrupt Enable
    TCCR1B |= 0x04; // 256 prescaller for B channel;
    TCCR1B |= 0x8;  // Clear Timer on Compare Match (CTC) mode
    TCNT1 = 0;  //Timer/Counter 1 Counter Value

    SREG |= 0x80;   // Global Interrupt Enable
}

void loop()
{
    Serial.println(counter);
    if (Serial.available())
    {
        input = Serial.parseInt();
        if (input == 1)
        {
            temp = PORTB ^ 0x10;    // Инвертируем значение с пина D12 (5 бит)
            PORTB = temp;   // digitalWrite(12, !digitalRead(12));
        }
        else if (input == 2)
        {
            temp = PORTD ^ 0x10;    // Инвертируем значение с пина D4 (5 бит)
            PORTD = temp;   // digitalWrite(4, !digitalRead(4));          
        }
    }
    delay(1000);
}