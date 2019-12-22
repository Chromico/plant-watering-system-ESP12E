
//Created for GCI 2019 by CHROMICO




#include "ThingSpeak.h"
#include <ESP8266WiFi.h>

//----------------  Fill in your credentials   ---------------------
char ssid[] = "HUAWEI-B315-8344";             // your network SSID (name) 
char pass[] = "F2GG7R99DGN";         // your network password
unsigned long myChannelNumber = 0;  // Replace the 0 with your channel number
const char * myWriteAPIKey = "QMNQF00BZRPOD3D7";    // Paste your ThingSpeak Write API Key between the quotes 
//------------------------------------------------------------------

WiFiClient  client;

int moisturesensor = A0;
int waterpump = D1;

void setup() {
  
  //Initialize serial and wait for port to open:

  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. 
  }

  pinMode(moisturesensor, INPUT);
  pinMode(waterpump, OUTPUT);
  
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client); 
  
}

void loop() {
  int analogvalue = analogRead(moisturesensor);
  // Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");

    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass);
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected.");
  }
  int moistdetect = 0;
  int nomoistdetect = 1;
  int watering = 0;
  int notwatering = 1;

  // set fields one at a time
   ThingSpeak.setField(1,analogvalue);
  
  // set the status if over the threshold
  if(analogvalue > 900){
    
    ThingSpeak.setStatus("ALERT! DRY SOIL");
    ThingSpeak.setField(2,moistdetect);
    digitalWrite(waterpump, HIGH);   //waters the plant
    ThingSpeak.setField(3,watering);
    Serial.println("SOIL IS DRY, INITIALISING WATERING SEQUENCE");
  }else if(analogvalue < 500){
    
    ThingSpeak.setStatus("SOIL HAS BEEN WATERED OR MOIST");
    ThingSpeak.setField(2,nomoistdetect);
    digitalWrite(waterpump, LOW);  //stops watering the plant
    ThingSpeak.setField(3,notwatering); 
    Serial.println("SOIL IS NOW MOIST, INITIALISATION COMPLETE");
  }

  // Write the fields that you've set all at once.
  ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  delay(20000);
   // Wait 20 seconds before sending a new value
}
