#include <AFMotor.h>
#include <Servo.h>
#include <DHT.h>


// Инициируем моторы
AF_DCMotor motor1(4);
AF_DCMotor motor2(3);
Servo servo1;

// Пины
#define TEMP_PIN 13 // номер пина, к которому подсоединен датчик температуры

// Таймеры


// Инициируем датчики
DHT dht(TEMP_PIN, DHT22);

// Инициируем переменные
float vcc = 0.0;  // Входящее напряжение
char controlChar;  // Символ контроля
long previousMillis = 0; // время, когда действие выполнялось последний раз
long previousScanMillis = 0;
long interval = 1000; // период ожидания следующего действия
long scanInterval = 50;
int pos = 0;
int scanStep = 1;
int sStep = 0;
bool scan = false;

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

void forward() {
  motor1.run(RELEASE);
  delay(20);
  motor1.run(FORWARD);
  Serial.println("Forward");
}

void backward() {
  motor1.run(RELEASE);
  delay(20);
  motor1.run(BACKWARD);
  Serial.println("Backard");
}

void left() {
  motor2.run(RELEASE);
  delay(20);
  motor2.run(BACKWARD);
  Serial.println("Left");
}

void right() {
  motor2.run(RELEASE);
  delay(20);
  motor2.run(FORWARD);
  Serial.println("Right");
}

void setup() {
  Serial.begin(9600);
  servo1.attach(10);
  dht.begin();

  // Задаем максимальную скорость вращения моторов (аналог работы PWM)
  motor1.setSpeed(255);
  motor1.run(RELEASE);
  motor2.setSpeed(255);
  motor2.run(RELEASE);
  servo1.write(0); //ставим вал под 0

}

void loop() {
  unsigned long currentMillis = millis(); // текущее время в миллисекундах

  if (currentMillis - previousMillis > interval) {
    // сохраняем последний момент, когда выполнялось действие
    previousMillis = currentMillis;

    vcc = readVCC();
    Serial.print("VCC: ");
    Serial.println(vcc);

    //Считываем влажность
    float h = dht.readHumidity();
    // Считываем температуру
    float t = dht.readTemperature();
    // Проверка удачно прошло ли считывание.
    if (isnan(h) || isnan(t)) {
      Serial.println("Не удается считать показания");
    } else {
      Serial.print("Humidity: ");
      Serial.print(h);
      Serial.print("%, Temperature: ");
      Serial.print(t);
      Serial.println("*C");
    }
  }

  if (currentMillis - previousScanMillis > scanInterval) {
    previousScanMillis = currentMillis;
    if (scan) {
      if (servo1.read() == 180) {
        sStep = 0 - scanStep;
      }
      if (servo1.read() == 0) {
        sStep = 0 + scanStep;
      }
      pos = pos + sStep;
      servo1.write(pos);
      Serial.println(servo1.read());
    }

  }

  if (Serial.available())
  {
    Serial.print("Command is ");
    controlChar = char(Serial.read());
    Serial.println(controlChar);
    servo1.write(controlChar);

    // При символе "1" едем вперед
    if (controlChar == '1')
    {
      forward();
    }
    // При символе "2" едем назад
    if (controlChar == '2')
    {
      backward();
    }
    // При символе "3" передние колеса направо
    if (controlChar == '3')
    {
      right();
    }
    // При символе "4" передние колеса налево
    if (controlChar == '4')
    {
      left();
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
    // При символе "6" сканируем
    if (controlChar == '6')
    {
      scan = true;
    }
    // При символе "7" отмена сканирования
    if (controlChar == '7')
    {
      scan = false;
      pos = 0;
      servo1.write(pos);
    }
  }
}




