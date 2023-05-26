#include <Arduino.h>
#include <DS18B20.h>
#include <WiFi.h>

#define ledPin 32 // GPI17
#define ledPin2 33
#define dsPin 14 // GPI14

DS18B20 ds(dsPin);
float temp = -100;
float temp2 = -100;

const char *ssid = ""; // Your ssid
const char *password = "";  // Your Password
WiFiServer server(80);

String header;
String ledstate1 = "off";
String ledstate2 = "off";

void setup(void)
{
  Serial.begin(115200);
  delay(1000);
  Serial.println();
  pinMode(ledPin, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  digitalWrite(ledPin, LOW);
  digitalWrite(ledPin2, LOW);
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
            ds.selectNext();
            temp2 = ds.getTempC();
            Serial.print(temp);
            Serial.println(" C ");

            if (header.indexOf("GET /led1/on") >= 0)
            {
              Serial.println("LED on");
              ledstate1 = "on";
              digitalWrite(ledPin, HIGH);
            }
            else if (header.indexOf("GET /led1/off") >= 0)
            {
              Serial.println("LED1 off");
              ledstate1 = "off";
              digitalWrite(ledPin, LOW);
            }

            if (header.indexOf("GET /led2/on") >= 0)
            {
              Serial.println("LED2 on");
              ledstate2 = "on";
              digitalWrite(ledPin2, HIGH);
            }
            else if (header.indexOf("GET /led2/off") >= 0)
            {
              Serial.println("LED2 off");
              ledstate2 = "off";
              digitalWrite(ledPin2, LOW);
            }
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\" />");
            client.println("<meta http-equiv=\"refresh\" content=\"2\" />");
            client.println("<link rel=\"icon\" href=\"data:,\" />");
            // CSS to style the on/off buttons
            // Feel free to change the background-color and font-size attributes
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #07878A; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #8a0787;}</style></head>");
            // Web Page Heading
            client.println("<body><h1>ESP32 temperature control</h1>");
            // Display current state, and ON/OFF buttons for GPIO
            client.println("<p>Temperature 1: " + String(temp) + " </p>");
            // If the LED is off, it displays the ON button
            if (ledstate1 == "off")
            {
              client.println("<p><a href=\"/led1/on\"><button class=\"button\">ON</button></a></p>");
            }
            else
            {
              client.println("<p><a href=\"/led1/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
            client.println("<p>Temperature2 : " + String(temp2) + " </p>");
            if (ledstate2 == "off")
            {
              client.println("<p><a href=\"/led2/on\"><button class=\"button\">ON</button></a></p>");
            }
            else
            {
              client.println("<p><a href=\"/led2/off\"><button class=\"button button2\">OFF</button></a></p>");
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
