// v01 : Suppress interrupts
// v02 : Temp Air_Eau et changement de couleurs jcb - Save 07-27
// v03 : String serverName = "http://192.168.129.100:82/";
#include <Arduino.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include <WiFi.h>
#include <HTTPClient.h>
int Test = 2;

// Code pour Interruption
// Filtre anti-rebond (debouncer)
/*
#define DEBOUNCE_TIME 250
volatile uint32_t DebounceTimer = 0; // Debounce pour Bouton
// Broche sur laquelle est connectee le bouton, le detecteur de mouvement PIR ou le radar
#define PIN_BUTTON 0 //0=Bouton Gauche, 35=Bouton Droit
uint32_t mode_count = 0;

// Decommenter pour de pas mettre la fonction dans la RAM de l ESP32
//void buttonpressed() {

// La fonction est placee dans la RAM de l ESP32. 
void IRAM_ATTR buttonpressed() {
  if ( millis() - DEBOUNCE_TIME  >= DebounceTimer ) {
    DebounceTimer = millis();
    mode_count += 1;
    //Serial.printf("Button has been pressed %u times\n", button_count);
  } 
}
*/

TFT_eSPI tft=TFT_eSPI();
String Value(String Source,String SS, String ES);
class jcbSensor{
  private:
    float f=0.0;
    int Pos;
    int LCount=0;
    int X =0;
    String Libelle;
    String SrchStringStart; 
    String SrchStringEnd;
  public:
        jcbSensor(String Lib,String SSStart,String SSEnd){
          this-> Libelle=Lib;
          this-> SrchStringStart=SSStart;
          this-> SrchStringEnd=SSEnd;
          }
        void processSensor(String jcbInput,TFT_eSPI &tft){
            tft.fillScreen(TFT_BLACK);
            //tft.setTextColor(TFT_GREEN, TFT_BLACK);
            f=Value(jcbInput,SrchStringStart,SrchStringEnd).toFloat();
            //tft.setTextColor(TFT_BLUE,TFT_BLACK);
            String val1=this->Libelle;
            Serial.println("Test:"+val1);
            String val2 = String(f,1);
            // Value(jcbInput,this->SrchStringStart,
            //  this-> SrchStringEnd).substring(0,5);
            int w = tft.textWidth(val1, 4);
            tft.drawString(val1,(tft.width() - w) / 2,10,4);
            w = tft.textWidth(val2, 7);
            tft.drawString(val2, 0, 48, 8);
            //tft.drawString(val2,(tft.width() - w) / 2,48,8);
            //tft.setTextColor(TFT_GREEN, TFT_BLACK);
            delay(1000);
                  };

 
 
};

jcbSensor jcbDJ1("Temp EAU","Temp eau:","°C");
jcbSensor jcbDHTT("Temp AIR","Temp:","°C");
jcbSensor jcbDHTH("Humidite %","Hum:","%");
jcbSensor jcbBMPP("Pression HPa","Pression:","HPa:");
jcbSensor jcbBMPT("Temp PoolHouse","Temp BMP:","°C:");

// Replace with your network details
const char* ssid = "WiFi-2.4-DBFA";
const char* password = "alixetjc";
String serverName = "http://192.168.129.100:82/";

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
//unsigned long timerDelay = 600000;
// Set timer to 5 seconds (5000)
unsigned long timerDelay = 5000;
//void jcbDisplay(String parA){};
String Value(String Source,String SS, String ES){
  /*
  Serial.println("Source:"+Source);
  Serial.println("SS:"+SS);
  Serial.println("ES:"+ES);
  */
int i1=Source.indexOf(SS);
//Serial.println(i1);
 // int i2=Source.indexOf(ES);
 int i2=Source.indexOf(ES,i1);
 //Serial.println(i2);  
 Serial.println(SS);
 String Out;
 
 Out=Source.substring(i1+1+SS.length(),i2-1);
 //Serial.println("Out:"+Out);
 
 
 return Out;

  
}

