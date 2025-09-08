#include <WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C LCD = LiquidCrystal_I2C(0x27, 16, 2); //Configure LCD address first

#define NTP_SERVER     "pool.ntp.org"
#define UTC_OFFSET     19800 //Adjust A
#define UTC_OFFSET_DST 0

void spinner() {
  static int8_t counter = 0;
  const char* glyphs = "\xa1\xa5\xdb";
  LCD.setCursor(15, 1);
  LCD.print(glyphs[counter++]);
  if (counter == strlen(glyphs)) {
    counter = 0;
  }
}

void printLocalTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    LCD.setCursor(0, 1);
    LCD.print("Connection Err ");
    return;
  }

  char timeStr[9];   // HH:MM:SS
  strftime(timeStr, sizeof(timeStr), "%H:%M:%S", &timeinfo);

  char dateStr[17];  
  strftime(dateStr, sizeof(dateStr), "%a %d-%b-%y", &timeinfo);

  LCD.setCursor(8, 0);
  LCD.print(timeStr);

  LCD.setCursor(0, 1);
  LCD.print(dateStr);
  LCD.setCursor(13, 1);   // place IN at end
  LCD.print(" IN");
}

void setup() {
  Serial.begin(115200);

  LCD.init();
  LCD.backlight();
  LCD.setCursor(0, 0);
  LCD.print("Connecting to ");
  LCD.setCursor(0, 1);
  LCD.print("WiFi ");

  WiFi.begin("YourWiFiName", "YourWiFiPassword");  //Change this line according to your wifi & password
  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    spinner();
    }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  LCD.clear();
  LCD.setCursor(0, 0);
  LCD.print("Online-");
  LCD.setCursor(0, 1);
  LCD.print("Updating time...");

  configTime(UTC_OFFSET, UTC_OFFSET_DST, NTP_SERVER);
}

void loop() {
  printLocalTime();
  delay(250);
}

