#include <WiFi.h>
#include <WiFiUdp.h>
#define M1P1 21
#define M1P2 13
#define M2P1 14
#define M2P2 25
#define M3P1 26
#define M3P2 27
#define M4P1 32
#define M4P2 33
#define EN1 22
#define EN2 23
#define EN3 18
#define EN4 19

const int freq = 30000;
const int resolution = 8;
int dutyCycle = 200;

// Wi-Fi Credentials (Replace with your network's SSID and password)
const char* ssid = "+";
const char* password = "qwertyui00";

// UDP Server Configuration
WiFiUDP udp;
const unsigned int localPort = 8080;  // Listening port
char incomingPacket[255];             // Buffer for received messages

void setup() {
  pinMode(M1P1, OUTPUT);
  pinMode(M1P2, OUTPUT);
  pinMode(EN1, OUTPUT);
    pinMode(M2P1, OUTPUT);
  pinMode(M2P2, OUTPUT);
  pinMode(EN2, OUTPUT);
    pinMode(M3P1, OUTPUT);
  pinMode(M3P2, OUTPUT);
  pinMode(EN3, OUTPUT);
   pinMode(M4P1, OUTPUT);
  pinMode(M4P2, OUTPUT);
  pinMode(EN4, OUTPUT);
  Serial.begin(115200);

    // Connect to Wi-Fi
    WiFi.begin(ssid, password);
    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConnected to Wi-Fi");

    // Start UDP
    udp.begin(localPort);
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    Serial.printf("Listening on UDP port %d\n", localPort);
      ledcAttachChannel(EN1, 3000, 8, 0);
      ledcAttachChannel(EN2, 3000, 8, 0);
      ledcAttachChannel(EN3, 3000, 8, 1);
      ledcAttachChannel(EN4, 3000, 8, 1);
      digitalWrite(M1P1, LOW);
     digitalWrite(M1P2, HIGH);
      digitalWrite(M2P1, LOW);
     digitalWrite(M2P2, HIGH);
      digitalWrite(M3P1, LOW);
     digitalWrite(M3P2, HIGH);
      digitalWrite(M4P1, LOW);
     digitalWrite(M4P2, HIGH);
      

}

void loop() {
    int packetSize = udp.parsePacket();
    int RT = 0, LT = 0;
    if (packetSize) {
        // Read packet
        int len = udp.read(incomingPacket, sizeof(incomingPacket) - 1);
            incomingPacket[len] = '\0';  // Null-terminate the received string
            Serial.printf("\rReceived: %s\n", incomingPacket);
            Serial.print(" ");
        // Parse the received string
        if (sscanf(incomingPacket, "(%d,%d)", &RT, &LT) == 2) {
            // Successfully extracted both integers
            Serial.printf("RT: %d, LT: %d\n", RT, LT);
        } else {
            // Parsing failed
            //Serial.println("Failed to parse the incoming packet.");
        }
        
    }
    analogWrite(EN1, RT);   
    analogWrite(EN3, RT);   
    analogWrite(EN2, LT);   
    analogWrite(EN4, LT);   
    delay(100);

}
