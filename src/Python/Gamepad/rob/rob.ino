#include <WiFi.h>

const char* ssid = "COMP_NEWDEPT";
const char* password = "geccomp@123";

WiFiServer server(8080);

int value1 = 0;
int value2 = 0;

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    Serial.println("Client connected");
    while (client.connected()) {
      if (client.available() > 0) {
        String incomingData = client.readStringUntil('\n');
        int commaIndex = incomingData.indexOf(',');
        if (commaIndex != -1) {
          value1 = incomingData.substring(0, commaIndex).toInt();
          value2 = incomingData.substring(commaIndex + 1).toInt();
          if (value1 >= 0 && value1 <= 255 && value2 >= 0 && value2 <= 255) {
            Serial.print("Value 1: ");
            Serial.print(value1);
            Serial.print(", Value 2: ");
            Serial.println(value2);
          } else {
            Serial.println("Invalid values");
          }
        } else {
          Serial.println("Invalid data format");
        }
      }
    }
    client.stop();
  }
  delay(100);
}
