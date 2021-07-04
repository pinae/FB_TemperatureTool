#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h> 
#include <SoftwareSerial.h>

#define ONE_WIRE_BUS 2

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature tempSensors(&oneWire);
SoftwareSerial rs485(9, 10);
bool temperatureRequested = false;
unsigned short int a;

void setup() {
  Serial.begin(115200);
  pinMode(3, OUTPUT);
  while (!Serial) {};
  rs485.begin(9600);
  tempSensors.begin();
  pinMode(11, OUTPUT);
}

void loop() {
  tempSensors.requestTemperatures();
  Serial.print("Temperature: ");
  float temperature = tempSensors.getTempCByIndex(0);
  Serial.print(temperature);
  Serial.println("°C"); 
  analogWrite(11, map(
    (long) floor(temperature*100), 
    5000, -1000, 
    0, 255));
  digitalWrite(3, LOW);
  rs485.listen();
  while(rs485.available() > 0) {
    char inByte = rs485.read();
    if (inByte != 0) Serial.write(inByte);
    if (inByte == 'r') temperatureRequested = true;
  }
  if (temperatureRequested) {
    char buffer[50];
    sprintf(buffer, "{\"temperature\": %d.%d}\n", 
      (int) floor(temperature), 
      (int) floor(temperature * 100) % 100);
    digitalWrite(3, HIGH);
    rs485.write(buffer);
    Serial.print("\nWritten to bus: ");
    Serial.print(buffer);
    temperatureRequested = false;
  }
}