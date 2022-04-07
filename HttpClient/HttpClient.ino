#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define SERVER_IP "http://arduino.vhealth.me/Powers/"

#ifndef STASSID
#define STASSID "iPhone - Narcis"
#define STAPSK  "12345678"
#endif
int nr=0;
void setup() {

  Serial.begin(9600);

  Serial.println();
  Serial.println();
  Serial.println();

  WiFi.begin(STASSID, STAPSK);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());

}

void loop() {
  
  // wait for WiFi connection
  if ((WiFi.status() == WL_CONNECTED)) {

     WiFiClient client;
    HTTPClient http;

    Serial.print("[HTTP] begin...\n");
    // configure traged server and url
    http.begin(client, SERVER_IP); //HTTP
    http.addHeader("Content-Type", "application/json");
    nr = nr + 1;
    Serial.print("[HTTP] POST...\n");
    // start connection and send HTTP header and body
    String post = "{\"wh\": "+String(nr)+"}";
    int httpCode = http.PUT(post);
    
    // httpCode will be negative on error
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] POST... code: %d\n", httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK) {
        const String& payload = http.getString();
        Serial.println("received payload:\n<<");
        Serial.println(payload);
        Serial.println(">>");
      }
    } else {
      Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  }

  delay(1000);
}
