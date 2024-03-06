#include <FastLED.h>
#include "MUX74HC4067.h"

#define NUM_LEDS1 24
#define DATA_PIN1 2
#define NUM_LEDS2 18
#define DATA_PIN2 4
#define NUM_LEDS3 12
#define DATA_PIN3 5
#define NUM_LEDS4 6
#define DATA_PIN4 6
#define ringgroesse 6

CRGB leds1[NUM_LEDS1+NUM_LEDS2+NUM_LEDS3+NUM_LEDS4];

// Erstelle die Klasse MUX74HC4067
// 1tes Argument ist das ENable PIN
// 2-5 die Argumente für die Digitalen Ausgänge für Boolsche Tabelle S0-S3
MUX74HC4067 mux(7, 8, 9, 10, 11);

int pwmpin = 3;   //PWM Signal für Pumpe
int zaehler = 0;
int becherzahl = 0;
int merker = 0;
String id = "p2";
char c;

byte dataarray[]={0,0,0,0,0,0,0,0,0,0};

void setup()
{
  Serial.begin(9600);  // Initialisiere seriellen Port
    // wartet bis Initialisierung beendet
    while ( !Serial ) ;
  
  // Konfiguriere den Einlesepin an Arduino - PIN 12 - Eingang - Digital lesen
  mux.signalPin(12, INPUT, DIGITAL);
  
  FastLED.addLeds<NEOPIXEL, DATA_PIN1>(leds1, 0, NUM_LEDS1);
  FastLED.addLeds<NEOPIXEL, DATA_PIN2>(leds1, 24, NUM_LEDS2);
  FastLED.addLeds<NEOPIXEL, DATA_PIN3>(leds1, 42, NUM_LEDS3);
  FastLED.addLeds<NEOPIXEL, DATA_PIN4>(leds1, 54, NUM_LEDS4);
  
  pinMode(pwmpin, OUTPUT);
}


void loop()
{
  multiplexarlesen();
  becherzaehlen();
  
  if(becherzahl == 0){
      rainbow();
  }
  else{
       becherlicht();
  }
  
  read_input();
}

 
//**********************************FUNKTIONEN*****************************//

byte multiplexarlesen(){
    for(int j=0; j<10; j++){
    dataarray[j] = mux.read(j);
    }
}

void rainbow(){
    if(zaehler<255){
    fill_rainbow(leds1, (NUM_LEDS1+NUM_LEDS2+NUM_LEDS3+NUM_LEDS4), zaehler, 5);
    zaehler ++;
    FastLED.delay(30);  
  }
  else if(zaehler == 255){
    zaehler = 0;
  }
}

void becherzaehlen(){
  becherzahl = 0;
  for(int j=0; j<10; j++){
    if(dataarray[j] == LOW){
      becherzahl++;
    }
  }
}

void becherlicht(){
  
  for(int j=0; j<10; j++){
    if(dataarray[j] == LOW){
        for(int i = (j*6); i<(6+(j*6)); i++){
          leds1[i] = CHSV(96,255,255);
          //fill_rainbow(leds1&[i], 6, i, 5);
        }
      }
      
    else{
        for(int i = (j*6); i<(6+(j*6)); i++){
          leds1[i] = CHSV(0,255,80);
        }
      }
    }
  FastLED.show();
}

void send_score() {
  Serial.println(10-becherzahl);
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
    }
  }
}



//*************************************************************************//

  

