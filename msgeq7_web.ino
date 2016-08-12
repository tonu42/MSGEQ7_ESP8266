#include <ArduinoJson.h>
#include <DHT.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WebSocketsClient.h>
#include <Hash.h>



int strobePin = 12; // strobe is attached to digital pin 2
int resetPin = 2; // reset is attached to digital pin 3
int spectrumValue[7]; // to hold a2d values
 int FilterValue[7] = { 350 , 250 , 300 , 300 , 300 , 225 , 250 };

WebSocketsClient webSocket;
StaticJsonBuffer<200> jsonBuffer;
JsonObject& data = jsonBuffer.createObject();
char buffer[256];

void webSocketEvent(WStype_t type, uint8_t * payload, size_t lenght) {


    switch(type) {
        case WStype_DISCONNECTED:
            Serial.printf("[WSc] Disconnected!\n");
            break;
        case WStype_CONNECTED:
            {
                Serial.printf("[WSc] Connected to url: %s\n",  payload);
        
          // send message to server when Connected
        webSocket.sendTXT("Connected");
            }
            break;
        case WStype_TEXT:
           // Serial.printf("[WSc] get text: %s\n", payload);

      // send message to server
      // webSocket.sendTXT("message here");
            break;
        case WStype_BIN:
            Serial.printf("[WSc] get binary lenght: %u\n", lenght);
            hexdump(payload, lenght);

            // send data to server
            // webSocket.sendBIN(payload, lenght);
            break;
    }

}

void setup() {
  // put your setup code here, to run once:

  
  Serial.begin(115200);

 pinMode(strobePin, OUTPUT);
 pinMode(resetPin, OUTPUT);


 digitalWrite(resetPin, LOW);
 digitalWrite(strobePin, HIGH);

 
  Serial.println();
    Serial.println();
    Serial.println();

      for(uint8_t t = 4; t > 0; t--) {
          Serial.printf("[SETUP] BOOT WAIT %d...\n", t);
          Serial.flush();
          delay(1000);
      }

   WiFi.begin(ssid, password);



   webSocket.begin("192.168.0.18", 8080, "/MSGEQ7/endpoint");
    
   webSocket.onEvent(webSocketEvent);
}

void loop() {
  // put your main code here, to run repeatedly:
webSocket.loop();

digitalWrite(resetPin, HIGH);
 digitalWrite(resetPin, LOW);

 for (int i = 0; i < 7; i++)
 {
 digitalWrite(strobePin, LOW);
 delayMicroseconds(30); // to allow the output to settle
 spectrumValue[i] = analogRead(A0);
 spectrumValue[i] = constrain(spectrumValue[i], FilterValue[i], 1023);
 spectrumValue[i] = map(spectrumValue[i], FilterValue[i], 1023, 0, 255);
 digitalWrite(strobePin, HIGH);
 }
 int low = 0;
 int mid = 0;
 int high = 0;

data["spec0"] = spectrumValue[0]; 
data["spec1"] = spectrumValue[1];
data["spec2"] = spectrumValue[2];
data["spec3"] = spectrumValue[3];
data["spec4"] = spectrumValue[4];
data["spec5"] = spectrumValue[5];
data["spec6"] = spectrumValue[6];


data.printTo(buffer, sizeof(buffer));



 webSocket.sendTXT(buffer);






 
}
