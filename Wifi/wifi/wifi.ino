#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Wire.h>
#include <Adafruit_INA219.h>
#include <ArduinoJson.h>
#define SERVER_IP "http://arduino.vhealth.me/Powers/live"
#define SERVER_WAYPOINTS "http://arduino.vhealth.me/Waypoints"

#ifndef STASSID
#define STASSID "iPhone - Narcis"
#define STAPSK  "12345678"
#endif
Adafruit_INA219 ina219;
float energy = 0;
float shuntvoltage = 0;
float busvoltage = 0;
float current_mA = 0;
float loadvoltage =0;

unsigned long timePUT = 0;
unsigned long timeWay = 0; 

void setup() {

  Serial.begin(9600);
  if (! ina219.begin()) {
    //Serial.println("Failed to find INA219 chip");
    while (1) { delay(10); }
  }

  //Serial.println("Measuring voltage and current with INA219 ...");
  
  

  //Serial.println();
  //Serial.println();
  //Serial.println();

  WiFi.begin(STASSID, STAPSK);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    //Serial.print(".");
  }
  //Serial.println("");
  //Serial.print("Connected! IP address: ");
  //Serial.println(WiFi.localIP());

}

void loop() {
  
  // wait for WiFi connection
  if ((WiFi.status() == WL_CONNECTED)) {
    WiFiClient client;
    HTTPClient http;
    int httpCode = 0;
    if(millis() - timeWay >= 5000 ){
      timeWay = millis();
      http.begin(client, SERVER_WAYPOINTS); //HTTP
      http.addHeader("Content-Type", "application/json");
      httpCode = http.GET();
      String payloadWaypoints = "";
      if (httpCode == HTTP_CODE_OK) {
          payloadWaypoints = http.getString();
          //Serial.println("received payload:\n<<");
          //Serial.println(payload);
          //Serial.println(">>");
       }
       StaticJsonDocument<200> doc;
       char json[50];
       payloadWaypoints.toCharArray(json, 50);
       DeserializationError error = deserializeJson(doc, json);
  
      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
      }
      int lr1 = doc[0]["lr"];
      int lr2 = doc[1]["lr"];
      int td1 = doc[0]["td"];
      int td2 = doc[1]["td"];
  
      Serial. println((String)lr1 + " " + (String)lr2 + " " + (String) td1 + "  " + (String)td2);
//      String way = (String)lr1 +";"+(String)lr2 +";"+(String)td1+";"+(String)td2;
//      char wayChar[50];
//      way.toCharArray(wayChar,50);
//      Serial.write(wayChar, 50);
    }
    
    //=====================================================================
    if(millis() - timePUT >= 1000){
      timePUT = millis();
      shuntvoltage = ina219.getShuntVoltage_mV();
      busvoltage = ina219.getBusVoltage_V();
      current_mA = ina219.getCurrent_mA();
      loadvoltage = busvoltage + (shuntvoltage / 1000);
      energy = energy + loadvoltage * current_mA / 3600;
    

      //Serial.print("[HTTP] begin...\n");
      // configure traged server and url
      http.begin(client, SERVER_IP); //HTTP
      http.addHeader("Content-Type", "application/json");
      //Serial.print("[HTTP] POST...\n");
      // start connection and send HTTP header and body
      String post = "{\n  \"livePower\": "+(String)(ina219.getPower_mW()/1000)+",\n  \"totalPower\": "+(String)(energy)+"\n}";
      //Serial.println(post);
      //String post = "{\"wh\": "+(String)ina219.getPower_mW()+"}";
      httpCode = http.PUT(post);
      
      // httpCode will be negative on error
      if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        //Serial.printf("[HTTP] POST... code: %d\n", httpCode);
  
        // file found at server
        if (httpCode == HTTP_CODE_OK) {
          const String& payload = http.getString();
          //Serial.println("received payload:\n<<");
          //Serial.println(payload);
          //Serial.println(">>");
        }
      } else {
        //Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }
  
      http.end();
    }
  }
}
