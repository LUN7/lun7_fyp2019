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
float humidity[10];
float tempeature[10];

class SensorsFeedBack {
    public:
        float humidity;
        float tempeature;
};

SensorsFeedBack indoorAirData[4];
SensorsFeedBack supplyAirData;

const int RELAY[6] = { 48 , 46, 44 , 42 , 40 , 38 }; //2V 4V 6V 8V 10V 12V 
const int DHTPIN[5] = { 45 , 43 , 41 , 39 , 37 };

//const int DHTPIN_0 = 45;
//const int DHTPIN_1 = 43;
//const int DHTPIN_2 = 41;
//const int DHTPIN_3 = 39;
//const int DHTPIN_4 = 37;

//fragiled sensor

DHT dht[5] = {
    DHT( DHTPIN[0] , DHTTYPE ) , 
    DHT( DHTPIN[1] , DHTTYPE ) ,
    DHT( DHTPIN[2] , DHTTYPE ) ,
    DHT( DHTPIN[3] , DHTTYPE ) ,
    DHT( DHTPIN[4] , DHTTYPE )
    };

//DHT dht0(DHTPIN_0, DHTTYPE)

const int ampmeter = A0;
const int chipSelect = 4 ;

LiquidCrystal lcd(33, 31, 28, 26, 24, 22);


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
    for (int i=0 ; i<5 ; i++){
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

    supplyAirData.humidity = dht[0].readHumidity();
    supplyAirData.tempeature = dht[0].readTemperature();
    
    for ( int i = 1 ; i < 5 ; i++ ){
        indoorAirData[i].humidity = dht[i].readHumidity();
        indoorAirData[i].tempeature = dht[i].readTemperature();
    }
    // Read temperature as Celsius   
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
        dataFile.print(supplyAirData.humidity);
        dataFile.print(", ");
        dataFile.print(supplyAirData.tempeature);
        dataFile.print(", ");
        for (int i = 1; i < 5 ; i++){
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