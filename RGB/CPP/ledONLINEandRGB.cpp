/*
 * ----------------------------------------------
 * Project/Program Name : led on off based on ONLINE text file 
 * File Name            : <ledONLINEandRGB.cpp>
 * Author               : Isaac Rodriguez
 * Date                 : Nov 9, 2025
 * Version              : V1
 * 
 * Purpose:
 *   This program uses a ESP8266, 2 buttons, 1 led and 1 rgb led to check the contents of a text file online. Based on
 *  the contents the led will turn off or on and the rgb led will change in intensity. If both buttons are pressed at 
 *   the same time. It will send random text data to my google charts.
 * 
 * Inputs:
 *   2 button
 * 
 * Outputs:
 *    WiFi signal to txt file and 1 led, 1 RGB led.
 * 
 * Example Application:
 *    In this application. This program is used to control an LED and RGB LED through external means.
 * 
 * 
 * ---------------------------------------------------------------------------
 */

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>


const char* ssid     = "ffffvvfvfvfv"; 
const char* password = "fvfvfvvfvfvf";

// const char* ssid     = "fgfgfgfgfgfg"; 
// const char* password = "bfbfbfbfbfbbf";

const char* url_led = "https://isaacr.org/api_assignment/ledControl/status.txt";
const char* url_rgb1 = "https://isaacr.org/api_assignment/rgbControl/status.txt";
String googleURL = "https://script.google.com/macros/s/AKfycbxog470SnfhovbMcX28FfwMlB8b85E-sVAT8ydaqWPSeYlqSej2aKP8dxrop0kBmuKQyA/exec?";
String timeURL = "https://timeapi.io/api/time/current/zone?timeZone=America%2FLosAngeles"; // America/Los_Angeles"; 
// 'https://timeapi.io/api/time/current/zone?timeZone='


struct Button {
    const uint8_t PIN;
    const uint8_t LED;
    bool oneButton;
    bool twoButton;
};

Button button1 = {D5, D7, false, false};
Button button2 = {D1, D8, false, false};

//variables to keep track of the timing of recent interrupts
unsigned long button_time = 0;  
unsigned long last_button_time = 0; 
bool loopFlag = false;

void ICACHE_RAM_ATTR isr() 
{
  button_time = millis();
  if (button_time - last_button_time > 250)
  {
    if (digitalRead(button1.PIN) == HIGH)
      { 
          digitalWrite(button1.LED, HIGH);
          loopFlag = false;
          int Later = millis() + 150;
          while (Later > millis()) 
            {
              if (digitalRead(button2.PIN) == HIGH)
              {
                digitalWrite(button2.LED, HIGH);
                loopFlag = true;
                button1.twoButton = true;
                button2.twoButton = true;
              }
            }
          if (loopFlag == false)
            {
                button1.oneButton = true;
            }
          digitalWrite(button1.LED, LOW);
          digitalWrite(button2.LED, LOW);
      }
    else if (digitalRead(button2.PIN) == HIGH)
      {
          digitalWrite(button2.LED, HIGH);
          loopFlag = false;
          int Later = millis() + 150;
          while (Later > millis()) 
            {
              if (digitalRead(button1.PIN) == HIGH)
              {
                digitalWrite(button1.LED, HIGH);
                loopFlag = true;
                button1.twoButton = true;
                button2.twoButton = true;
              }
            }
          if (loopFlag == false)
            {
                button2.oneButton = true;
            }
          digitalWrite(button1.LED, LOW);
          digitalWrite(button2.LED, LOW);
      }
      last_button_time = button_time;
  }
}


int RGB1 = 4;
int RGB1_val = 0;
int RGB2 = 0;
int RGB2_val = 0;
int RGB3 = 2;
int RGB3_val = 0;
int LED1 = 12;
int LED1_val = LOW;

unsigned char State;  
int counter = 0;

void setup() 
{
  Serial.begin(9600);
  pinMode(button1.PIN, INPUT);    
  pinMode(button2.PIN, INPUT);
  attachInterrupt(button2.PIN, isr, RISING);
  attachInterrupt(button1.PIN, isr, RISING);
  pinMode(RGB1, OUTPUT);
  pinMode(RGB2, OUTPUT);
  pinMode(RGB3, OUTPUT); 
  pinMode(LED1, OUTPUT);
  pinMode(button1.LED, OUTPUT);
  pinMode(button2.LED, OUTPUT);

  String thisBoard= ARDUINO_BOARD;
  analogWriteRange(255);

  WiFi.mode(WIFI_STA); // SETS TO STATION MODE!
  //WiFi.disconnect();
  Serial.println("");
  WiFi.hostname("ESP-host");
  delay(100);
  Serial.println("Connecting to WiFi");
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) 
    {
      delay(500);
      Serial.print(".+.");
    }
  Serial.println("");
  Serial.println(WiFi.getHostname());
}

