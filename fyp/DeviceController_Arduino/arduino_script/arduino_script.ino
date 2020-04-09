#include "DHT.h"
#include <LiquidCrystal.h>
#include <SPI.h>

#define DHTTYPE DHT22
int mVperAmp = 100;
int RawValue = 0;
int ACSoffset = 2500;
int serialInMode = 0;
int TECMode = 6; 
int NewTECMode = 0;
float voltage = 0;
float Amps = 0;
String request;
const int ampmeter = A0;                        //pin of ampmeter
const int RELAY[6] = {48, 46, 44, 42, 40, 38};  //2V 4V 6V 8V 10V 12V
const int DHTPIN[6] = {45, 43, 41, 39, 37, 36}; // first pin for supply air , last pin for outdoor air

class SensorsFeedBack
{
public:
    float humidity;
    float tempeature;
};

SensorsFeedBack indoorAirData[4];
SensorsFeedBack supplyAirData;
SensorsFeedBack outDoorAirData;

DHT dht[6] = {
    DHT(DHTPIN[0], DHTTYPE),
    DHT(DHTPIN[1], DHTTYPE),
    DHT(DHTPIN[2], DHTTYPE),
    DHT(DHTPIN[3], DHTTYPE),
    DHT(DHTPIN[4], DHTTYPE),
    DHT(DHTPIN[5], DHTTYPE)};

void setup()
{

    Serial.begin(115200);
    while(!Serial){
        ;//wait for serial port to connect
    }

    //initialize the RELAY, ensure all relays are off
    for (int i = 0; i <= 5; i++)
    {   
        dht[i].begin();
        pinMode(RELAY[i], OUTPUT);
        digitalWrite(RELAY[i], LOW);
    }
    Serial.println("ready");
}

void ReadSerial(){

    while (true){
        if (Serial.available() > 0)
        {
        // read the incoming byte:
        request = Serial.readString();
        return;
        }
    }
}

void loop()
{

    ReadSerial();
    if (request == "requestData") {
        Serial.println("data request sent successfully");
        ReadData();
        SendData();
    } else if (request == "SetMode"){
        Serial.println("set mode request received");
        ReadSerial();
        NewTECMode = request.toInt();
        if (NewTECMode != TECMode){
            TECMode = NewTECMode;
            Serial.println("Setting TECmode: "+ String(TECMode));
            SetMode();
            Serial.println("TECmode: "+ String(TECMode)+ " set successfully");
        } else {
            TECMode = NewTECMode;
            Serial.println("TECmode unchage");
            Serial.println("TECmode "+ String(TECMode)+ " unchage");
        }
        
    } 
}

void SendData(){

    Serial.println(outDoorAirData.humidity);
    Serial.println(outDoorAirData.tempeature);
    Serial.println(supplyAirData.humidity);
    Serial.println(supplyAirData.tempeature);

    for (int i = 0; i < 4; i++)
    {
        Serial.println(indoorAirData[i].humidity);
        Serial.println(indoorAirData[i].tempeature);

    }

    Serial.println((TECMode + 1 ) * 2 );;
    Serial.println(Amps);
}

void ReadData()
{
    outDoorAirData.humidity =   dht[5].readHumidity() ;
    outDoorAirData.tempeature =  dht[5].readTemperature() ;

    supplyAirData.humidity =   dht[4].readHumidity() ;
    supplyAirData.tempeature =  dht[4].readTemperature();

    for (int i = 0; i < 4; i++)
    {
        indoorAirData[i].humidity =  dht[i].readHumidity() ;
        indoorAirData[i].tempeature =  dht[i].readTemperature() ;
    }

    //get amp value
    RawValue = analogRead(ampmeter);
    voltage =  (RawValue / 1024.0) * 5000;
    Amps = (((voltage - ACSoffset) / mVperAmp) + 0.05) ;
    // Read temperature as Celsius
}

void SetMode()
{
    //deactivate all relay
    for (int foo = 0; foo <= 5; foo++)
    {
        digitalWrite(RELAY[foo], LOW);
        delay(100);
    };

    if (TECMode != 6)
    {
        digitalWrite(RELAY[TECMode], HIGH); //set selected relay to hi
        //TECvoltage = String((TECmode + 1) * 2) + "V"; voltage value
    }
}
