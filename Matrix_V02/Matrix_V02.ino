#include <FastLED.h>

#include <LEDMatrix.h>

#include <LEDText.h>

#include <FontMatrise.h>

#include <FontHollow.h>

#include "numbers.h"

// Change the next 6 defines to match your matrix type and size

#define LED_PIN        2
#define COLOR_ORDER    GRB
#define CHIPSET        WS2812B

#define MATRIX_WIDTH   23  // Set this negative if physical led 0 is opposite to where you want logical 0
#define MATRIX_HEIGHT  -11  // Set this negative if physical led 0 is opposite to where you want logical 0
#define MATRIX_TYPE    HORIZONTAL_ZIGZAG_MATRIX  // See top of LEDMatrix.h for matrix wiring types

#define FRAMES_PER_SECOND 60

cLEDMatrix<MATRIX_WIDTH, MATRIX_HEIGHT, MATRIX_TYPE> leds;

cLEDText ScrollingMsg;


// Variables to controll LEDs and Programsteps

uint8_t mode = 0;
// 0 -> IDLE | 1 -> STARTUP | 2 -> GAME | 3 -> BREAK | 4 -> GAME END

unsigned long currentmillis = 0;
unsigned long modetimer = 0;

uint8_t angle = 0;
uint8_t rhue = 0;
uint8_t counter = 0;

String cmd = "";
String p1name = "PLAYER1";
String p2name = "PLAYER2";


uint8_t p1score = 0;
uint8_t p1prevscore = 0;
uint8_t p1series = 0;

uint8_t p2prevscore = 0;
uint8_t p2score = 0;
uint8_t p2series = 0;


// TextDemo for debugging

char TxtDemo[30];

void setup()
{
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds[0], leds.Size());
  FastLED.clear(true);
  Serial.begin(38400);

  ScrollingMsg.SetFont(FontHollowData);
  ScrollingMsg.Init(&leds, leds.Width(), ScrollingMsg.FontHeight() + 3, -1, 0); // Matrix, Breite, Hoehe, xOffset, yOffset
  ScrollingMsg.SetText((unsigned char *)TxtDemo, sizeof(TxtDemo) - 1); // laed text in Buffer
  ScrollingMsg.SetTextColrOptions(COLR_RGB | COLR_SINGLE, 0x00, 0x00, 0xff); // setzt Farbe der Schrift
  ScrollingMsg.SetFrameRate(10);

}


void loop()
{
  currentmillis = millis();
  read_input();

  switch (mode) {
    case 0:
      idle();
      break;

    case 1:
      startup();
      break;

    case 2:
      beerpong();
      break;

    case 3:
      breaking();
      break;

    case 4:
      game_end();
      break;
  }

  FastLED.show();

  EVERY_N_MILLISECONDS(20) {
    rhue ++;
  }

  if (millis() < (currentmillis + ( 1000 / FRAMES_PER_SECOND))) {
    delay(( currentmillis + ( 1000 / FRAMES_PER_SECOND)) - millis());
  }
}

void idle() {
  uint8_t h = sin8(angle);
  leds.ShiftLeft();
  for (int16_t y = leds.Height() - 1; y >= 0; --y) {
    leds(leds.Width() - 1, y) = CHSV(h, 255, 180);
    h += 32;
  }
  angle += 4;
}

void startup() {
  if (modetimer == 0) {
    modetimer = currentmillis;
    counter = 0;
    String emptyText = "                              ";
    emptyText.toCharArray(TxtDemo, 30);
    String playernames = "  " + p1name + " VS " + p2name;
    playernames.toCharArray(TxtDemo, 30);
  }
  leds.DrawFilledRectangle(0, 11, 23, 0, CRGB::Black);
  ScrollingMsg.UpdateText();

  if ((ScrollingMsg.UpdateText () == -1) & (counter < 2)) {
    ScrollingMsg.SetText((unsigned char*)TxtDemo, sizeof(TxtDemo) - 1);
    counter ++;
  }
  else if ((ScrollingMsg.UpdateText () == -1) & (counter == 2)) {
    mode = 2;
    modetimer = 0;
  }

  if ((currentmillis - modetimer > 60000) & (currentmillis - modetimer < 10000000)) { // BEENDET STARTUP NACH 20 SEC
    // Notbremse nach einer Minute
    mode = 2;
    modetimer = 0;
  }

}

void beerpong() {
  if (modetimer == 0) {
    modetimer = currentmillis;
    FastLED.clear(true);
  }

  leds.DrawRectangle(0, 10, 22, 0, CHSV(rhue + 127, 255, 255));

  if (p1score != p1prevscore) {
    p1series ++;
    p2series = 0;
    modetimer = currentmillis;
    p1prevscore = p1score;
    // ANIMATION EINBAUEN
  }

  if (p2score != p2prevscore) {
    p2series ++;
    p1series = 0;
    modetimer = currentmillis;
    p2prevscore = p2score;
    // ANIMATION EINBAUEN
  }

  draw_score(p1score, p2score, rhue);
}

void breaking() {
  uint8_t sat = sin8(angle);
  leds.DrawFilledRectangle(0, 11, 23, 0, CHSV(0, sat, 255));
  angle += 4;
}

