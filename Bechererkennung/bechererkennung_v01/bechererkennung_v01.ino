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

CRGB leds1[NUM_LEDS1];
CRGB leds2[NUM_LEDS2];
CRGB leds3[NUM_LEDS3];
CRGB leds4[NUM_LEDS4];


// Erstelle die Klasse MUX74HC4067
// 1tes Argument ist das ENable PIN
// 2-5 die Argumente für die Digitalen Ausgänge für Boolsche Tabelle S0-S3
MUX74HC4067 mux(7, 8, 9, 10, 11);

String id = "p1";
int pwmpin = 3;   //PWM Signal für Pumpe
uint8_t zaehler = 0;
uint8_t becherzahl = 0;
uint8_t merker = 0;
byte dataarray[]={0,0,0,0,0,0,0,0,0,0};
char c;

void setup()
{
  Serial.begin(9600);  // Initialisiere seriellen Port
    // wartet bis Initialisierung beendet
    while ( !Serial ) ;
  
  // Konfiguriere den Einlesepin an Arduino - PIN 12 - Eingang - Digital lesen
  mux.signalPin(12, INPUT, DIGITAL);
  
  FastLED.addLeds<NEOPIXEL, DATA_PIN1>(leds1, NUM_LEDS1);
  FastLED.addLeds<NEOPIXEL, DATA_PIN2>(leds2, NUM_LEDS2);
  FastLED.addLeds<NEOPIXEL, DATA_PIN3>(leds3, NUM_LEDS3);
  FastLED.addLeds<NEOPIXEL, DATA_PIN4>(leds4, NUM_LEDS4);
  pinMode(pwmpin, OUTPUT);

    for(uint8_t i = 0; i < 6; ++i){
    uint8_t leds = 24;
    for(uint8_t j = 0; j<leds; j++){

      leds1[j] = CRGB::Blue;
      leds2[j] = CRGB::Blue; 
      leds3[j] = CRGB::Blue; 
      leds4[j] = CRGB::Blue;
    }
  leds = leds - 6;  

  }
  FastLED.show();
}


