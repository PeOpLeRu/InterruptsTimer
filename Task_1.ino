// Реализовать blink по прерываниям на 8 битном таймере с максимальным периодом

// Частота МК = 16'000'000
// Предделитель = 1024
// Частота таймера = 16'000'000 / 1024 = 15'625 (Тактов в секунду)
// Количество значений для счётчика = 256 <---> [0..255]
// Прерывание срабатывает при переполнении счетика ->
// -> Количество прерываний в секунду = Колв-во обновлений счетчика (частота таймера) / количество значений для счётчика
// Количество прерываний в секунду = 15'625 / 256 = 61 прерывание в секунду

uint8_t data = 0;
volatile int counter = 0;

ISR(TIMER2_OVF_vect)   // Прерывание на переполнение 2 таймера
{
    TCCR2B = 0; // Stop timer/counter
    
    ++counter;
    
    data = PORTB ^ 0x20;    // Инвертируем значение с пина D13 (6 бит)
    PORTB = data;   // digitalWrite(13, !digitalRead(13));
    
    TCCR2B |= 0x07; // 1024 prescaller for B channel;
}

void setup()
{
    Serial.begin(9600);
    
    DDRB |= 1 << 5; // Пин D13 output

    // Инициализация Таймера 2
     TCCR2A = 0;
     TCCR2B = 0;

    // reference -> datasheet -> page ~205-211
    TIMSK2 |= 0x01; //  Timer/Counter2, Overflow Interrupt Enable
    TCCR2B |= 0x07; // 1024 prescaller for B channel;
    TCNT2 = 0;  //Timer/Counter 2 Counter Value
    SREG |= 0x80;   // Global Interrupt Enable
}

void loop()
{
    Serial.println(counter);
    delay(1000);
}