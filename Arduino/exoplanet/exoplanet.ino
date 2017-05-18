// Functions
void serialAction();

// Variables
int handshakePhase;

// Code
void setup() {
  Serial.begin(115200);
  Serial.print("Ready\n");
  handshakePhase = 0;
}

void loop() {
  int i = Serial.available();
  if (i > 0) { serialAction(i); }
  delay(100);
}

void serialAction(int chars) {
  char *serialData = (char *) malloc(chars + 1);
  for (int i = 0; i < chars; i++){
    serialData[i] = Serial.read();
  }
  serialData[chars] = '\0';
  switch (handshakePhase) {
    case 0:
      Serial.print(serialData);
      handshakePhase++;
      break;
    case 1:
      if (!strncmp(serialData, "ok", 2)) {
        handshakePhase++;
        Serial.print("Connected\n");
      } else {
        Serial.print("Not okay. You sent ");
        Serial.println(serialData);
      }
      break;      
  }
  free(serialData);
}

