#include "DHT.h"
#include "OneWire.h"
#include "DallasTemperature.h"
#include <LiquidCrystal.h>
#include <SPI.h>
#include <SD.h>
#define DHTTYPE DHT22

LiquidCrystal lcd(33, 31, 28, 26, 24, 22);

int mVperAmp = 100;
int RawValue = 0;
int ACSoffset = 2500;
float humidity[5];
float tempeature[7];
double Voltage = 0;
double Amps = 0;
const int relay1 = 13;
const int relay2 = 12;
const int relay3 = 11;
const int relay4 = 10;
const int relay5 = 9;
const int relay6 = 8;
const int DHTPIN_0 = 2;
const int DHTPIN_1 = 3;
const int DHTPIN_2 = 5;
const int DHTPIN_3 = 6;
const int DHTPIN_4 = 7;
const int ONE_WIRE_BUS = 10;
const int TEMPERATURE_PRECISION = 11;
const int ampmeter = A7;
const int chipSelect = 4;

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

int numberOfDevices;
DeviceAddress tempDeviceAddress;

DHT dht0(DHTPIN_0, DHTTYPE);
DHT dht1(DHTPIN_1, DHTTYPE);
DHT dht2(DHTPIN_2, DHTTYPE);
DHT dht3(DHTPIN_3, DHTTYPE);
DHT dht4(DHTPIN_4, DHTTYPE);

void setup()
{
  Serial.begin(9600);
  while (!Serial)
  {
    ;
  }
  Serial.print("Initialing SD card");
  LcdPrint(1, "Initialing SD card");
  if (!SD.begin(chipSelect))
  {
    Serial.println("Card failed, or not present");
    LcdPrint(1, "Card failed, or not present");
    // don't do anything more:
    while (1)
      ;
  }
  delay(1000);

  lcd.begin(16, 2);
  lcd.print("LUN FYP 2019");
  delay(5000);

  Serial.println("card initialized.");
  LcdPrint(1, "card initialized.");
  delay(500);
  dht0.begin();
  dht1.begin();
  dht2.begin();
  dht3.begin();
  dht4.begin();
  sensors.begin();

  pinMode(ampmeter, INPUT);
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);
  pinMode(relay5, OUTPUT);
  pinMode(relay6, OUTPUT);
  digitalWrite(relay1, LOW);
  digitalWrite(relay2, LOW);
  digitalWrite(relay3, LOW);
  digitalWrite(relay4, LOW);
  digitalWrite(relay5, HIGH);
  digitalWrite(relay6, LOW);

  numberOfDevices = sensors.getDeviceCount();

  for (int i = 0; i < numberOfDevices; i++)
  {
    // Search the wire for address
    if (sensors.getAddress(tempDeviceAddress, i))
    {
      sensors.setResolution(tempDeviceAddress, TEMPERATURE_PRECISION);
    }
  }
}

// function to print the temperature for a device
void printTemperature(DeviceAddress deviceAddress)
{

  float tempC = sensors.getTempC(deviceAddress);
  ;
  Serial.print(tempC);
  Serial.print("*C: ");
  Serial.println();
}

void LcdPrint(int line, string stringToPrint)
{
  line--;
  for (int i = 0; i <= 16; i++)
  {
    lcd.setCursor(i, line);
    lcd.print("");
  }
  lcd.setCursor(0, line);
  lcd.print(stringToPrint);
}

void ReadAmpere()
{
  RawValue = analogRead(ampmeter);
  Voltage = (RawValue / 1024.0) * 5000;
  Amps = ((Voltage - ACSoffset) / mVperAmp);
}

void ReadTempAndHum()
{
  h[0] = dht0.readHumidity();
  h[1] = dht1.readHumidity();
  h[2] = dht2.readHumidity();
  h[3] = dht3.readHumidity();
  h[4] = dht4.readHumidity();

  // Read temperature as Celsius
  t[0] = dht0.readTemperature();
  t[1] = dht1.readTemperature();
  t[2] = dht2.readTemperature();
  t[3] = dht3.readTemperature();
  t[4] = dht4.readTemperature();
  //t[6] =
  //t[7] =
}
void loop()
{
  // put your main code here, to run repeatedly:
  delay(1000);
  Serial.println(analogRead(ampmeter));
  //
  //
  //amper!!!!!!!!
  //
  //
  //
  sensors.requestTemperatures();
  int i;
  for (i = 0; i < numberOfDevices; i++)
  {
    // Search the wire for address
    if (sensors.getAddress(tempDeviceAddress, i))
    {
      // Output the device ID
      Serial.print("Water Tempeature ");
      Serial.print(i, DEC);
      Serial.print(" :");

      // It responds almost immediately. Let's print out the data
      printTemperature(tempDeviceAddress); // Use a simple function to print out the data
    }
    //else ghost device! Check your power requirements and cabling
  }

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)

  ReadTempAndHum();

  for (i = 0; i <= 4; i++)
  {
    Serial.print(i);
    Serial.print("\t | Humidity: ");
    Serial.print(h[i]);
    Serial.print("%");
    Serial.print(" | Tempeature: ");
    Serial.print(t[i]);
    Serial.print("*");
    Serial.print(" |");
    Serial.println();
  }

  Serial.println("------------------------------------------------------------");

  String dataString = "content";
  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile)
  {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    Serial.println(dataString);
  }
  // if the file isn't open, pop up an error:
  else
  {
    Serial.println("error opening datalog.txt");
  }
}
// function to print a device address