void loop()
{
  multiplexarlesen();
  becherzaehlen();
  
  if(merker != becherzahl){   //Sende immer nur dann Anzahl der Becher, wenn eine Änderung stattgefunden hat
    Serial.write(becherzahl);
    Serial.println(becherzahl);
    merker = becherzahl;
  }
  
  if(becherzahl == 0){
      rainbow();
  }
  else if(becherzahl > 0){
    
    if(dataarray[0]==HIGH){
    //Serial.println("nicht bedeckt");
    leds1[0]=CRGB::Red;
    leds1[1]=CRGB::Red;
    leds1[2]=CRGB::Red;
    leds1[3]=CRGB::Red;
    leds1[4]=CRGB::Red;
    leds1[5]=CRGB::Red;
    }
    else if (dataarray[0]==LOW){
      //Serial.println("bedeckt");
      leds1[0]=CRGB::Blue;
      leds1[1]=CRGB::Blue;
      leds1[2]=CRGB::Blue;
      leds1[3]=CRGB::Blue;
      leds1[4]=CRGB::Blue;
      leds1[5]=CRGB::Blue;

    }
    if(dataarray[1]==HIGH){
    //Serial.println("nicht bedeckt");
    leds1[6]=CRGB::Red;
    leds1[7]=CRGB::Red;
    leds1[8]=CRGB::Red;
    leds1[9]=CRGB::Red;
    leds1[10]=CRGB::Red;
    leds1[11]=CRGB::Red;
    }
    else if (dataarray[1]==LOW){
      //Serial.println("bedeckt");
      leds1[6]=CRGB::Blue;
      leds1[7]=CRGB::Blue;
      leds1[8]=CRGB::Blue;
      leds1[9]=CRGB::Blue;
      leds1[10]=CRGB::Blue;
      leds1[11]=CRGB::Blue;

    }
    if(dataarray[2]==HIGH){
    //Serial.println("nicht bedeckt");
    leds1[12]=CRGB::Red;
    leds1[13]=CRGB::Red;
    leds1[14]=CRGB::Red;
    leds1[15]=CRGB::Red;
    leds1[16]=CRGB::Red;
    leds1[17]=CRGB::Red;
    }
    else if (dataarray[2]==LOW){
      //Serial.println("bedeckt");
      leds1[12]=CRGB::Blue;
      leds1[13]=CRGB::Blue;
      leds1[14]=CRGB::Blue;
      leds1[15]=CRGB::Blue;
      leds1[16]=CRGB::Blue;
      leds1[17]=CRGB::Blue;

    }
    if(dataarray[3]==HIGH){
    //Serial.println("nicht bedeckt");
    leds1[18]=CRGB::Red;
    leds1[19]=CRGB::Red;
    leds1[20]=CRGB::Red;
    leds1[21]=CRGB::Red;
    leds1[22]=CRGB::Red;
    leds1[23]=CRGB::Red;
    }
    else if (dataarray[3]==LOW){
      //Serial.println("bedeckt");
      leds1[18]=CRGB::Blue;
      leds1[19]=CRGB::Blue;
      leds1[20]=CRGB::Blue;
      leds1[21]=CRGB::Blue;
      leds1[22]=CRGB::Blue;
      leds1[23]=CRGB::Blue;
    }  

    if(dataarray[4]==HIGH){
    //Serial.println("nicht bedeckt");
    leds2[0]=CRGB::Red;
    leds2[1]=CRGB::Red;
    leds2[2]=CRGB::Red;
    leds2[3]=CRGB::Red;
    leds2[4]=CRGB::Red;
    leds2[5]=CRGB::Red;
    }
    else if (dataarray[4]==LOW){
      //Serial.println("bedeckt");
      leds2[0]=CRGB::Blue;
      leds2[1]=CRGB::Blue;
      leds2[2]=CRGB::Blue;
      leds2[3]=CRGB::Blue;
      leds2[4]=CRGB::Blue;
      leds2[5]=CRGB::Blue;
    }  

    if(dataarray[5]==HIGH){
    //Serial.println("nicht bedeckt");
    leds2[6]=CRGB::Red;
    leds2[7]=CRGB::Red;
    leds2[8]=CRGB::Red;
    leds2[9]=CRGB::Red;
    leds2[10]=CRGB::Red;
    leds2[11]=CRGB::Red;
    }
    else if (dataarray[5]==LOW){
      //Serial.println("bedeckt");
      leds2[6]=CRGB::Blue;
      leds2[7]=CRGB::Blue;
      leds2[8]=CRGB::Blue;
      leds2[9]=CRGB::Blue;
      leds2[10]=CRGB::Blue;
      leds2[11]=CRGB::Blue;
    }  

    if(dataarray[6]==HIGH){
    //Serial.println("nicht bedeckt");
    leds2[12]=CRGB::Red;
    leds2[13]=CRGB::Red;
    leds2[14]=CRGB::Red;
    leds2[15]=CRGB::Red;
    leds2[16]=CRGB::Red;
    leds2[17]=CRGB::Red;
    }
    else if (dataarray[6]==LOW){
      //Serial.println("bedeckt");
      leds2[12]=CRGB::Blue;
      leds2[13]=CRGB::Blue;
      leds2[14]=CRGB::Blue;
      leds2[15]=CRGB::Blue;
      leds2[16]=CRGB::Blue;
      leds2[17]=CRGB::Blue;
    }  

    if(dataarray[7]==HIGH){
    //Serial.println("nicht bedeckt");
    leds3[0]=CRGB::Red;
    leds3[1]=CRGB::Red;
    leds3[2]=CRGB::Red;
    leds3[3]=CRGB::Red;
    leds3[4]=CRGB::Red;
    leds3[5]=CRGB::Red;
    }
    else if (dataarray[7]==LOW){
      //Serial.println("bedeckt");
      leds3[0]=CRGB::Blue;
      leds3[1]=CRGB::Blue;
      leds3[2]=CRGB::Blue;
      leds3[3]=CRGB::Blue;
      leds3[4]=CRGB::Blue;
      leds3[5]=CRGB::Blue;
    }  

        if(dataarray[8]==HIGH){
    //Serial.println("nicht bedeckt");
    leds3[6]=CRGB::Red;
    leds3[7]=CRGB::Red;
    leds3[8]=CRGB::Red;
    leds3[9]=CRGB::Red;
    leds3[10]=CRGB::Red;
    leds3[11]=CRGB::Red;
    }
    else if (dataarray[8]==LOW){
      //Serial.println("bedeckt");
      leds3[6]=CRGB::Blue;
      leds3[7]=CRGB::Blue;
      leds3[8]=CRGB::Blue;
      leds3[9]=CRGB::Blue;
      leds3[10]=CRGB::Blue;
      leds3[11]=CRGB::Blue;
    }  
    if(dataarray[9]==HIGH){
    //Serial.println("nicht bedeckt");
    leds4[0]=CRGB::Red;
    leds4[1]=CRGB::Red;
    leds4[2]=CRGB::Red;
    leds4[3]=CRGB::Red;
    leds4[4]=CRGB::Red;
    leds4[5]=CRGB::Red;
    }
    else if (dataarray[9]==LOW){
      //Serial.println("bedeckt");
      leds4[0]=CRGB::Blue;
      leds4[1]=CRGB::Blue;
      leds4[2]=CRGB::Blue;
      leds4[3]=CRGB::Blue;
      leds4[4]=CRGB::Blue;
      leds4[5]=CRGB::Blue;
    }
  

  FastLED.delay(30);
  }
}
 
//**********************************FUNKTIONEN*****************************//

byte multiplexarlesen(){
    for(uint8_t j=0; j<10; j++){
    dataarray[j] = mux.read(j);
    }
}

void rainbow(){
    if(zaehler<255){
    fill_rainbow(leds1, NUM_LEDS1, zaehler, 5);
    fill_rainbow(leds2, NUM_LEDS2, zaehler, 5);
    fill_rainbow(leds3, NUM_LEDS3, zaehler, 5);
    fill_rainbow(leds4, NUM_LEDS4, zaehler, 5);
    zaehler ++;
    FastLED.delay(30);  
  }
  else if(zaehler == 255){
    zaehler = 0;
  }
}

void becherzaehlen(){
  becherzahl = 0;
  for(uint8_t j=0; j<10; j++){
    if(dataarray[j] == LOW){
      becherzahl++;
    }
  }
}

void send_id(){
  Serial.println(id);
}

void send_help(){
  Serial.println("here's help supposed to be");
}

void send_score(){
  Serial.println(10 - becherzahl);
}

void read_input(){
    if (Serial.available()){
    c = Serial.read();

    switch(c){
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

  

