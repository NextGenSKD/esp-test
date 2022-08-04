int ledPin = 14;                 // Светодиод подключенный к вход/выходу 14
int time_latency = 1000;
int freq [1000];
bool input_mode = true;
int begin_time;
int val = 0;
int counter = 0;
int delta_time;
int old_val;
int real_time;
int rev_counter;
int time_jitter = 100;
int input_delay = 5000;

void led_blink(int t_latency)
{
  digitalWrite(ledPin, HIGH);   // включает светодиод
  delay(t_latency);             // ждет
  digitalWrite(ledPin, LOW);    // выключает светодиод
}


void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println("-----------");
  Serial.println("Begin blink");
  Serial.println("-----------");

  pinMode(ledPin, OUTPUT);      // устанавливает режим работы - выход
  int i = 3;
  while (i >= 1)
  {
    led_blink(500);
    delay(500);
    Serial.println(i);
    i--;


  }
  pinMode(ledPin, INPUT);      // устанавливает режим работы - вход
  input_mode = true;
  begin_time = millis();
  Serial.println("Setup complete");

}

void loop()
{


  if (input_mode)
  {
    pinMode(ledPin, INPUT);
    digitalWrite(ledPin, HIGH);
    old_val = val;
    val = digitalRead(ledPin);

    if (val != old_val)
    {
      real_time = millis();
      delta_time = real_time - begin_time;
      if (delta_time >= time_jitter) {
        freq[counter] = delta_time;
        //Serial.println(freq[counter]);
        //Serial.println(counter);
        counter++;

      }
      begin_time = real_time;
    } else {

      real_time = millis();
      delta_time = real_time - begin_time;
      //Serial.print(delta_time);
      if (delta_time >= input_delay && counter != 0) {
        input_mode = false;
        begin_time = real_time;
        //Serial.println("Replay ....");
        //Serial.print("Counter=");
        //Serial.println(counter);
        //Serial.print("DeltaTime=");
        //Serial.println(delta_time);
        rev_counter = counter;
        counter = 0;
      }

    }

  }
  else
  {
    pinMode(ledPin, OUTPUT);
    Serial.println("----------");
    Serial.print("Counter=");
    Serial.println(counter);
    while (rev_counter >= 0) {
      led_blink(freq[rev_counter]);
      Serial.print(freq[counter]);
      Serial.print("-ON ");
      rev_counter--;
      delay(freq[rev_counter]);
      Serial.print(freq[counter]);
      Serial.print("-OFF ");
      rev_counter--;
    }
    input_mode = true;
    counter = 0;

  }

}
