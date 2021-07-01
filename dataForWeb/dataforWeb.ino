/*
 * Author Name: Neelabh Dubey
 * Team Name: ProblemSolver
 * Filename: DataforWeb
 * 
 * Description: Data uploading on server for our website
 */


//FirebaseESP8266.h must be included before ESP8266WiFi.h
#include "FirebaseESP8266.h"
#include <ESP8266WiFi.h>

#define WIFI_SSID "G"
#define WIFI_PASSWORD "Divya02413"
#define FIREBASE_HOST "gsmtest-6139a.firebaseio.com" //Do not include https:// in FIREBASE_HOST
#define FIREBASE_AUTH "1NaCoIwrmTXHv5x8s2JRMxC6zRMIiJ6VOlHQROwo"

//Define Firebase Data object
FirebaseData firebaseData;

void setup()
{

  Serial.begin(115200);
  Serial.println();
  Serial.println();

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

  String rules = "";

  Serial.println("------------------------------------");
  Serial.println("Read Database Rules test...");

  if (Firebase.getRules(firebaseData))
  {
    Serial.println("PASSED");
    Serial.println("DATABASE RULES: ");

    FirebaseJson &json = firebaseData.jsonObject();
    json.toString(rules, true);
    Serial.println(rules);

    Serial.println();
    Serial.println("------------------------------------");
    Serial.println("PARSE: ");

    size_t len = json.iteratorBegin();
    String key, value = "";
    int type = 0;
    for (size_t i = 0; i < len; i++)
    {
      json.iteratorGet(i, type, key, value);
      Serial.print(i);
      Serial.print(", ");
      Serial.print("Type: ");
      Serial.print(type == JSON_OBJECT ? "object" : "array");
      if (type == JSON_OBJECT)
      {
        Serial.print(", Key: ");
        Serial.print(key);
      }
      Serial.print(", Value: ");
      Serial.println(value);
    }

    json.iteratorEnd();
    Serial.println();
  }
  else
  {
    Serial.println("FAILED");
    Serial.println("REASON: " + firebaseData.errorReason());
    Serial.println("------------------------------------");
    Serial.println();
  }


  Serial.println("------------------------------------");
  Serial.println("Write Database Rules test...");

  if (Firebase.setRules(firebaseData, rules))
  {
    Serial.println("PASSED");
    Serial.println("------------------------------------");
    Serial.println();
  }
  else
  {
    Serial.println("FAILED");
    Serial.println("REASON: " + firebaseData.errorReason());
    Serial.println("------------------------------------");
    Serial.println();
  }
}

void loop()
{
   server.update();
   //contant updation of data on firebase through wifi
}
