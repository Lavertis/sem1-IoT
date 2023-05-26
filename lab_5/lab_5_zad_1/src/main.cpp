// /*
//   Blink

//   Turns an LED on for one second, then off for one second, repeatedly.

//   Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
//   it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
//   the correct LED pin independent of which board is used.
//   If you want to know what pin the on-board LED is connected to on your Arduino
//   model, check the Technical Specs of your board at:
//   https://www.arduino.cc/en/Main/Products

//   modified 8 May 2014
//   by Scott Fitzgerald
//   modified 2 Sep 2016
//   by Arturo Guadalupi
//   modified 8 Sep 2016
//   by Colby Newman

//   This example code is in the public domain.

//   https://www.arduino.cc/en/Tutorial/BuiltInExamples/Blink
// */

// #include <Arduino.h>
// #include <DS18B20.h>
// #define LED 16

// DS18B20 ds(14);

// // the setup function runs once when you press reset or power the board
// void setup()
// {
//   // initialize digital pin LED_BUILTIN as an output.
//   pinMode(LED, OUTPUT);
// }

// // the loop function runs over and over again forever
// void loop()
// {
//   digitalWrite(LED, HIGH); // turn the LED on (HIGH is the voltage level)
//   delay(1000);             // wait for a second
//   digitalWrite(LED, LOW);  // turn the LED off by making the voltage LOW
//   delay(1000);             // wait for a second
//   while (ds.selectNext())
//   {
//     Serial.println(ds.getTempC());
//   }
// }

/*
  WiFiAccessPoint.ino creates a WiFi access point and provides a web server on it.

  Steps:
  1. Connect to the access point "yourAp"
  2. Point your web browser to http://192.168.4.1/H to turn the LED on or http://192.168.4.1/L to turn it off
     OR
     Run raw TCP "GET /H" and "GET /L" on PuTTY terminal with 192.168.4.1 as IP address and 80 as port

  Created for arduino-esp32 on 04 July, 2018
  by Elochukwu Ifediora (fedy0)
*/

#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>

#define LED_GREEN 16 // Set the GPIO pin where you connected your test LED or comment this line out if your dev board has a built-in LED

// Set these to your desired credentials.
const char *ssid = "AndroidAP7741";
const char *password = "1234567890";

#define ledPin 17 // GPI17
#define ledPin2 16
#define dsPin 16
#include <DS18B20.h>
DS18B20 ds(dsPin);
float temp = -100;
#include <WiFi.h>

WiFiServer server(80);
String header;
String ledstate = "off";
String ledstate2 = "off";

void setup(void)
{
  Serial.begin(115200);
  delay(1000);
  Serial.println();
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  pinMode(ledPin2, OUTPUT);
  digitalWrite(ledPin2, HIGH);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  int i = 0;
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    i = i + 1;
    if (i % 2 == 0)
    {
      Serial.print(".");
    }
    else
    {
      Serial.print("+");
    }
  }
  Serial.println();
  Serial.println("WiFi connected");
  server.begin(); // Start the server
  Serial.println("Server started");
  Serial.print("Use this URL : ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
}

void loop(void)
{
  WiFiClient client = server.available(); // Listen for incoming clients
  if (client)
  {                          // If a new client connects,
    String currentLine = ""; // make a String to hold incoming data from the client
    while (client.connected())
    { // loop while the client's connected
      if (client.available())
      {                         // if there's bytes to read from the client,
        char c = client.read(); // read a byte, then
        Serial.write(c);        // print it out the serial monitor
        header += c;
        if (c == '\n')
        { // if the byte is a newline character
          if (currentLine.length() == 0)
          { // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            Serial.print("Temperature: ");
            temp = ds.getTempC();
            Serial.print(temp);
            Serial.println(" C ");
            if (header.indexOf("GET /led/1/on") >= 0)
            {
              Serial.println("LED on");
              ledstate = "on";
              digitalWrite(ledPin, HIGH);
            }
            else if (header.indexOf("GET /led/1/off") >= 0)
            {
              Serial.println("LED off");
              ledstate = "off";
              digitalWrite(ledPin, LOW);
            }

            if (header.indexOf("GET /led/2/on") >= 0)
            {
              Serial.println("LED on");
              ledstate2 = "on";
              digitalWrite(ledPin2, HIGH);
            }
            else if (header.indexOf("GET /led/2/off") >= 0)
            {
              Serial.println("LED off");
              ledstate2 = "off";
              digitalWrite(ledPin2, LOW);
            }
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\" />");
            client.println("<meta http-equiv=\"refresh\" content=\"10\" />");
            client.println("<link rel=\"icon\" href=\"data:,\" />");
            // CSS to style the on/off buttons
            // Feel free to change the background-color and font-size attributes
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #07878A; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color:#8a0787;}</style></head>");
            // Web Page Heading
            client.println("<body><h1>ESP32 temperature control</h1>");
            // Display current state, and ON/OFF buttons for GPIO
            client.println("<p>Temperature: " + String(temp) + " </p>");
            // If the LED is off, it displays the ON button

            if (ledstate == "off")
            {
              client.println("<p><a href=\"/led/1/on\"><button class=\"button\">ON</button></a></p>");
            }
            else
            {
              client.println("<p><a href=\"/led/1/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
            if (ledstate2 == "off")
            {
              client.println("<p><a href=\"/led/2/on\"><button class=\"button\">ON</button></a></p>");
            }
            else
            {
              client.println("<p><a href=\"/led/2/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
            client.println("</body></html>"); // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          }
          else
          {
            currentLine = ""; // if you got a newline, then clear currentLine
          }
        }
        else if (c != '\r')
        {                   // if you got anything else but a carriage return character,
          currentLine += c; // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println();
  }
}