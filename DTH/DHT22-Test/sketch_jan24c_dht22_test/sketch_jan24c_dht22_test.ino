#include <DHT.h>
//#include <DHT_U.h>
//#include <Wire.h>

// DHT датчик
uint8_t DHTPin = D5;
#define DHTTYPE DHT22

// Инициализация датчика DHT
DHT dht(DHTPin, DHTTYPE);
float Temperature;
float Humidity;
float xTemperature;
float xHumidity;

unsigned long globaltime;   //время глобальное



void dots(int countermax) {
  int counter = 0;
  while (counter < countermax)  {
    Serial.print(".");
    counter++;
    delay(500);
  }
}

void xdots() {
  Serial.print(".");
}


class delayer {
  protected:
    unsigned long _timestmap;
    uint16_t _delay;

  public:
    delayer(uint16_t delay) {
      this->_delay = delay;
      this->_timestmap = delay + millis();
    }
    void begin(unsigned long timestmap, void (*ptrFunc)() ) {
      if (timestmap > this->_timestmap) {
        this->_timestmap += this->_delay;
        (*ptrFunc)();
      }
    }
};

delayer printdots(1000);
delayer getsensors(5000);
delayer printsensors(5000);

void sensorsread () {

  xTemperature = Temperature;
  xHumidity = Humidity;
  Temperature = dht.readTemperature(); // Получает значения температуры
  Humidity = dht.readHumidity(); // Получает значения влажности

}

void sensorsprint () {
  Serial.println();
  if (xTemperature != Temperature) {
    float deltatemperature = Temperature - xTemperature;
    Serial.print("Temperature: ");
    Serial.print(Temperature);
    Serial.print(" Delta: ");
    Serial.println(deltatemperature);
  }
  if (xHumidity != Humidity) {
    float deltahumidity = Humidity - xHumidity;
    Serial.print("Humidity: ");
    Serial.print(Humidity);
    Serial.print(" Delta: ");
    Serial.println(deltahumidity);
  }
}

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  delay(100);
    pinMode(DHTPin, INPUT);
  dht.begin();
  Serial.println();
  sensorsread ();
  dots(5);
  Serial.print("Первоначальный запуск:");
  dots(5);
  Serial.println("OK");

}

void loop() {
  // put your main code here, to run repeatedly:

  globaltime = millis();
  getsensors.begin (globaltime, &sensorsread);
  printsensors.begin (globaltime, &sensorsprint);
  printdots.begin (globaltime, &xdots);
}