void loop() 
{ 
  if (WiFi.status() == WL_CONNECTED)
  {
    counter = counter + 1;
    if ((counter % 50000) == 0)
      {
        Serial.print(".-.");
        counter = 0;
      }
    // delay(100);

    if (button1.oneButton == true)
      {
        Serial.println("Button #1 clicked");
        analogWrite(button1.LED, HIGH);
        Serial.println("Sending LED Request");
        HTTPClient http;
        WiFiClientSecure client;
        client.setInsecure();
        http.begin(client, url_led);
        int httpCode = http.GET();
        analogWrite(button1.LED, LOW);
        if (httpCode > 0) 
          { // Check for successful request
            String payload = http.getString();
            Serial.println("--- Text File Content ---");
            Serial.println(payload);
            int stringLength = payload.length();
            if (stringLength >= 3) 
              {
                String lastThreeChars = payload.substring(stringLength - 3);
                
                if (lastThreeChars[0] == 'n')
                {
                  digitalWrite(LED1, HIGH); 
                }
                if (lastThreeChars[0] == 'f')
                {
                  digitalWrite(LED1, LOW); 
                }

              } 
          }
        else 
          {
            Serial.print("Error on HTTP request: ");
            Serial.println(httpCode);
          }
        http.end(); // Free resources
        button1.oneButton = false;
      }

    else if (button2.oneButton == true)
      {
        Serial.println("Button #2 clicked");
        analogWrite(button2.LED, HIGH);
        // RGB request
        Serial.println("Sending RGB Request");
        HTTPClient http;
        WiFiClientSecure client;
        client.setInsecure();
        http.begin(client, url_rgb1);
        int httpCode = http.GET();
        analogWrite(button2.LED, LOW);
        if (httpCode > 0) 
          { // Check for successful request
            String payload = http.getString();
            payload.replace("\\", "");
            Serial.println(payload);
            StaticJsonDocument<700> doc;
            // Deserialize the JSON document
            DeserializationError error = deserializeJson(doc, payload);
            if (error) 
              {
                Serial.print(F("deserializeJson() failed: "));
                Serial.println(error.f_str());
                return;
              }
            JsonObject nodes = doc["Node1"]; 
            RGB1_val = nodes["RGB1"];
            RGB2_val = nodes["RGB2"];
            RGB3_val = nodes["RGB3"];
            // Serial.println(RGB1_val);
          }
        else 
          {
            Serial.print("Error on HTTP request: ");
            Serial.println(httpCode);
          }
        http.end(); // Free resources
        button2.oneButton = false;
      }

    else if ((button1.twoButton == true) & (button2.twoButton == true))
      {
        Serial.println("two buttons clicked");
        String googleURL = "https://script.google.com/macros/s/AKfycbxog470SnfhovbMcX28FfwMlB8b85E-sVAT8ydaqWPSeYlqSej2aKP8dxrop0kBmuKQyA/exec?";
        // timeio did not work for me for this project. Instead I would upload random datetime data.
        
        // WiFiClientSecure client;
        // // client.setInsecure();
        // HTTPClient https;
        // // https.setConnectTimeout(5000); // 5 seconds in milliseconds
        // // Set the request timeout (how long to wait for a response after connection)
        // https.setTimeout(40000); 
        // // READ TIME
        // Serial.println("Requesting Time: -->" + timeURL);
        // if (https.begin(client, timeURL))
        // {
        
        //   int httpCode = https.GET(); 
        //   delay(750);
        //   // int httpCode = https.POST(fullUrl); // for post comms

        //   Serial.println("Response code <--: " + String(httpCode));
        //   if (httpCode > 0)
        //     {
        //       String dateTime = https.getString();

        //       StaticJsonDocument<200> doc;
        //       DeserializationError error = deserializeJson(doc, dateTime);
        //       if (error) 
        //         {
        //           Serial.print(F("deserializeJson() failed: "));
        //           Serial.println(error.f_str());
        //         }
        //       String hour = doc["hour"];
        //       String minute = doc["minute"];
        //       String second = doc["seconds"];
        //       String day = doc["day"];
        //       String month = doc["month"];
        //       String year = doc["year"];
        //       if (second.length() < 2){
        //         second = "0"+second;
        //       }
        //       if (minute.length() < 2){
        //         minute = "0"+minute;
        //       }
        //       if (hour.length() < 2){
        //         hour = "0"+hour;
        //       }
        //       if (day.length() < 2){
        //         day = "0"+day;
        //       } 
        //       if (month.length() < 2){
        //         month = "0"+month;
        //       }        
        //       Serial.println("");
        //       Serial.println("today is: " + month + "/" + day + "/" + year);
        //       Serial.println("the time is: " + hour + ":" + minute + ":" + second);
        //       String db_datetime = year + "-" + month + "-" + day + "+" + hour + "%3A" + minute + "%3A" + second;
        //     }
        //    https.end();
        // }
        
        int sensorData = random(1, 101);
        String randSecond = String(random(1, 60));
        String randMinute = String(random(1, 60));
        if (randSecond.length() < 2){
          randSecond = "0"+randSecond;
        }
        if (randMinute.length() < 2){
          randMinute = "0"+randMinute;
        }

        String data = "sensor=" + String(sensorData) + "&date=2025-11-14%2020:" + randMinute + ":" + randSecond ;
        // 90&date=2025-11-01%2020:00:00";
        googleURL = googleURL + data;
        HTTPClient http;
        WiFiClientSecure client;
        client.setInsecure();
        http.begin(client, googleURL);
        int httpCode = http.GET();
        analogWrite(button1.LED, LOW);
        if (httpCode > 0) 
          { // Check for successful request
            String payload = http.getString();
            Serial.println(payload);
          }
        else 
          {
            Serial.print("Error on HTTP request: ");
            Serial.println(httpCode);
          }
        http.end(); // Free resources
      
        button1.twoButton = false;
        button2.twoButton = false;
      }

    else
    {
    
        // analogWrite(RGB1, RGB1_val);
        // analogWrite(RGB2, RGB2_val);
        // analogWrite(RGB3, RGB3_val);
        // digitalWrite(LED1, LED1_val); 
    }

  }
}