//String Test="<!DOCTYPE HTML><html><head><meta http-equiv='refresh' content='12'></head><body><h1>ESP8266 - Pressure, Temperature and Humidity</h1></h3><h3>At UTC %ESPTime=2021-01-14T16:10:02_<h3>%DHTTemp=  1.60_</h3><h3>%DHTHumidity= 90.10_</h3><h3>%BMPTemp=  4.90_</h3><h3>%BMPPressure=997.56_</h3><h3>%BMPSLPressure=1018.49_</h3><h3>%DJ1Temp=3.00_</h3><h3>%ResetSec=14691_</h3><h3>%ForeCast=Sunny_</h3><h3>Commandes:/LED - /Time - /Temp/- /Reset - /Version-</h3><h3>OTA:SondePiscine01/jcb-</h3><h3>Version:SondePiscinev2021-01-14-01_Suppres_OTA.ino-</h3><h3>Mac Address:DC:4F:22:01:04:8B";
float fEau=0.0;
float fEauMin=99.0;




HTTPClient http;
void setup() {
  Serial.begin(9600); 
  uint64_t chipid = ESP.getEfuseMac();
  Serial.print("MAC : ");
  Serial.println(WiFi.macAddress());
  tft.init();
  tft.setTextSize(1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setRotation(1);
  

  WiFi.begin(ssid, password);
  tft.drawString("Connecting to ", 0, 0, 4);
  tft.drawString(ssid, 0, 32, 4);
  Serial.println("Connecting to ");
  Serial.println(ssid);
  Serial.println("-");
  Serial.print(password);
  delay(5000);
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  tft.drawString("Connected to ", 0, 0, 4);
  //tft.drawString(WiFi.localIP(), 0, 0, 64);
 
  Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");
  WiFiClient client;
  http.begin(serverName); //Specify the URL
  tft.fillScreen(TFT_BLACK);
  Serial.println("HTTPClient begin");
  tft.drawString("HTTPClient begin", 0, 0, 4);
  tft.drawString(serverName.substring(0,7), 0, 32, 4);
  tft.drawString(serverName.substring(7), 0, 64, 4);
  //Serial.println("End Of Setup");
  //delay(50000);
}

String payload;
char buf[50];
bool jcbGreen=true;
//
void loop() {
      if(WiFi.status()== WL_CONNECTED){
            //http.begin(serverName); //Specify the URL
            int httpCode = http.GET();                                        //Make the request
            //delay(5000);
            Serial.println(httpCode);
            if (httpCode ==200) { //Check for the returning code
                  payload = http.getString();
                  //Serial.println(httpCode);
                  //Serial.println(payload);
                  if (jcbGreen) {
                    tft.setTextColor(TFT_GREEN, TFT_BLACK);
                    jcbGreen=false;
                  }
                  else
                  {
                    tft.setTextColor(TFT_PINK, TFT_BLACK);
                    jcbGreen=true;
                  }
                  //tft.drawString(Time, 0, 0, 4);
                  //ltoa(millis(),buf,10);
                  Serial.println(payload);
                  jcbDHTH.processSensor(payload,tft);
                  jcbDHTT.processSensor(payload,tft);
                  jcbBMPP.processSensor(payload,tft);
                  jcbBMPT.processSensor(payload,tft); 
                  jcbDJ1.processSensor(payload,tft);
                  //tft.drawString(buf, 0, 96, 4);
                  }
 
            else {
                  Serial.println("Error on HTTP request");
                  }
            int jcbDelay = 30000;
            int interDelay = 2000;
            int Loop = 0;
            while (Loop < jcbDelay) {
              delay ( interDelay);
              Loop += interDelay;
              jcbDHTT.processSensor(payload,tft);
              delay ( interDelay);
              Loop += interDelay;
              jcbDJ1.processSensor(payload,tft);
            }
              

           }
       
}
