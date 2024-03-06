#include <FastLED.h>
#include "MUX74HC4067.h"

#define NUM_leds 24
#define DATA_PIN1 2
#define NUM_LEDS2 18
#define DATA_PIN2 4
#define NUM_LEDS3 12
#define DATA_PIN3 5
#define NUM_LEDS4 6
#define DATA_PIN4 6
#define ringgroesse 6
#define BUTTON_PIN 7

CRGB leds[NUM_leds + NUM_LEDS2 + NUM_LEDS3 + NUM_LEDS4];

// Erstelle die Klasse MUX74HC4067
// 1tes Argument ist das ENable PIN
// 2-5 die Argumente für die Digitalen Ausgänge für Boolsche Tabelle S0-S3
MUX74HC4067 mux(7, 8, 9, 10, 11);

uint8_t pwmpin = 3;   //PWM Signal für Pumpe -> ungenutzt
uint8_t zaehler = 0;
uint8_t becherzahl = 0;
uint8_t merker = 0;
String id = "p2";
unsigned char c;

boolean pause = false;
boolean send_debug = false;
uint8_t buttonState;
uint8_t lastButtonState = LOW;
unsigned long lastDebounceTime = 0;

unsigned long currentmillis = 0;
unsigned long debounce = 400;
unsigned long buffertimer[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

byte dataarray[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
byte safearray[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
byte bufferarray[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

uint8_t random_color = 0;
uint8_t rhue = 0;

const uint8_t colors[4][6] =
{
  {96, 255, 255, 0, 255, 80},
  {160, 255, 255, 54, 255, 80},
  {172, 255, 255, 128, 255, 80},
  { 0, 0 , 255, 0, 0 , 80}
};



void setup()
{
  Serial.begin(9600);  // Initialisiere seriellen Port

  // Konfiguriere den Einlesepin an Arduino - PIN 12 - Eingang - Digital lesen
  // mux.signalPin(12, INPUT, DIGITAL);
  // Konfiguriere den Einlesepin an Arduino - PIN A0 - Eingang - Analog lesen 
  mux.signalPin(A0, INPUT, ANALOG);
  FastLED.addLeds<NEOPIXEL, DATA_PIN1>(leds, 0, NUM_leds);
  FastLED.addLeds<NEOPIXEL, DATA_PIN2>(leds, 24, NUM_LEDS2);
  FastLED.addLeds<NEOPIXEL, DATA_PIN3>(leds, 42, NUM_LEDS3);
  FastLED.addLeds<NEOPIXEL, DATA_PIN4>(leds, 54, NUM_LEDS4);

  pinMode(pwmpin, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
}


void loop()
{
  currentmillis = millis();
  multiplexarlesen();
  read_button();
  if(send_debug){send_debug_outout();}
  if(pause){
    pausenlicht();
    pausenbecherlicht();
    for (uint8_t j = 0; j < 10; j++) {
      safearray[j] = dataarray[j];
    }
  }
  else{
    for (uint8_t j = 0; j < 10; j++) {
    // LOW => BECHER 
    // HIGH => KEIN BECHER
  
      if ((safearray[j] == HIGH) & (dataarray[j] == LOW) & (bufferarray[j] == LOW)) {
        // nicht registrierter Becher wurde wieder abgestellt -> Aenderung anstoßen
        bufferarray[j] = HIGH;
        buffertimer[j] = currentmillis;
      }
  
      if((safearray[j] == HIGH) & (dataarray[j] == LOW) & (bufferarray[j] == HIGH)){
        // nicht registrierter Becher wurde erkannt und Aenderung angestoßen
        if (currentmillis - buffertimer[j] > debounce * 1 * 2/3) {
          //erste LED einschalten 
          leds[j * 6 + 5] = CHSV( colors[random_color][0], colors[random_color][1], colors[random_color][2]);
        }
        if (currentmillis - buffertimer[j] > debounce * 2 * 2/3) {
          //zweite LED ausschalten
          leds[j * 6 + 4] = CHSV( colors[random_color][0], colors[random_color][1], colors[random_color][2]);
        }
        if (currentmillis - buffertimer[j] > debounce * 3 * 2/3) {
          //dritte LED ausschalten
          leds[j * 6 + 3] = CHSV( colors[random_color][0], colors[random_color][1], colors[random_color][2]);
        }
        if (currentmillis - buffertimer[j] > debounce * 4 * 2/3) {
          //vierte LED ausschalten
          leds[j * 6 + 2] = CHSV( colors[random_color][0], colors[random_color][1], colors[random_color][2]);
        }
        if (currentmillis - buffertimer[j] > debounce * 5 * 2/3) {
          //fuenfte LED ausschalten
          leds[j * 6 + 1] = CHSV( colors[random_color][0], colors[random_color][1], colors[random_color][2]);
        }
        if (currentmillis - buffertimer[j] > debounce * 6 * 2/3) {
          //sechste LED ausschalten
          leds[j * 6 + 0] = CHSV( colors[random_color][0], colors[random_color][1], colors[random_color][2]);
          bufferarray[j] = LOW;
          safearray[j] = dataarray[j];
        }
      }
  
      if ((safearray[j] == LOW) & (dataarray[j] == HIGH) & (bufferarray[j] == LOW)) {
        // registrierter Becher wurde hochgehoben -> Aenderung soll starten
        bufferarray[j] = HIGH;
        buffertimer[j] = currentmillis;
      }
  
      if (safearray[j] == LOW & dataarray[j] == HIGH & bufferarray[j] == HIGH) {
        // registrierter Becher wurde hochgehoben und Aenderung laeuft
        if (currentmillis - buffertimer[j] > debounce * 1) {
          //erste LED ausschalten
          leds[j * 6 + 0] = CHSV( colors[random_color][3], colors[random_color][4], colors[random_color][5]);
        }
        if (currentmillis - buffertimer[j] > debounce * 2) {
          //zweite LED ausschalten
          leds[j * 6 + 1] = CHSV( colors[random_color][3], colors[random_color][4], colors[random_color][5]);
        }
        if (currentmillis - buffertimer[j] > debounce * 3) {
          //dritte LED ausschalten
          leds[j * 6 + 2] = CHSV( colors[random_color][3], colors[random_color][4], colors[random_color][5]);
        }
        if (currentmillis - buffertimer[j] > debounce * 4) {
          //vierte LED ausschalten
          leds[j * 6 + 3] = CHSV( colors[random_color][3], colors[random_color][4], colors[random_color][5]);
        }
        if (currentmillis - buffertimer[j] > debounce * 5) {
          //fuenfte LED ausschalten
          leds[j * 6 + 4] = CHSV( colors[random_color][3], colors[random_color][4], colors[random_color][5]);
        }
        if (currentmillis - buffertimer[j] > debounce * 6) {
          //sechste LED ausschalten
          leds[j * 6 + 5] = CHSV( colors[random_color][3], colors[random_color][4], colors[random_color][5]);
          bufferarray[j] = LOW;
          safearray[j] = dataarray[j];
          random_color = random8(4);
        }
      }
  
      if (safearray[j] == LOW & dataarray[j] == LOW) {
        // Becher steht drauf und ist registriert
        bufferarray[j] = LOW;
        buffertimer[j] = currentmillis;
        for (uint8_t i = (j * 6); i < (6 + (j * 6)); i++) {
          leds[i] = CHSV( colors[random_color][0], colors[random_color][1], colors[random_color][2]);
        }
      }
      if (safearray[j] == HIGH & dataarray[j] == HIGH) {
        // Becher steht nicht drauf und ist nicht registriert
        buffertimer[j] = currentmillis;
        for (uint8_t i = (j * 6); i < (6 + (j * 6)); i++) {
          leds[i] = CHSV( colors[random_color][3], colors[random_color][4], colors[random_color][5]);
        }
      }
    }
  becherzaehlen();
  }
  
  FastLED.show();
  read_input();
  EVERY_N_MILLISECONDS(20) {
    rhue ++;
  }
}


//**********************************FUNKTIONEN*****************************//

void multiplexarlesen() {
  for (int j = 0; j < 10; j++) {
    dataarray[j] = mux.read(j);
  }
}

void read_button(){
  uint8_t reading = digitalRead(BUTTON_PIN);

  if(reading != lastButtonState){
    lastDebounceTime = millis();
  }
  
  if((millis() - lastDebounceTime) > (debounce/8)){
    if(reading != buttonState){
      buttonState = reading;

      if (buttonState == HIGH){
        pause = !pause;
      } 
    }    
  }
  lastButtonState = reading;
}

void rainbow() {
  if (zaehler < 255) {
    fill_rainbow(leds, (NUM_leds + NUM_LEDS2 + NUM_LEDS3 + NUM_LEDS4), zaehler, 5);
    zaehler ++;
    FastLED.delay(30);
  }
  else if (zaehler == 255) {
    zaehler = 0;
  }
}

void becherzaehlen() {
  becherzahl = 0;
  for (int j = 0; j < 10; j++) {
    if (safearray[j] == LOW) {
      becherzahl++;
    }
  }
}

void becherlicht() {

  for (uint8_t j = 0; j < 10; j++) {
    if (dataarray[j] == LOW) {
      for (uint8_t i = (j * 6); i < (6 + (j * 6)); i++) {
        leds[i] = CHSV( colors[random_color][0], colors[random_color][1], colors[random_color][2]);
      }
    }

    else {
      for (int i = (j * 6); i < (6 + (j * 6)); i++) {
        leds[i] = CHSV( colors[random_color][3], colors[random_color][4], colors[random_color][5]);
      }
    }
  }
}

void pausenbecherlicht() {

  for (uint8_t j = 0; j < 10; j++) {
    if (dataarray[j] == LOW) {
      for (uint8_t i = (j * 6); i < (6 + (j * 6)); i++) {
        leds[i] = CHSV( colors[random_color][0], colors[random_color][1], colors[random_color][2]);
      }
    }
  }
}

void pausenlicht(){
  uint8_t bright = map8(sin8(rhue), 100, 200);  

  for (uint8_t j = 0; j < 10; j++){
    for (uint8_t i = (j*6); i < (6 + (j*6)); i++){
      leds[i] = CHSV( (rhue + ( j * 25 )), 255, bright);
    }
  }
}

void send_score() {
  Serial.println(becherzahl);
  // 'score' mit der Variable für die Becheranzahl ersetzen!
}

void send_help() {
  Serial.println("here's help supposed to be");
  // Platzhalter für helpfunction
}

void send_id() {
  Serial.println(id);
  //
}

void send_debug_outout(){
  Serial.print("Multiplexer Vals: ");
    for (int j = 0; j < 10; j++) {
      Serial.print(j);
      Serial.print(": ");
      Serial.print(mux.read(j));
      Serial.print("  ");
  }  
  Serial.println();
  delay(500);

  
}


void read_input() {
  // Liest das Kommando vom RPI aus und reagiert entsprechend
  if (Serial.available()) {
    c = Serial.read();
    switch (c) {
      case '?':
        send_score();
        break;
      case 'h':
        send_help();
        break;
      case 'i':
        send_id();
        break;
      case 'd':
        send_debug = true;
        break;
      case 's':
        send_debug = false;
        break;
    }
  }
}



//*************************************************************************//
