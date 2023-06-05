#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <DS18B20.h>

WebServer webServer(80);

float dataT[1000] = {0.0};
String sOut = "";
int autorefresh = 10;
int ile = 0;
float bmpT = 0.0;

#define dsPin 14 // GPI14

DS18B20 ds(dsPin);

const char *ssid = "AndroidAP7741";  // Your ssid
const char *password = "1234567890"; // Your Password

void PrintWykres(int dataLen, int color); // funkcja rysująca wykres, dane z dataT
void handleRoot();                        // funkcja generująca stronę w formacie html
void handleNotFound()
{
  webServer.send(200, "text/html", "Not found");
}

void setup(void)
{
  Serial.begin(115200);

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
  Serial.println();
  Serial.println("WiFi connected");
  Serial.println("Server started");
  Serial.print("Use this URL : ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");

  webServer.on("/", handleRoot);
  webServer.onNotFound(handleNotFound);
  webServer.begin();
  Serial.println("HTTP server started");
}

unsigned long time_ = 0;

void loop(void)
{
  webServer.handleClient();
  delay(2); // allow the cpu to switch to other tasks
  // odczyt zegara, BME, zapis na karcie
  if (ile <= 400)
  {
    if (millis() - time_ > 2000)
    {
      ile += 1;
      ds.setResolution(12);
      bmpT = ds.getTempC();
      dataT[ile] = bmpT;
      Serial.println(bmpT);
      time_ = millis();
    }
  }
  else
  {
    ile = 0;
  }
}

void handleRoot()
{
  int sec = millis() / 1000;
  int mi = sec / 60;
  int hr = mi / 60;
  char tempS[100];
  sOut = "<html><head>\
<meta http-equiv='refresh' content='";
  sOut += String(autorefresh) + "'/>";
  sOut += "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />\
<meta http-equiv=\"Content-Language\" content=\"pl\" />\
<title>ESP32</title>\
<style> html{font-family:Verdana;display:inline-block;background-color:#FFF;}\
.font_t{color:white;text-align:center;text-decoration:none;color:#111; font-weight:bold;padding:8px;margin:8px;}\
.line_t{border-top:3pxsolid;border-image: linear-gradient(toleft,#111,#E00,#111);border-image-slice:1;}\
.graph_t{text-align:center;}\
</style> </head><body>\
<h1 class=\"font_t\">ESP32 laboratorium</h1>";

  sOut += "<hr class=\"line_t\"><h1 class=\"font_t\">Temperatura = ";
  sOut += String(bmpT) + " C";
  sOut += "</h1><br><div class=\"graph_t\">";
  PrintWykres(ile, 1);
  sOut += "</div>";
  sOut += "</body></html>";
  webServer.send(200, "text/html", sOut);
}

void PrintWykres(int dataLen, int color)
{
  char tempS[100];
  sOut += "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\"\
width=\"450\" height=\"220\">\n"; // increase height to accomodate x-axis labels
  sOut += "<rect x=\"50\" y=\"20\" width=\"350\" height=\"150\" fill=\"rgb(200, 200, 200)\
\" stroke-width=\"1\" stroke=\"rgb(255, 255, 255)\" />\n";

  // Add axes
  sOut += "<line x1=\"50\" y1=\"170\" x2=\"400\" y2=\"170\" stroke=\"black\" stroke-width=\"2\" />\n"; // X-axis
  sOut += "<line x1=\"50\" y1=\"20\" x2=\"50\" y2=\"170\" stroke=\"black\" stroke-width=\"2\" />\n";   // Y-axis

  if (color == 1)
    sOut += "<g stroke=\"red\">\n";
  if (color == 2)
    sOut += "<g stroke=\"blue\">\n";
  if (color == 3)
    sOut += "<g stroke=\"yellow\">\n";

  float miny = 1000;
  float maxy = -1000;
  float dx, skala;
  int newX, x, y, newY;
  for (int i = 1; i < dataLen; i++)
  {
    if (dataT[i] > maxy)
      maxy = dataT[i];
    if (dataT[i] < miny)
      miny = dataT[i];
  }
  dx = abs(maxy - miny);
  if (dx > 0)
    skala = 120 / dx;
  else
    skala = 75;
  y = int((dataT[1] - miny) * skala);
  x = 1;

  for (int i = 2; i < dataLen; i += 1)
  {
    newY = int((dataT[i] - miny) * skala);
    newX = x + 1;
    sprintf(tempS, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" \
stroke-width=\"2\" />\n",
            x + 50, 170 - y, newX + 50, 170 - newY);
    sOut += tempS;
    y = newY;
    x = newX;
  }
  sOut += "</g>\n";

  // Add Y-axis labels and tick marks
  // sprintf(tempS, "<text x=\"0\" y=\"25\" font-family=\"Verdana\" font-size=\"15\">%.2f</text>\n", maxy);
  // sOut += tempS;
  // sprintf(tempS, "<text x=\"0\" y=\"175\" font-family=\"Verdana\" font-size=\"15\">%.2f</text>\n", miny);
  // sOut += tempS;
  sOut += "<line x1=\"45\" y1=\"170\" x2=\"50\" y2=\"170\" stroke=\"black\" stroke-width=\"2\" />\n"; // Y min tick
  // sOut += "<line x1=\"45\" y1=\"20\" x2=\"50\" y2=\"20\" stroke=\"black\" stroke-width=\"2\" />\n";   // Y max tick

  float ySpacing = 1.0; // you can change this value to change the spacing of y-axis ticks and labels

  // Add Y-axis tick marks
  float yStart = ceil(miny / ySpacing) * ySpacing; // starting point for y-axis labels and ticks, rounded up to nearest ySpacing
  for (float y = miny; y <= maxy + ySpacing; y += ySpacing)
  {
    int yPixel = 170 - int((y - miny) * skala);
    sprintf(tempS, "<text x=\"0\" y=\"%d\" font-family=\"Verdana\" font-size=\"10\">%.1f</text>\n", yPixel + 5, y);
    sOut += tempS;
    sprintf(tempS, "<line x1=\"45\" y1=\"%d\" x2=\"50\" y2=\"%d\" stroke=\"black\" stroke-width=\"2\" />\n", yPixel, yPixel); // Y tick
    sOut += tempS;
  }

  int xOffset = 49;

  // Add X-axis labels and tick marks
  for (int i = 0; i <= dataLen; i += 15) // assuming the readings are sufficiently large
  {
    sprintf(tempS, "<text x=\"%d\" y=\"200\" font-family=\"Verdana\" font-size=\"10\">%d</text>\n", xOffset + i, i);
    sOut += tempS;
    sprintf(tempS, "<line x1=\"%d\" y1=\"170\" x2=\"%d\" y2=\"175\" stroke=\"black\" stroke-width=\"2\" />\n", xOffset + i, xOffset + i); // X tick
    sOut += tempS;
  }

  // Add axis labels
  sOut += "<text x=\"46\" y=\"10\" font-family=\"Verdana\" font-size=\"15\">T</text>\n";   // Y-axis label
  sOut += "<text x=\"410\" y=\"175\" font-family=\"Verdana\" font-size=\"15\">n</text>\n"; // X-axis label

  sOut += "</svg>\n";
}
