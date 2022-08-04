int counter = 0;
int freq [1000];
int input_delay = 5000;     //default time no change to end input 5 sec.
uint8_t ledPin = 14;
bool input_mode = true;
class dig_read
{
  public:
    uint8_t ledPin = 14;
    int time_jitter = 100;      //default sensetivity 0,1 sec
    int time_begin = 0;
    int time_delta = 0;
    int time_real = 0;
    int val = 1;

    dig_read(uint8_t _ledPin) {
      this->ledPin = _ledPin;
    }
    dig_read(uint8_t _ledPin, int _time_jitter) {
      this->time_jitter = _time_jitter;
      this->ledPin = _ledPin;
    }

    bool begin() {
      this->time_real = millis();
      if (this->change_value()) {
        this->time_delta = this->time_real - this->time_begin;
        if (this->onClick()) {
          this->val = get_value();
          this->time_begin = this->time_real;
          return true;
        }

      }
      else       {
        this->time_delta = this->time_real - this->time_begin;

        // this->time_begin = this->time_real;
        if (end_input(5000)) {
          return false;
        }

      }
      if (end_input(5000)) {
        return false;}
    }
    bool end_input(int _delay) {
      return this->time_delta >= _delay;
    }
    bool onClick() {
      return this->time_delta >= this->time_jitter;
    }
    bool change_value() {
      return this->val != get_value();
    }
    int get_value() {
      return digitalRead(ledPin);
    }

  private:

};

dig_read *p14;

void setup() {

  p14 = new dig_read(14, 100);
  Serial.begin(115200);
  Serial.println();
  Serial.println("-----------");
  Serial.println("Begin blink");
  Serial.println("-----------");
  pinMode(ledPin, OUTPUT);      // устанавливает режим работы - выход
  int i = 3;
  while (i >= 1)
  {
    digitalWrite(ledPin, HIGH);   // включает светодиод
    delay(500);             // ждет
    digitalWrite(ledPin, LOW);
    delay(500);
    Serial.println(i);
    i--;
  }
  input_mode = true;
  Serial.println("Setup complete");
  Serial.println("----------");
  Serial.print("Waiting input.....");
}

void loop() {

  //input
  if (input_mode)
  {
    pinMode(ledPin, INPUT);
    digitalWrite(ledPin, HIGH);

    if (p14->begin()) {
      Serial.print("time_delta=");
      Serial.println(p14->time_delta);
      Serial.print("Counter=");
      Serial.println(counter);
      freq[counter++] = p14->time_delta;
      input_mode = !p14->end_input(input_delay);


    }

  }
  //output
  else
  {
    pinMode(ledPin, OUTPUT);
    Serial.println("----------");
    Serial.print("Playing.....");
    Serial.println(counter);

    while (counter >= 0) {
      digitalWrite(ledPin, HIGH);
      delay(counter--);
      Serial.print(freq[counter]);
      Serial.print("-ON ");
      digitalWrite(ledPin, LOW);

      delay(freq[counter--]);
      Serial.print(freq[counter]);
      Serial.print("-OFF ");
    }
    
    Serial.println("----------");
    Serial.print("Waiting input.....");
    input_mode = true;
    counter = 0;

  }





}
