//wifi
#include <WiFi.h>
#include <ThingSpeak.h>
#include <dht11.h>
dht11 DHT11;
#define dhtpin 14
WiFiClient client;

const char *ssid = "abir";
const char *password = "abir1234";

const long CHANNEL = 942845; //Your ThingSpeak Channel ID;
const char *WRITE_API = "LIJVET9ZGTQAOQQB";

//defines pins numbers
float flag = 0.00, bin = 0.00;
float temp;
int pin8 = 18;
int pin9 = 23;
int sensor = 34;  //A2 will take the data of gas sensor
int sensor2 = 27;
int sensorValue = 0;
int sensorValue2 = 0;
int 

// defines variables
long prevMillisSensor = 0;
int intervalSensor = 2000;
long prevMillisThingSpeak = 0;
int intervalThingSpeak = 1000; // Minimum ThingSpeak write interval is 15 seconds


void setup() {
  Serial.begin(115200);
  pinMode(pin8, OUTPUT);
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);  // Initialize ThingSpeak

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println("Connected to the WiFi network");

}

void loop() {

  HeartbeatMonitor();      //function for pulse measurment

  BodyTemperatureMonitor();  //function for body temperature measurment

  CO2();  //function for gas measurment
  CO();
  DHT11.read(dhtpin);// initialize the reading

  int humidity = DHT11.humidity;// get humidity
  humidity=humidity-30;
  // Connect or reconnect to WiFi
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    while (WiFi.status() != WL_CONNECTED) {
      Serial.println("wait conneting");
      Serial.print(" Body temperature: ");
  Serial.print(temp);
  Serial.print("*F\n");
      WiFi.begin(ssid, password);
      Serial.print(".");
      delay(500);
    }
    Serial.println("\nConnected.");
  }

  //send data in thingspeak
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
      WiFi.begin(ssid, password);
    }
  }

  //send data in thingspeak

  if (millis() - prevMillisThingSpeak > intervalThingSpeak) {
    // Set the fields with the values
    ThingSpeak.setField(1, bin);
    ThingSpeak.setField(2, temp);
    ThingSpeak.setField(3, sensorValue);
    ThingSpeak.setField(4, sensorValue2);
    ThingSpeak.setField(5, humidity);
    // Write to the ThingSpeak channel
    int x = ThingSpeak.writeFields(CHANNEL, WRITE_API);
    if (x == 200) {
      Serial.println("Channel update successful.");
    }
    else {
      Serial.println("Problem updating channel. HTTP error code " + String(x));
    }
    prevMillisThingSpeak = millis();
  }



  delay(1000);

}

void HeartbeatMonitor()
{
  flag = analogRead(26); //A0 will take heartbeat data
  bin = (flag / 35.0) + 60;
  bin = bin - 100;

  Serial.print(" Heart-Beat Found ");
  Serial.print("BPM: ");
  Serial.print(bin);
  Serial.print("\n");
}

void BodyTemperatureMonitor()
{
  temp = analogRead(35);  //A1 will read temperature data
  temp = temp / 12; //*0.48828125;
  temp = (temp * 9) / 5 + 32;

  Serial.print(" Body temperature: ");
  Serial.print(temp);
  Serial.print("*F\n");
}

void CO2()
{
  // Read the input on analog pin 0 (named 'sensor')
  sensorValue = analogRead(sensor);
  sensorValue = sensorValue / 7.5;
  // If sensorValue is greater than 500
  if (sensorValue > 500) {
    // Activate digital output pin 8 - the LED will light up
    digitalWrite(pin8, HIGH);
  }
  else {
    // Deactivate digital output pin 8 - the LED will not light up
    digitalWrite(pin8, LOW);
  }
  Serial.print(" CO2 ");
  Serial.print(sensorValue);
  Serial.print("\n");
}

void CO()
{
  // Read the input on analog pin 0 (named 'sensor')
  sensorValue2 = analogRead(sensor2);
  sensorValue2 = sensorValue2 / 100;
  // If sensorValue is greater than 500
  if (sensorValue2 > 50) {
    // Activate digital output pin 8 - the LED will light up
    digitalWrite(pin9, HIGH);
  }
  else {
    // Deactivate digital output pin 8 - the LED will not light up
    digitalWrite(pin9, LOW);
  }
  Serial.print(" CO ");
  Serial.print(sensorValue2);
  Serial.print("\n");
}
