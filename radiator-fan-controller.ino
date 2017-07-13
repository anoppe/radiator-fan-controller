#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS D2
#define FAN_PORT D0

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);

char temperatureString[6];
const int switchPin = D0;

void setup(void){
  Serial.begin(115200);
  Serial.println("");
  
  DS18B20.begin();
  pinMode(FAN_PORT, OUTPUT);

}

float getTemperature() {
  float temp;
  do {
    DS18B20.requestTemperatures(); 
    temp = DS18B20.getTempCByIndex(0);
    delay(100);
  } while (temp == 85.0 || temp == (-127.0));
  return temp;
}


void loop() {

  float temperature = getTemperature();

  dtostrf(temperature, 2, 2, temperatureString);
  // send temperature to the serial console
  Serial.println(temperatureString);
  if (temperature > 28.00) {
    Serial.println("temperature to high, lets start the fan");
    digitalWrite(FAN_PORT, HIGH);
  } else {
    Serial.println("temperature decreased enough, lets stop the fan");
    digitalWrite(FAN_PORT, LOW);
  }
  
  delay(500);

}