void game_end() {
  unsigned int flashtimer = 1000;
  uint8_t maxblinks = 20;

  if (modetimer == 0) {
    counter = 0;
  }

  if ((currentmillis - modetimer) <= flashtimer) {
    leds.DrawFilledRectangle(0, 11, 23, 0, CRGB::Black);
  }
  else if ((currentmillis - modetimer) > flashtimer) {
    draw_score(p1score, p2score, 127);
  }

  if ((currentmillis - modetimer) > 2 * flashtimer) {
    modetimer = currentmillis;
    leds.DrawFilledRectangle(0, 11, 23, 0, CRGB::Black);
    counter ++;
  }

  if (counter > maxblinks) {
    mode = 0;
  }

}

void read_input() {
  // reads serial signals and calls functions
  while (Serial.available()) {


    char nextChar;
    if (Serial.available() > 0)
    {
      nextChar = Serial.read();
      Serial.print( "Zeichen: '" );
      Serial.print( nextChar );
      Serial.print( "'\n" );

      if ( nextChar == ';' ) {
        Serial.print( "Kommando vollstaendig '" );
        Serial.print( cmd );
        Serial.print( "'\n" );

        if ( cmd == "start" )
          mode = 1;
        else if ( cmd == "terminate" )
          mode = 0;
        else if ( cmd == "help")
          send_help();
        else if ( cmd == "id")
          send_id();
        else if ( cmd == "faststart")
          mode = 2;
        else if ( cmd == "end") {
          mode = 4;
          modetimer = 0;
        }


        cmd = "";
      }

      else if (nextChar == '$') {
        Serial.print( "Player 1 Name erkannt: ");
        Serial.print(cmd);
        Serial.print("\n");
        p1name = cmd;
        Serial.println(p1name);
        cmd = "";
      }

      else if (nextChar == '%') {
        Serial.print( "Player 2 Name erkannt: ");
        Serial.print(cmd);
        Serial.print("\n");
        p2name = cmd;
        Serial.println(p2name);
        cmd = "";
      }

      else if ( nextChar == ':') {
        Serial.print( "Score p1 erkannt: ");
        Serial.print(cmd);
        Serial.print("\n");
        p1score = cmd.toInt();
        Serial.print("Player 1 score: ");
        Serial.println(p1score);
        cmd = "";
      }

      else if ( nextChar == '#') {
        Serial.print( "Score p2 erkannt: ");
        Serial.print(cmd);
        Serial.print("\n");
        p2score = cmd.toInt();
        Serial.print("Player 2 score: ");
        Serial.println(p2score);
        cmd = "";
      }

      else if (nextChar >= 32) {
        cmd += nextChar;
        Serial.print( "Kommando bis jetzt " );
        Serial.print( cmd );
        Serial.print( "\n" );
      }
    }
  }
}

void send_id() {
  // sends ID for initialization
  Serial.println("ma");
}

void send_help() {
  // sends help file
  Serial.println("\n\nThis is the LED Matrix Help Page\n\n");
  Serial.println("Things you can do:\n");
  Serial.println("to set Player 1 Name use: 'NAME$' (Dollar Sign) ");
  Serial.println("to set Player 2 Name use: 'NAME%' (Percentage Sign)");
  Serial.println("to set the Score use: 'NUMBER:NUMER#'\n");
  Serial.println("you can use these commandos followed by ';' (Semicolon):");
  Serial.println("'help;' well... obviously.");
  Serial.println("'start;' start the usual beerpong game. Make sure Names have been set!");
  Serial.println("'faststart;' skipps the startup. For debugging");
  Serial.println("'terminate;' end the running game. In case you forgot to set names...");
  Serial.println("'end;' starts the end animation. Call after game ended.");
  Serial.println("\n\nEnd of Help Page");
}

void draw_score(uint8_t score_p1, uint8_t score_p2, uint8_t hue) {
  // loops over coloums and rows and sets matrix pixel to score from scores.h
  uint8_t p1xOffset = 6;
  uint8_t p1yOffset = 2;
  uint8_t p2xOffset = 16;
  uint8_t p2yOffset = 2;

  if (score_p1 == 10) p1xOffset --;
  if (score_p2 == 10) p2xOffset ++;

  leds.DrawFilledRectangle(3, 7, 18, 3, CRGB::Black);

  // SCORE P1:
  for (uint8_t hp = 5; hp > 0; hp--)
    // go through rows; number depending on hp 'height of number'
  {
    for (uint8_t wp = 0; wp < 5; wp++)
      // run through coloums and compare the bitvalue of
      // the pattern with Index 'patternNumber' at pos 'wp' bit with true. If true, set LED ON
      // wp stands for 'width of pattern'
    {
      bool bit = ((numbers[score_p1][hp-1]) >> wp) & 1;
      if (bit) {
        leds(p1xOffset + (2 - wp), p1yOffset + hp) = CHSV(hue, 255, 255);
      }
      // else not set
    }
  }
  // SCORE P2:
  for (uint8_t hp = 5; hp > 0; hp--)
    // go through rows; number depending on hp 'height of pattern'
  {
    for (uint8_t wp = 0; wp < 5; wp++)
      // run through coloums and compare the bitvalue of
      // the pattern with Index 'patternNumber' at pos 'wp' bit with true. If true, set LED ON
      // wp stands for 'width of pattern'
    {
      bool bit = ((numbers[score_p2][hp-1]) >> wp) & 1;
      if (bit) {
        leds(p2xOffset + (2 - wp), p2yOffset + hp) = CHSV(hue, 255, 255);
      }
      // else not set
    }
  }

  leds(11, 4) = CHSV(hue, 255, 255);
  leds(11, 6) = CHSV(hue, 255, 255);
}

void p1score_animation() {
  uint8_t i = random8(0, leds.Height());
  leds(0, i) = CHSV(rhue, 255, 255);
  leds.ShiftLeft();
}


