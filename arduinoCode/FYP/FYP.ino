#include "DHT.h"
#include <LiquidCrystal.h>
#include <SPI.h>
#include <SD.h>
#include <uptime_formatter.h>
#define DHTTYPE DHT22

int TECmode;
int mVperAmp = 100;
int RawValue = 0;
int ACSoffset = 2500;
float Voltage = 0;
float Amps = 0;

unsigned long lastMillis;
unsigned long currentMillis;

class SensorsFeedBack
{
public:
    float humidity;
    float tempeature;
};

SensorsFeedBack indoorAirData[4];
SensorsFeedBack supplyAirData;
SensorsFeedBack outDoorAirData;

const int RELAY[6] = {48, 46, 44, 42, 40, 38}; //2V 4V 6V 8V 10V 12V
const int DHTPIN[6] = {45, 43, 41, 39, 37, 36};

//const int DHTPIN_0 = 45;
//const int DHTPIN_1 = 43;
//const int DHTPIN_2 = 41;
//const int DHTPIN_3 = 39;
//const int DHTPIN_4 = 37;

//fragiled sensor

DHT dht[6] = {
    DHT(DHTPIN[0], DHTTYPE),
    DHT(DHTPIN[1], DHTTYPE),
    DHT(DHTPIN[2], DHTTYPE),
    DHT(DHTPIN[3], DHTTYPE),
    DHT(DHTPIN[4], DHTTYPE),
    DHT(DHTPIN[5], DHTTYPE)
    };

//DHT dht0(DHTPIN_0, DHTTYPE)

const int ampmeter = A0;
const int chipSelect = 4;

LiquidCrystal lcd(33, 31, 28, 26, 24, 22);

byte degC[] = {
    B01110,
    B01010,
    B01110,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000};
//degree C symbol

void setup()
{

    Serial.begin(9600);
    lcd.createChar(0, degC);
    lcd.begin(16, 2);
    PrintToLCD("Hello World");
    PrintToLCD("LUN FYP");
    PrintToLCD("Sys start up");

    PrintToLCD("Init SD card");
    Serial.println("Initialing SD card");
    if (!SD.begin(chipSelect))
    {
        PrintToLCD("Card failed");
        Serial.println("Card failed, or not present");
        // don't do anything more:
        while (1)
            ;
    }
    PrintToLCD("Card init succ");
    Serial.println("card initialized.");
    delay(1000);
    PrintToLCD("Init sensors");

    pinMode(RELAY[0], OUTPUT);
    pinMode(RELAY[1], OUTPUT);
    pinMode(RELAY[2], OUTPUT);
    pinMode(RELAY[3], OUTPUT);
    pinMode(RELAY[4], OUTPUT);
    pinMode(RELAY[5], OUTPUT);
    digitalWrite(RELAY[0], LOW);
    digitalWrite(RELAY[1], LOW);
    digitalWrite(RELAY[2], LOW);
    digitalWrite(RELAY[3], LOW);
    digitalWrite(RELAY[4], LOW);
    digitalWrite(RELAY[5], LOW);

    
    PrintToLCD("sys start up");
    PrintToLCD("successfully");
}

void loop()
{
    delay(1000);
    ReadAmp();
    ReadTempAndHumidity();
    //SerialPrintData();
    SetMode();
    lastMillis = millis();
    currentMillis = millis();
    do {
      ReadAmp();
      ReadTempAndHumidity();  
      if ( (currentMillis - lastMillis) < 0 ){
        while (1){
          PrintToLCD("millis() error");
        }; 
      };
      DisplayData();
      currentMillis = millis();
    } while( (currentMillis - lastMillis) <= 53000 );
    SaveData();
}

// function to print a device address
void SerialPrintData()
{

    Serial.print("OutDoor air");
    Serial.print("\t | Humidity: ");
    Serial.print(outDoorAirData.humidity);
    Serial.print("%");
    Serial.print(" | Tempeature: ");
    Serial.print(outDoorAirData.tempeature);
    Serial.print(" Degree C");
    Serial.print(" |");
    Serial.println();    
    Serial.print("Supply air");
    Serial.print("\t | Humidity: ");
    Serial.print(supplyAirData.humidity);
    Serial.print("%");
    Serial.print(" | Tempeature: ");
    Serial.print(supplyAirData.tempeature);
    Serial.print(" Degree C");
    Serial.print(" |");
    Serial.println();    

    for (int i = 0; i < 4; i++)
    {
        Serial.print(i);
        Serial.print("\t | Humidity: ");
        Serial.print(indoorAirData[i].humidity);
        Serial.print("%");
        Serial.print(" | Tempeature: ");
        Serial.print(indoorAirData[i].tempeature);
        Serial.print(" Degree C");
        Serial.print(" |");
        Serial.println();
    }
    Serial.println("------------------------------------------------------------");
}

