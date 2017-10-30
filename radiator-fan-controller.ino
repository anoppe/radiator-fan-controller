#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP8266Influxdb.h>
#include <ESP8266WiFi.h>
 
#define ONE_WIRE_BUS D2
#define FAN_PORT D0

const char* ssid = "<SSID>";
const char* password = "<SSID_PWD>";

const char *INFLUXDB_HOST = "<INFLUX_HOST>";
const uint16_t INFLUXDB_PORT = <INFLUX_PORT>;

const char *DATABASE = "<INFLUX_DB>";
const char *DB_USER = "<INFLUX_USR>";
const char *DB_PASSWORD = "<INFLUX_PWD>";

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);

WiFiClient espClient;

Influxdb influxdb(INFLUXDB_HOST, INFLUXDB_PORT);

char temperatureString[6];
const int switchPin = D0;

void setup(void){
  Serial.begin(115200);
  setup_wifi();
  
  influxdb.opendb(DATABASE, DB_USER, DB_PASSWORD);
  
  DS18B20.begin();
  pinMode(FAN_PORT, OUTPUT);

}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
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
  String data;
  Serial.println(temperatureString);
  if (temperature > 28.00) {
    Serial.println("temperature to high, lets start the fan");
    digitalWrite(FAN_PORT, HIGH);
    data = "fancontrol,room=woonkamer,location=back-window enabled=1";
  } else {
    Serial.println("temperature decreased enough, lets stop the fan");
    digitalWrite(FAN_PORT, LOW);
    data = "fancontrol,room=woonkamer,location=back-window enabled=0";
  }

  influxdb.write("fancontrol,room=woonkamer,location=back-window temperature=" + String(temperatureString));

  influxdb.write(data);
  Serial.println(influxdb.response() == DB_SUCCESS ? "HTTP write success" : "Writing failed");
  
  delay(60000);

}
