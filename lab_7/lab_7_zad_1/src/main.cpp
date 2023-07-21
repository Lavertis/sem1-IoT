#include <Arduino.h>

#define seconds 5
#define LED 27

RTC_DATA_ATTR int bootCount = 0;

void print_wakeup_reason();

void setup()
{
  Serial.begin(115200);
  delay(1000);
  pinMode(LED, OUTPUT);

  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, HIGH);

  ++bootCount;
  Serial.println("Boot number: " + String(bootCount));
  print_wakeup_reason();
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_33, 1); // 1 = High, 0 = Low
  Serial.println("Going to sleep now");
  delay(1000);
  esp_deep_sleep_start();
  Serial.println("This will never be printed");
}

void loop()
{
  digitalWrite(LED, HIGH);
  delay(500);
  digitalWrite(LED, LOW);
  delay(500);
}

void print_wakeup_reason()
{
  esp_sleep_wakeup_cause_t w_r;
  w_r = esp_sleep_get_wakeup_cause();
  switch (w_r)
  {
  case ESP_SLEEP_WAKEUP_EXT0:
    Serial.println("Wakeup using RTC_GPIO");
    break;
  case ESP_SLEEP_WAKEUP_EXT1:
    Serial.println("Wakeup using RTC_CNTL");
    break;
  case ESP_SLEEP_WAKEUP_TIMER:
    Serial.println("Wakeup caused by timer");
    break;
  case ESP_SLEEP_WAKEUP_TOUCHPAD:
    Serial.println("Wakeup caused by touchpad");
    break;
  case ESP_SLEEP_WAKEUP_ULP:
    Serial.println("Wakeup caused by ULP program");
    break;
  default:
    Serial.printf("Wakeup was not caused by deep sleep: %d\n", w_r);
    break;
  }
}