#include "DHT.h"
#include <LiquidCrystal.h>
#include <SPI.h>
#include <SD.h>
#include <uptime_formatter.h>
#include <virtuabotixRTC.h>

#define DHTTYPE DHT22

int TECmode;
int mVperAmp = 100;
int RawValue = 0;
int ACSoffset = 2500;
float Voltage = 0;
float Amps = 0;
String TECvoltage = "";
int lastMin = 0;
int lastSec = 0;

class SensorsFeedBack 
{
public:
    float humidity;
    float tempeature;
};

SensorsFeedBack indoorAirData[4];
SensorsFeedBack supplyAirData;
SensorsFeedBack outDoorAirData;

virtuabotixRTC myRTC(6, 7, 8);

const int RELAY[6] = {48, 46, 44, 42, 40, 38};  //2V 4V 6V 8V 10V 12V
const int DHTPIN[6] = {45, 43, 41, 39, 37, 36}; // first pin for supply air , last pin for outdoor air

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
}

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
    B00000
}
//degree C symbol

void setup()
{
    Serial.begin(9600);
    lcd.createChar(0, degC);
    lcd.begin(16, 2);
    PrintToLCD("LUN FYP 2020");
    delay(1000);
    PrintToLCD("Initialing");

    PrintToLCD("Init SD card");
    Serial.println("Initialing SD card");
    if (!SD.begin(chipSelect))
    {
        PrintToLCD("Card failed");
        Serial.println("Card failed, or not present");
        // don't do anything more:
        while (1);
    }
    
    PrintToLCD("Card init succ");
    Serial.println("card initialized.");
    delay(1000);
    PrintToLCD("Init sensors");

    for (int i = 0 ; i <= 5 ; i++ ){
        pinMode(RELAY[i], OUTPUT);
        digitalWrite(RELAY[i], LOW);
    }

    // Set the current date, and time in the following format:
    // seconds, minutes, hours, day of the week, day of the month, month, year
    //myRTC.setDS1302Time(45, 41, 13, 2, 18, 2, 2020); //Here you write your actual time/date as shown above 
    //but remember to "comment/remove" this function once you're done
    //The setup is done only one time and the module will continue counting it automatically
    
    myRTC.updateTime();
    //Serial.println(getDateTime());
    PrintToLCD(getDateTime());
    delay(1000);
    
    do {
        myRTC.updateTime(); 
    } while(myRTC.seconds != 0);
    //wait unit 00s

    PrintToLCD("sys start up");
    PrintToLCD("successfully");

}

void loop() {
    //SerialPrintData();
    TECmode = RandMode();
    SetMode();
    myRTC.updateTime();
    delay(4000)
    ReadAmp();
    ReadTempAndHumidity();
    DisplayData();
    //SerialPrintData();
    PrintToLCD(getDateTime());
    do
    {
        delay(500);
    } while (checkTime());
    SaveData();
    delay(100);

}
boolean checkTime() {
    myRTC.updateTime();
    if( myRTC.seconds >= 0 && myRTC.seconds <= 3){ 
        return 0;
    } else if ( myRTC.seconds >= 20 || myRTC.seconds <= 23 ){
        return 0;
    } else if ( myRTC.seconds >= 40 || myRTC.seconds <= 43 ){
        return 0;
    } else {
        return 1;
    }
}

void ReadTempAndHumidity() {
    outDoorAirData.humidity = dht[5].readHumidity();
    outDoorAirData.tempeature = dht[5].readTemperature();

    supplyAirData.humidity = dht[4].readHumidity();
    supplyAirData.tempeature = dht[4].readTemperature();

    for (int i = 0; i < 4; i++) {
        indoorAirData[i].humidity = dht[i].readHumidity();
        indoorAirData[i].tempeature = dht[i].readTemperature();
    }
    // Read temperature as Celsius
}

void ReadAmp() {
    //get amp value
    RawValue = analogRead(ampmeter);
    Voltage = (RawValue / 1024.0) * 5000;
    Amps = (((Voltage - ACSoffset) / mVperAmp) + 0.05);
    //Serial.println(Amps);
}

void PrintToLCD(String s) {
    //clear the monitor and print string s
    lcd.clear();
    lcd.print(s);
    delay(500);
}

String getDateTime() {
    //return a datetime string
    return String(myRTC.dayofmonth) + "/" + String(myRTC.month) + "/" + String(myRTC.year) + " " + String(myRTC.hours) + ":" + String(myRTC.minutes) + ":" + String(myRTC.seconds);
}

