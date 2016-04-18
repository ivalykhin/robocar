#include <AFMotor.h>

// Инициируем моторы
AF_DCMotor motor1(4);
AF_DCMotor motor2(3);

// Пины

// Инициируем датчики

// Инициируем переменные
float vcc = 0.0;
char controlChar;

void setup() {
  Serial.begin(9600);
// Задаем максимальную скорость вращения моторов (аналог работы PWM) 
  motor1.setSpeed(255);
  motor1.run(RELEASE);
  motor2.setSpeed(255);
  motor2.run(RELEASE);
}

void loop() {
  //vcc = readVCC();
  //Serial.print("VCC: ");
  //Serial.println(vcc);
  if (Serial.available())
  {
    Serial.print("Command is ");
    controlChar = char(Serial.read());
    Serial.println(controlChar);
    
    // При символе "1" едем вперед
    if (controlChar == '1')
    { 
      motor1.run(RELEASE);
      delay(20);
      motor1.run(FORWARD);
      Serial.println("Forward");
    }
    // При символе "2" едем назад
    if (controlChar == '2')
    {
      motor1.run(RELEASE);
      delay(20);
      motor1.run(BACKWARD);
      Serial.println("Backard");
    }
    // При символе "3" передние колеса направо
    if (controlChar == '3')
    {
      motor2.run(RELEASE);
      delay(20);
      motor2.run(FORWARD); 
      Serial.println("Right");
    }
    // При символе "4" передние колеса налево
    if (controlChar == '4')
    {
      motor2.run(RELEASE);
      delay(20);
      motor2.run(BACKWARD); 
      Serial.println("Left");
    }
    // При символе "0" стоим на месте
    if (controlChar == '0')
    {
       motor1.run(RELEASE); 
       Serial.println("Stop");
    }
    // При символе "5" колеса прямо
    if (controlChar == '5')
    {
       motor2.run(RELEASE); 
       Serial.println("Streigt");
    }
  }
}

float readVCC() {
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  ADCSRA |= _BV(ADSC); // начало преобразований
  while (bit_is_set(ADCSRA, ADSC)); // измерение
  uint8_t low = ADCL; // сначала нужно прочесть ADCL - это запирает ADCH
  uint8_t high = ADCH; // разлочить оба
  float result = (high << 8) | low;
  result = (1.1 * 1023.0) / result; // Результат Vcc в милливольтах
  return result;
}


