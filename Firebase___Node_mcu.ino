//
// Copyright 2015 Google Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

// FirebaseDemo_ESP8266 is a sample that demo the different functions
// of the FirebaseArduino API.

#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

// Set these to run example.
#define FIREBASE_HOST "------"
#define FIREBASE_AUTH "------"
#define WIFI_SSID "-------"
#define WIFI_PASSWORD "------"
String LEDStatus = "";

void setup() {

pinMode(LED_BUILTIN,OUTPUT);

digitalWrite(LED_BUILTIN,LOW);

Serial.begin(9600);

WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

Serial.print("connecting");

while (WiFi.status() != WL_CONNECTED) {

Serial.print(".");

delay(500);

}

Serial.println();

Serial.print("connected: ");

Serial.println(WiFi.localIP());

Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

//Firebase.setString("TestGP/box1/BoxNo","\"2103\"");
//Firebase.setString("TestGP/box1/City","\"Nablus\"");
//Firebase.setString("TestGP/box1/Weight","\"115.6\"");
//Firebase.setString("TestGP/box1/PersonId","\"556\"");
//Firebase.setInt("TestGP/box1/1_RoofNo",1);
}
void setFirebase(int i)
{
  String temp = "TestGP/box_" + String(i) + "/" ;
  Firebase.setString(temp + "BoxNo","2103");
  Firebase.setString(temp + "City","Nablus");
  Firebase.setString(temp + "Weight","115.6");
  Firebase.setString(temp + "PersonId","556");
  Firebase.setBool(temp + "Empty",false);
  Firebase.setBool(temp + "Open",false);
  Firebase.setInt(temp + "RoofNo",i);
}
int i =1;
bool empty = false;
void loop() {
i=i+1;
if(i>4)
i=1;
setFirebase(i);
//empty = !empty; 
//Firebase.setBool("TestGP/box1/1_Empty",empty);
LEDStatus = Firebase.getString("Sorter/LEDStatus");
LEDStatus.replace("\"", "");
Serial.println(LEDStatus.toInt());
if(LEDStatus.toInt() == 1)

{

digitalWrite(LED_BUILTIN,HIGH);

}

else

{

digitalWrite(LED_BUILTIN,LOW);

}

if (Firebase.failed()) // Check for errors 
{

Serial.print("setting /number failed:");

Serial.println(Firebase.error());
delay(1000);
return;

}

delay(1000);

}


