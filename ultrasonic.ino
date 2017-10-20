/*
   created by Boris Dusnoki, https://www.youtube.com/channel/UCgOEgQpu1thY7D-GzO5vdeQ
   Distance Sensor with graph and variable sound frequency and delay
*/
int tonePin = 8;    // buzzer pin
int trigPin = 9;    // sensor trigger pin
int echoPin = 10;   // sensor echo pin
int clockPin = 11;  // shift register clock pin
int latchPin = 12;  // shift register latch pin
int dataPin = 13;   // shift register data pin

unsigned long previousMillisLEDS = 0; // initialization of the previosMillis for LEDS
unsigned long previousMillisLOW = 0;  // initialization of the previosMillis for LOW distance buzzer
unsigned long previousMillisMID = 0;  // initialization of the previosMillis for MED distance buzzer
unsigned long previousMillisHIGH = 0; // initialization of the previosMillis for HIGH distance buzer

const int intervalLEDS = 100; // interval of refreshing the LEDS state
const int intervalLOW = 800;  // interval of the LOW distance buzzer
const int intervalMID = 400;  // interval of the MED distance buzzer
const int intervalHIGH = 100; // interval of the HIGH distance buzzer

const int freqLOW = 1500;  // frequency of the LOW distabce buzzer
const int freqMID = 1800;  // frequency of the MED distabce buzzer
const int freqHIGH = 2000; // frequency of the HIGH distabce buzzer

const int durLOW = 100;   // on time of the LOW distabce buzzer
const int durMED = 100;   // on time of the MED distabce buzzer
const int durHIGH = 80;   // on time of the HIGH distabce buzzer

const byte patterns[9] = {   // initialization of the patterns the LEDS are going to display
  B00000000,  // all LEDS OFF
  B00000001,  // 1 LED ON
  B00000011,  // 2 LEDS ON
  B00000111,  // 3 LEDS ON
  B00001111,  // 4 LEDS ON
  B00011111,  // 5 LEDS ON
  B00111111,  // 6 LEDS ON
  B01111111,  // 7 LEDS ON
  B11111111,  // 8 LEDS ON
};

int prox = 0; // initialization of the proximity value (0-8)
int dur;      // initialization of the duration between the Trigger and Echo signal of the sensor
int dist;     // initialization of the distance between the sensor and the object in front of it (in centimeters)

void setup() {
  pinMode(tonePin, OUTPUT);   // set tone pin to OUTPUT
  pinMode(trigPin, OUTPUT);   // set trigger pin to OUTPUT
  pinMode(echoPin, INPUT);    // set echo pin to INPUT
  pinMode(clockPin, OUTPUT);  // set clock pin to OUTPUT
  pinMode(latchPin, OUTPUT);  // set latch pin to OUTPUT
  pinMode(dataPin, OUTPUT);   // set data pin to OUTPUT
}

void loop() {

  unsigned long currentMillis = millis(); // set the currentMillis variable to the current number of milliseconds from the start of the loop

  if ((unsigned long)(currentMillis - previousMillisLEDS) >= intervalLEDS) {    // check if the time between the current time and previous time for LEDS is larger or equal to the interval the LEDS should stay on
    digitalWrite(latchPin, LOW);  // set the latch pin to LOW
    digitalWrite(trigPin, LOW);   // set the trigger pin to LOW
    delayMicroseconds(2);         // delay 2 microseconds
    digitalWrite(trigPin, HIGH);  // set the trigger pin to HIGH and send out a sound signal
    delayMicroseconds(100);       // delay 100 microseconds
    digitalWrite(trigPin, LOW);   // set the trigger pin to LOW
    dur = pulseIn(echoPin, HIGH); // caluclate the duration between the trigger and echo
    dist = dur / 2 / 29;          // calculate distance in centemeters based on the speed of sound

    prox = map(dist, 0, 48, 8, 0);    // map the distannce between 0 and 48 cm to a value between 0 and 8
    shiftOut(dataPin, clockPin, MSBFIRST, patterns[prox]);  // send the pattern to the shift register based on the prox value (0-8)
    digitalWrite(latchPin, HIGH);   // latch the shift register
    previousMillisLEDS = currentMillis;   // set the previousMillis for LEDS to the current time in milliseconds from the start of the loop
  }
  if (prox < 0) {   // if we get a proximity value less than 0 set it to 0
    prox = 0;
  }
  else if (prox == 6) {   // if we get a proximity value of 6
    if ((unsigned long)(currentMillis - previousMillisLOW) >= intervalLOW) {   // check if the time between the current time and previous time for LOW buzzer interal is larger or equal to the interval the buzzer should stay on
      tone(tonePin, freqLOW, 100);    // set the tone pin to the LOW frequency and an on time of 100 milliseconds
      previousMillisLOW = currentMillis; // set the previousMillis for LOW buzzer interal to the current time in milliseconds from the start of the loop
    }

  }
  else if (prox == 7) { // if we get a proximity value of 7
    if ((unsigned long)(currentMillis - previousMillisMID) >= intervalMID) {  // check if the time between the current time and previous time for MID buzzer interal is larger or equal to the interval the buzzer should stay on
      tone(tonePin, freqMID, 100);    // set the tone pin to the MID frequency and an on time of 100 milliseconds
      previousMillisMID = currentMillis;  // set the previousMillis for MID buzzer interal to the current time in milliseconds from the start of the loop
    }

  }
  else if (prox == 8) { // if we get a proximity value of 8
    if ((unsigned long)(currentMillis - previousMillisHIGH) >= intervalHIGH) {  // check if the time between the current time and previous time for HIGH buzzer interal is larger or equal to the interval the buzzer should stay on
      tone(tonePin, freqHIGH, 80);    // set the tone pin to the HIGH frequency and an on time of 80 milliseconds
      previousMillisHIGH = currentMillis;   // set the previousMillis for HIGH buzzer interal to the current time in milliseconds from the start of the loop
    }
  }
}
