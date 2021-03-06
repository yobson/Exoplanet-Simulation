// Functions
char *serialGetData(int);
void serialHandshake(int);
void serialSelector(int);
void serialSettings(int);
void finiteGetSendData();
void infiniteGetSendData();

void clearSerial();

// Variables
void (*readCB)(int);
int handshakePhase;
int settingsPhase;

float sampleRate;
float referenceVoltage;
unsigned long sampleLength;

// Code
void setup() {
  clearSerial();
  Serial.begin(115200);
  Serial.print("Ready\n");
  handshakePhase = 0;
  readCB = &serialHandshake;
  sampleRate = 10;
}

void loop() {
  int i = Serial.available();
  if (i > 0) {
    delay(100);
    if (i == Serial.available()) {
      readCB(i);
    }
  }
}

char *serialGetData(int chars) {
  char *serialData = (char *) malloc(chars + 1);
  for (int i = 0; i < chars; i++){
    serialData[i] = Serial.read();
  }
  serialData[chars] = '\0';
  return serialData;
}

void serialHandshake(int chars) {
  char *serialData = serialGetData(chars);
  switch (handshakePhase) {
    case 0:
      Serial.print(serialData);
      handshakePhase++;
      break;
    case 1:
      if (!strncmp(serialData, "ok", 2)) {
        handshakePhase++;
        Serial.print("Connected\n");
        readCB = &serialSelector;
      } else {
        Serial.print("Not okay. You sent ");
        Serial.println(serialData);
      }
      break;      
  }
  free(serialData);
}

void serialSelector(int i) {
  char *serialData = serialGetData(i);
  if (!strncmp(serialData, "Settings", 8)) {
    Serial.print("SReady");
    readCB = &serialSettings;
    settingsPhase = 0;
  }
  else if (!strncmp(serialData, "Handshake", 9)) {
    readCB = &serialHandshake;
    handshakePhase = 0;
  }
  else if (!strncmp(serialData, "Live", 4)) {
    double sampleLengthDb = 1000 / sampleRate;
    sampleLength = (unsigned long)sampleLengthDb;
    infiniteGetSendData();
  }
  else {
    Serial.print("Unrecognised option ");
    Serial.print(serialData);
  }
  free (serialData);
}

void serialSettings(int i) {
  switch (settingsPhase) {
    case 0:
      sampleRate = Serial.parseFloat();
      break;
    case 1:
      referenceVoltage = Serial.parseFloat();
      readCB = &serialSelector;
  }
  Serial.print("SReady");
  settingsPhase++;
}

void infiniteGetSendData() {
  int i = 0;
  while (true) {
    if (Serial.available()) break;
    double rad = (double)i * 180/PI;
    double data = sin(rad) + 0.5*sin(2*rad);
    Serial.print(data);
    i++;
    delay(sampleLength);
  }
}

void clearSerial() {
  while (Serial.available()) {
    Serial.read();
  }
}

