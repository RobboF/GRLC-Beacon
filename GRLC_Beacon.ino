#include <ArduinoJson.h>
#include "WiFi.h"
#include <HTTPClient.h>


const char* apiURL="https://api.nomics.com/v1/currencies/ticker?ids=GRLC&key=<API key goes here>";

void initWiFi() {
  const char* ssid = "<SSID goes here>";
  const char* password = "<Password goes here>";
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}

void setup(){
  Serial.begin(115200);
  initWiFi(); 
}
void loop(){
  if(WiFi.status()== WL_CONNECTED){
      HTTPClient http;
      http.begin(apiURL);
      int httpResponseCode = http.GET();
      if (httpResponseCode>0) {
        String payload = http.getString();
        StaticJsonDocument<2048> doc;
        DeserializationError error = deserializeJson(doc, payload);

        if (error) {
          Serial.print(F("deserializeJson() failed: "));
          Serial.println(error.f_str());
          return;
        }
        JsonObject root = doc[0];
        float price = root["price"];
        Serial.println(price, 4);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }

  }
  delay(10000);
}
