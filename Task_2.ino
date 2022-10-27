// Написать свою реализацию функции millis()

// Частота МК = 16'000'000
// Предделитель = 8
// Частота таймера = 16'000'000 / 8 = 2'000'000 (Тактов в секунду)
// Обновление счётчика требуется раз в 1ms
// Количество значений для счётчика = 65'536 <---> [0..65535]
// Прерывание срабатывает при сравнении счетика со сравнимым значением ->
// -> Сравнимое значение = частота таймера / кол-во нужных прерываний в секунду
// -> Сравнимое значение = 2'000'000 / 1000 (раз в 1 ms) = 2'000

volatile uint32_t counter = 0;

uint32_t my_millis()
{
  return counter;
}

ISR(TIMER1_COMPA_vect)   // Прерывание на сравнение 1 таймера со значением
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