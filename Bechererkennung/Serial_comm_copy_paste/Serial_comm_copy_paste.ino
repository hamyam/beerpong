String id = "p1";
char c;

void send_score() {
  Serial.println(score);
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

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  read_input();
}