void LCDisplatPatter(String sensorName, tempeature, humidity ){

        lcd.setCursor(0, 0);
        lcd.print(sensorName);
        lcd.setCursor(0, 1);
        lcd.print(tempeature);
        lcd.write((byte)0);
        lcd.print("C   ");
        lcd.print(humidity);
        lcd.print("%");
        delay(1500);
        lcd.setCursor(0, 0);
        lcd.print("      ");
        lcd.setCursor(0, 1);
        lcd.print("                ");

}

void DisplayData()
{
    lcd.clear();
    lcd.setCursor(7, 0);
    lcd.print(TECvoltage + " ");
    lcd.print(Amps / 1);
    lcd.print("A");

    for (int foo = 0; foo < 4; foo++) {
        LCDisplatPatter(("IAS" + String(foo)+ " ") ,indoorAirData[foo].tempeature, indoorAirData[foo].humidity)
    };

    LCDisplatPatter("SAS   " , supplyAirData.tempeature , supplyAirData.humidity );
    LCDisplatPatter("SAS   " , supplyAirData.tempeature , supplyAirData.humidity );

}

void SaveData()
{
    
    File dataFile = SD.open("DATALOG.csv", FILE_WRITE);
    PrintToLCD("Storing data...");
    // if the file is available, write to it:
    if (dataFile) {
        dataFile.print(getDateTime());
        dataFile.print(", ");
        dataFile.print(supplyAirData.humidity);
        dataFile.print(", ");
        dataFile.print(supplyAirData.tempeature);
        dataFile.print(", ");
        dataFile.print(outDoorAirData.humidity);
        dataFile.print(", ");
        dataFile.print(outDoorAirData.tempeature);
        dataFile.print(", ");

        for (int i = 0; i < 4; i++) {
            dataFile.print(indoorAirData[i].humidity);
            dataFile.print(", ");
            dataFile.print(indoorAirData[i].tempeature);
            dataFile.print(", ");
        }
        dataFile.print(TECvoltage);
        dataFile.print(", ");
        dataFile.print(Amps);
        dataFile.println("");
        dataFile.close();
    }
    // if the file isn't open, pop up an error:
    else{
        Serial.println("error opening datalog.csv");
        PrintToLCD("Error opening datalog.csv");
        while (1);
    }
    PrintToLCD("Data Stored successfully");
}

int RandMode()
{
    if (((myRTC.hours * myRTC.dayofmonth) % 3) == 0 ){
        if (random(0, 9) > 4){
            return random(3, 5);
        } else   {
            return random(0, 6);      
        }
    } else  {
        return random(0,6);  
    }
}

void SetMode()
{
    if (TECmode > 6 || TECmode < 0){
        Serial.print("TECModeError");
        PrintToLCD("TEC Mode out of bound");
        while (1);
    }
    //deactivate all relay
    for (int foo = 0; foo <= 5; foo++){
        digitalWrite(RELAY[foo], LOW);
        delay(100);
    };

    if (TECmode != 6){
        digitalWrite(RELAY[TECmode], HIGH);
        TECvoltage = String((TECmode + 1) * 2) + "V";
        //add Voltage value;
    }
    else {
        // do nothing. i.e. all relay deactivate
        PrintToLCD("TEC OFF");
        TECvoltage = "0V";
    }
}

/* 
void SerialPrintPattern(String sensorName ,double humidity ,double tempeature){

    //printing pattern of serial 
    Serial.print(sensorName);
    Serial.print("\t | Humidity: ");
    Serial.print(humidity);
    Serial.print("%");
    Serial.print(" | Tempeature: ");
    Serial.print(tempeature);
    Serial.print(" Degree C");
    Serial.print(" |");
    Serial.println();
    
}
*/
/* 
void SerialPrintData()
{ 
    //print data to serial monitor
    SeriaPrintPattern("Outdoor air" ,outDoorAirData.humidity, outDoorAirData.tempeature);
    SeriaPrintPattern("supply air" ,supplyAirData.humidity, supplyAirData.tempeature);

    for (int i = 0; i < 4; i++)
    {
        SeriaPrintPattern(String(i) ,indoorAirData[i].humidity, indoorAirData[i].tempeature)
    }
    Serial.println("------------------------------------------------------------");
} 
 */