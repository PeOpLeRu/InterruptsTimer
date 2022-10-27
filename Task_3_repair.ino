// Реализуйте три вида blink в одной программе. Blink по прерыванию работает всегда, остальные два режима переключаются по команде получаемой по UART (Serial)

// По команде "1" из COM порта переключаются Blink2 и Blink3

// Blink1 - Работает по прерыванию таймера 1 раз в 1 секунду (Мигает встроенным светодиодом)

// Blink2 и Blink3- Работает по прерыванию таймера 2

// Blink2 Работает по прерыванию на переполнеие счетчика
// Количество прерываний = 16'000'000 / 8 (предделитель) = 2'000'000 (Гц) -> 2'000'000 / 256 (количество значений счётчика) = 7 812,5‬ (раз срабатывает прерывание)

// Blink3 - Работает по прерыванию на сравнение счетчика со сравнимым значением
// Количество прерываний = 16'000'000 / 8 (предделитель) = 2'000'000 (Гц) -> 2'000'000 / 200 (сравнимое значение) = 10'000 (раз срабатывает прерывание)

uint8_t data = 0;   // временная переменная  для использования в прерывании (так как с регистра не получается читать и записывать в одной инструкции (строке))
volatile int counter = 0;   // Подсчет кол-ва прерывания Blink1 (раз в секунду)

int input = 0;  // Ввод по COM порту
int temp = 0;
volatile unsigned int counter_2 = 0;   // Подсчет кол-ва прерывания Blink2 or Blink3 (раз в секунду)

ISR(TIMER1_COMPA_vect)   // Прерывание на сравнение 2 таймера со значением
{
    ++counter;
    
    data = PORTB ^ 0x20;    // Инвертируем значение с пина D13 (6 бит)
    PORTB = data;   // digitalWrite(13, !digitalRead(13));
}

ISR(TIMER2_OVF_vect)   // Прерывание на переполнение 2 таймера
{
    ++counter_2;
}

ISR(TIMER2_COMPA_vect)   // Прерывание на сравнение 1 таймера со значением
{
    ++counter_2;
}

void setup()
{
    Serial.begin(9600);
    
    DDRB |= 1 << 5; // Пин D13 output

    // Инициализация Таймера 1 (16 бит)
     TCCR1A = 0;
     TCCR1B = 0;

     OCR1A = 62500;    // Output Compare Register A

    // reference -> datasheet
    TIMSK1 |= 0x02; //  Timer/Counter1, Output Compare A Match Interrupt Enable
    TCCR1B |= 0x04; // 256 prescaller for B channel;
    TCCR1B |= 0x8;  // Clear Timer on Compare Match (CTC) mode
    TCNT1 = 0;  //Timer/Counter 1 Counter Value

    // Инициализация Таймера 2
    TCCR2A = 0;
    TCCR2B = 0;

    OCR2A = 200;  // Output Compare Register A

    // reference -> datasheet -> page ~205-211
    TIMSK2 |= 0x01; //  Timer/Counter2, Overflow Interrupt Enable
    TCCR2B |= 0x02; // 8 prescaller for B channel;
    TCNT2 = 0;  //Timer/Counter 2 Counter Value

    SREG |= 0x80;   // Global Interrupt Enable
}

void loop()
{
    Serial.print(counter);
    Serial.print("<--->");
    Serial.println(counter_2);
    if (Serial.available())
    {
        input = Serial.parseInt();
        if (input == 1)
        {
            Serial.println("change");
            
            temp = TIMSK2 ^ 0x01; // Инвертирование состояния Timer/Counter2, Overflow Interrupt Enable
            TIMSK2 = temp;
            
            temp = TIMSK2 ^ 0x02; // Инвертирование состояния Timer/Counter2, Output Compare A Match Interrupt Enable
	    TIMSK2 = temp;
            
            TCCR2A ^= (1 << WGM21);  // Инвертирование состояния CTC режима
            
            counter_2 = 0;  // Для демонстрации изменения скорости счетчика (можно отключить)
        }
    }
    delay(1000);
}