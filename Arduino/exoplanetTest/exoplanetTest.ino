void finiteGetSendData() {
  double rad, data;
  for (int i = 0; i < 2048; i++) {
    rad = (double)i * 180/PI;
    data = sin(rad) + 0.5*sin(2*rad);
    Serial.println(data);
    delay(10);
  }
}

void setup() {
  Serial.begin(115200);
}

void loop() {
  Serial.println("Start############################################");
  finiteGetSendData();

}
