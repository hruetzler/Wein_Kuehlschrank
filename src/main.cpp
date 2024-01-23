#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include <weinWebserver.h>
#include <Keypad.h>

JsonDocument zustand;
JsonDocument details;
WeinWebserver* server = nullptr;
bool zustandSchloss = false;

const int LED = D8;

String code = "";
const byte COLS = 4; //4 Spalten
const byte ROWS = 4; //4 Zeilen

char hexaKeys[ROWS][COLS]={
{'1','2','3','A'},
{'4','5','6','B'},
{'7','8','9','C'},
{'*','0','#','D'}
};

byte colPins[COLS] = {D4,D5,D6,D7}; //Definition der Pins für die 3 Spalten
byte rowPins[ROWS] = {D0,D1,D2,D3}; //Definition der Pins für die 4 Zeilen
char Taste;

Keypad Tastenfeld = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

void codeEingabe();

void setup() {
  Serial.begin(115200);
  if(!LittleFS.begin()){
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }

  File f = LittleFS.open("/wlanConfig.txt", "r");

   if (!f) {
    Serial.println("Datei konnte nicht geöffnet werden");
    return;
  }

  String ssid = f.readStringUntil('\n');
  String password = f.readStringUntil('\n');

  f.close();
  
  WiFi.begin(ssid, password);
  Serial.print("Connection to Wifi . . .");
  while (WiFi.status() != WL_CONNECTED )
  {
    delay(500);
    Serial.print(" .");
  }
  Serial.println();
  Serial.println(WiFi.localIP());


  server = new WeinWebserver(&LittleFS);

  pinMode(LED, OUTPUT);
  // digitalWrite(LED, HIGH);


  }

void loop() {
  zustandSchloss =  server->getZustand();
  digitalWrite(LED, zustandSchloss);

  codeEingabe();

  
  

  
}

void codeEingabe(){

  Taste = Tastenfeld.getKey();

  if (Taste)
  {
    if (Taste =='A')
    {
      code = "";
      Serial.println("Resetet");
      return;
    }
    if(Taste == 'D'){
      Serial.println(code);
      Serial.println("Bestätigt");
      server->checkCode(code);
      code = "";
      return;
    }
    code = code + Taste;
    Serial.println(code);

    // Serial.println(Taste);
  }
}

