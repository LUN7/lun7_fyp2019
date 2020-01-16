#include "DHT.h"
#include <LiquidCrystal.h>
#include <SPI.h>
#include <SD.h>
#define DHTTYPE DHT22

int TECmode;
int mVperAmp = 100;
int RawValue = 0;
int ACSoffset = 2500;
float Voltage = 0;
float Amps = 0;
float humidity[5];
float tempeature[7];

const int RELAY[6] = {48, 46, 44, 42, 40, 38}; //2V 4V 6V 8V 10V 12V 
const int DHTPIN_0 = 45;
const int DHTPIN_1 = 43;
const int DHTPIN_2 = 41;
const int DHTPIN_3 = 39;
const int DHTPIN_4 = 37;
//fragiled sensor
const int ampmeter = A0;
const int chipSelect = 4 ;

LiquidCrystal lcd(33, 31, 28, 26, 24, 22);

DHT dht0(DHTPIN_0, DHTTYPE);
DHT dht1(DHTPIN_1, DHTTYPE);
DHT dht2(DHTPIN_2, DHTTYPE);
DHT dht3(DHTPIN_3, DHTTYPE);
DHT dht4(DHTPIN_4, DHTTYPE);

void setup() {

  Serial.begin(9600);

  lcd.begin(16, 2);
    PrintToLCD("Hello World");
    PrintToLCD("LUN FYP");
    PrintToLCD("Sys start up");
    
  PrintToLCD("Init SD card");
  Serial.println("Initialing SD card");
  if (!SD.begin(chipSelect)) {
    PrintToLCD("Card failed");
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
    PrintToLCD("Card init succ");
    Serial.println("card initialized.");
    delay(1000);
    PrintToLCD("Init sensors");
  
  dht0.begin();
  dht1.begin();
  dht2.begin();
  dht3.begin();
  dht4.begin();
  pinMode(RELAY[0], OUTPUT);
  pinMode(RELAY[1], OUTPUT);
  pinMode(RELAY[2], OUTPUT);
  pinMode(RELAY[3], OUTPUT);
  pinMode(RELAY[4], OUTPUT);
  pinMode(RELAY[5], OUTPUT);

  digitalWrite(RELAY[0] , LOW); 
  digitalWrite(RELAY[1] , LOW);
  digitalWrite(RELAY[2] , LOW);
  digitalWrite(RELAY[3] , LOW);
  digitalWrite(RELAY[4] , LOW);
  digitalWrite(RELAY[5] , LOW);

  PrintToLCD("sys start up");
  PrintToLCD("successfully");
}

void loop() {
    delay(1000);
    ReadAmp();
    ReadTempAndHumidity();
    SerialPrintData();
    SaveData();
    TECmode = RandMode();
    SetMode();
    DisplayData();
}

// function to print a device address
void SerialPrintData(){
    for (int i=0 ; i<=4 ; i++){
        Serial.print(i);
        Serial.print("\t | Humidity: ");
        Serial.print(humidity[i]);
        Serial.print("%");
        Serial.print(" | Tempeature: ");
        Serial.print(tempeature[i]);
        Serial.print("*");
        Serial.print(" |");
        Serial.println();
    }
    Serial.println("------------------------------------------------------------");
}

void ReadTempAndHumidity(){
    humidity[0] = dht0.readHumidity();
    humidity[1] = dht1.readHumidity();
    humidity[2] = dht2.readHumidity();
    humidity[3] = dht3.readHumidity();
    humidity[4] = dht4.readHumidity();

    // Read temperature as Celsius
    tempeature[0] = dht0.readTemperature();
    tempeature[1] = dht1.readTemperature();
    tempeature[2] = dht2.readTemperature();
    tempeature[3] = dht3.readTemperature();
    tempeature[4] = dht4.readTemperature();
    //tempeature[6] = 
    //tempeature[7] =  
}

void ReadAmp(){
    //get amp value
    RawValue = analogRead(ampmeter);
    Voltage = (RawValue / 1024.0) * 5000;
    Amps = (((Voltage - ACSoffset) / mVperAmp) + 0.05);
    Serial.println(Amps);
}

void PrintToLCD(String firstLine){

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(firstLine);
    delay(1000);
    }

void DisplayData(){

}

void SaveData(){
    
    File dataFile = SD.open("datalog.csv", FILE_WRITE);
    PrintToLCD("Data Storing...");
    // if the file is available, write to it:
    if (dataFile) {
        for (int i = 0; i <= 4; i++){
            dataFile.print(humidity[i]);
            dataFile.print(", ");
            dataFile.print(tempeature[i]);
            dataFile.print(", ");
        }
        dataFile.print(TECmode);
        dataFile.print(", ");
        dataFile.print(Amps);
        dataFile.println("");
        dataFile.close();
    }
    // if the file isn't open, pop up an error:
    else {
        Serial.println("error opening datalog.csv");
        PrintToLCD("Error opening datalog.csv");
        while(1);
    }
    PrintToLCD("Data Stored successfully");
}


int RandMode(){
   return random(0,6);
}

void SetMode(){
    
    if (TECmode > 6 || TECmode < 0){
        Serial.print("TECModeError");
        PrintToLCD("TEC Mode out of bound");
        while(1);
    }
    //deactivate all relay
    for ( int foo = 0 ; foo<= 5 ; foo++){
        digitalWrite(RELAY[foo] , LOW);
        delay(100);
    }
    if (TECmode != 6){
        digitalWrite(RELAY[TECmode] , HIGH);
        PrintToLCD("TEC Mode ");
        PrintToLCD(String(TECmode));
        PrintToLCD("V: ");
        //add Voltage value;
    }
    else {
        // do nothing. i.e. all relay deactivate
        PrintToLCD("TEC OFF");
    }
    
}
