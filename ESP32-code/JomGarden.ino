#define BLYNK_TEMPLATE_ID       "PUT_BLYNK_TEMPLATE_ID_HERE"
#define BLYNK_TEMPLATE_NAME     "PUT_BLYNK_TEMPLATE_NAME_HERE"

#include "BlynkEdgent.h"

#define LDR_PIN       2
#define LED_PIN       4
#define SOIL_PIN      13
#define RELAY_PIN     14
#define TIME_PUMP_ON  3

// blynk
#define BLYNK_PRINT             Serial
#define APP_DEBUG

const float DRY_SOIL = 5.00;
int adc_value = 0;
int pumpButtonState = 0;
double ledButtonState = 0.00;

BLYNK_WRITE(V1) { /* Virtual Pin 1 is assigned to the button widget */
  // get the state of the button
  pumpButtonState = param.asInt();
  if (pumpButtonState == 1) {
    Serial.println("(PUMP) Turned on remotely via Blynk");
    digitalWrite(RELAY_PIN, LOW);
  }
  else if (pumpButtonState == 0) {
    Serial.println("(PUMP) Turned off remotely via Blynk");
    digitalWrite(RELAY_PIN, HIGH);
  }
}

void getSoilMoisture() {
  adc_value = analogRead(SOIL_PIN);
  // calculate percentage soil moisture from obtained 'value' from SOILPIN
  float moisture_pct = 100 - ((adc_value/4095.00) * 100);
  // Serial.print("ADC Value: ");
  // Serial.print(adc_value);
  // Serial.print("(MST-SSR) Moisture value: ");
  // Serial.println(moisture_pct);
  Blynk.virtualWrite(V5, moisture_pct);
  updatePump(moisture_pct);
}

void updatePump(float moisture_pct) {
  if (moisture_pct < DRY_SOIL) {
    // Serial.println("(PUMP) moisture_pct below DRY_SOIL threshold");
    digitalWrite(RELAY_PIN, HIGH);
  }
}

void getLightSensor() {
  int val = analogRead(LDR_PIN);
  //Serial.println("(LDR_PIN)-val: ");
  //Serial.print(val);

  // check sensorvalue and determine digitalwrite
  if (val == HIGH) {
    digitalWrite(LED_PIN, LOW);
  }
  else if (val == LOW) {
    digitalWrite(LED_PIN, HIGH);
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LDR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  BlynkEdgent.begin();
}

void loop() {
  getLightSensor(); // LDR fried itself.
  getSoilMoisture();
  BlynkEdgent.run();
  delay(250);
}

// ...virtualprint into blynk
