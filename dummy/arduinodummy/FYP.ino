#include "DHT.h"
#include <LiquidCrystal.h>
#include <SPI.h>
#include <SD.h>
#include <uptime_formatter.h>
#include <virtuabotixRTC.h>

#define DHTTYPE DHT22

int sequence = 0;
int TECmode = 0;
int mVperAmp = 100;
int RawValue = 0;
int ACSoffset = 2500;
float Voltage = 0;
float Amps = 0;
String TECvoltage = "";
String saveVoltage = "";
int lastMin = 0;

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

DHT dht[6] = {
    DHT(DHTPIN[0], DHTTYPE),
    DHT(DHTPIN[1], DHTTYPE),
    DHT(DHTPIN[2], DHTTYPE),
    DHT(DHTPIN[3], DHTTYPE),
    DHT(DHTPIN[4], DHTTYPE),
    DHT(DHTPIN[5], DHTTYPE)
};

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
};
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
    //myRTC.setDS1302Time(30, 30, 15, 2, 10, 3, 2020); //Here you write your actual time/date as shown above 
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
    lastMin = myRTC.minutes;
    RandMode();
    SetMode();
}

void loop() {
    //SerialPrintData();
    delay(4000);
    myRTC.updateTime();
    ReadTempAndHumidity();
    ReadAmp();
    DisplayData();
    //SerialPrintData();
    myRTC.updateTime();
    do
    {
        PrintToLCD(String(myRTC.seconds) + 's');
        delay(100);
    } while (checkTime());
    myRTC.updateTime();
    ReadAmp();
    ReadTempAndHumidity();
    
    RandMode();
    saveVoltage = TECvoltage;
    SetMode();
    SaveData();
    saveVoltage = TECvoltage;
}

boolean checkTime() {
    myRTC.updateTime();
    if( myRTC.minutes != lastMin ){ 
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

void LCDisplatPatten(String sensorName, float tempeature, float humidity ){

        lcd.setCursor(0, 0);
        lcd.print(sensorName);
        lcd.setCursor(0, 1);
        lcd.print(tempeature);
        lcd.write((byte)0);
        lcd.print("C   ");
        lcd.print(humidity);
        lcd.print("%");
        delay(800);
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
        LCDisplatPatten(("IAS" + String(foo)+ " ") ,indoorAirData[foo].tempeature, indoorAirData[foo].humidity);
    };

    LCDisplatPatten("SAS   " , supplyAirData.tempeature , supplyAirData.humidity );
    LCDisplatPatten("SAS   " , supplyAirData.tempeature , supplyAirData.humidity );

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
        dataFile.print(saveVoltage);
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

void RandMode()
{
    if ( sequence !=0){
        sequence --;
        return;
    } else {

        sequence = random(1,3);

        if (random(0, 10) > 3){
            TECmode = random(3, 6);
            return;
        } else   {
            TECmode = random(0, 7);   
            return;
        }
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
void SerialPrintPatten(String sensorName ,double humidity ,double tempeature){

    //printing Patten of serial 
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
    SeriaPrintPatten("Outdoor air" ,outDoorAirData.humidity, outDoorAirData.tempeature);
    SeriaPrintPatten("supply air" ,supplyAirData.humidity, supplyAirData.tempeature);

    for (int i = 0; i < 4; i++)
    {
        SeriaPrintPatten(String(i) ,indoorAirData[i].humidity, indoorAirData[i].tempeature)
    }
    Serial.println("------------------------------------------------------------");
} 
 */