void ReadTempAndHumidity()
{
    outDoorAirData.humidity = dht[5].readHumidity();
    outDoorAirData.tempeature = dht[5].readTemperature();

    supplyAirData.humidity = dht[0].readHumidity();
    supplyAirData.tempeature = dht[0].readTemperature();

    for (int i = 0; i < 4; i++)
    {
        indoorAirData[i].humidity = dht[i + 1].readHumidity();
        indoorAirData[i].tempeature = dht[i + 1].readTemperature();
    }
    // Read temperature as Celsius
}

void ReadAmp()
{
    //get amp value
    RawValue = analogRead(ampmeter);
    Voltage = (RawValue / 1024.0) * 5000;
    Amps = (((Voltage - ACSoffset) / mVperAmp) + 0.05);
    Serial.println(Amps);
}

void PrintToLCD(String s)
{
    lcd.clear();
    lcd.print(s);
    delay(500);
}

void DisplayData()
{
    lcd.clear();
    lcd.setCursor(7, 0);
    switch (TECmode)
    {
    case 0:
        //2V
        lcd.print("02V ");
        break;
    case 1:
        //4V
        lcd.print("04V ");
        break;
    case 2:
        //6V
        lcd.print("06V ");
        break;
    case 3:
        //8V
        lcd.print("08V ");
        break;
    case 4:
        //10V
        lcd.print("10V ");
        break;
    case 5:
        //12V
        lcd.print("12V ");
        break;
    case 6:
        //OFF
        lcd.print("OFF ");
        break;
    }
    lcd.print(Amps / 1);
    lcd.print("A");

    for (int foo = 0; foo < 4; foo++)
    {
        lcd.setCursor(0, 0);
        lcd.print("IAS");
        //IAS = Indoor Air Sensor
        lcd.print(foo);
        lcd.print(" ");
        lcd.setCursor(0, 1);
        lcd.print(indoorAirData[foo].tempeature);
        lcd.write((byte)0);
        lcd.print("C   ");
        lcd.print(indoorAirData[foo].humidity);
        lcd.print("%");
        delay(1500);
        lcd.setCursor(0, 0);
        lcd.print("      ");
        lcd.setCursor(0, 1);
        lcd.print("                ");
    };
    lcd.setCursor(0, 0);
    lcd.print("SAS");
    //SAS = Supply Air SensorsFeedBack
    lcd.print("   ");
    lcd.setCursor(0, 1);
    lcd.print(supplyAirData.tempeature);
    lcd.write((byte)0);
    lcd.print("C   ");
    lcd.print(supplyAirData.humidity);
    lcd.print("%");

    lcd.setCursor(0, 0);
    lcd.print("OAS");
    //OAS = Outdoor Air SensorsFeedBack
    lcd.print("   ");
    lcd.setCursor(0, 1);
    lcd.print(outDoorAirData.tempeature);
    lcd.write((byte)0);
    lcd.print("C   ");
    lcd.print(outDoorAirData.humidity);
    lcd.print("%");

}

void SaveData()
{

    File dataFile = SD.open("datalog.csv", FILE_WRITE);
    PrintToLCD("Storing data...");
    // if the file is available, write to it:
    if (dataFile)
    {
        dataFile.print(supplyAirData.humidity);
        dataFile.print(", ");
        dataFile.print(supplyAirData.tempeature);
        dataFile.print(", ");
        dataFile.print(outDoorAirData.humidity);
        dataFile.print(", ");
        dataFile.print(outDoorAirData.tempeature);
        dataFile.print(", ");

        for (int i = 0; i < 4; i++)
        {
            dataFile.print(indoorAirData[i].humidity);
            dataFile.print(", ");
            dataFile.print(indoorAirData[i].tempeature);
            dataFile.print(", ");
        }
        dataFile.print(TECmode);
        dataFile.print(", ");
        dataFile.print(Amps);
        dataFile.println("");
        dataFile.close();
    }
    // if the file isn't open, pop up an error:
    else
    {
        Serial.println("error opening datalog.csv");
        PrintToLCD("Error opening datalog.csv");
        while (1)
            ;
    }
    PrintToLCD("Data Stored successfully");
}

int RandMode()
{
    return random(0, 6);
}

void SetMode()
{
    TECmode = RandMode();
    if (TECmode > 6 || TECmode < 0)
    {
        Serial.print("TECModeError");
        PrintToLCD("TEC Mode out of bound");
        while (1)
            ;
    }
    //deactivate all relay
    for (int foo = 0; foo <= 5; foo++)
    {
        digitalWrite(RELAY[foo], LOW);
        delay(100);
    };

    if (TECmode != 6)
    {
        digitalWrite(RELAY[TECmode], HIGH);
        //add Voltage value;
    }
    else
    {
        // do nothing. i.e. all relay deactivate
        PrintToLCD("TEC OFF");
    }
}
