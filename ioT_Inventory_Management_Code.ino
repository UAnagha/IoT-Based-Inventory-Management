#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <WiFi.h>
#include<ThingSpeak.h>

#define CHANNEL_ID 2486692
#define CHANNEL_API_KEY "I1RGA84BJXFXL21B"

WiFiClient globalClient;


#define DHT_PIN 11
#define DHT_TYPE DHT11
DHT_Unified dht(DHT_PIN, DHT_TYPE);

uint32_t delayMS;
int relay = 5;

#define trigPin 7
#define echoPin 8

#define buzzer 6

#define max_dist 38
#define min_dist 5
#define item_height 6

int qty_left = 0;

const char *ssid = "Ana";
const char *password ="Password";
WiFiServer server(80);

void setup()
{
  Serial.begin(9600);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(buzzer, OUTPUT);
  pinMode(relay, OUTPUT);

  dht.begin();
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  delayMS = sensor.min_delay / 1000;

  Serial.print("Connecting to ");
  Serial.println(ssid);
  connectToWiFi();
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();

  ThingSpeak.begin(globalClient);
}

void loop() {
  handleClientRequests();
  printSensorData(globalClient); 
  delay(1000); 
  ThingSpeak.setField(1,qty_left);
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  float temperature = event.temperature;
  ThingSpeak.setField(2, temperature); 

  dht.humidity().getEvent(&event);
  float humidity = event.relative_humidity;
  ThingSpeak.setField(3, humidity);
  ThingSpeak.writeFields(CHANNEL_ID,CHANNEL_API_KEY);
  delay(15000);
}


void connectToWiFi() {
  WiFi.begin(ssid, password);
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 10) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
}

void handleClientRequests() {
  WiFiClient client = server.available();
  if (client) {
    globalClient = client;
    Serial.println("New Client.");
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (c == '\n') {
          if (currentLine.length() == 0) {
            processRequest(client);
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    client.stop();
    Serial.println("Client disconnected.");
  }
}

void processRequest(WiFiClient client) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/html");
  client.println("Connection: close");
  client.println();

  client.println("<!DOCTYPE html><html>");
  client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
  client.println("<link rel=\"icon\" href=\"data:,\">");
  client.println("<style>html { font-family: Helvetica; display: flex; justify-content: center; align-items: center;}");
   client.println("h1 { text-align: center; color: #333333;font-size: 46px;}");
  client.println(".container { display: flex; flex-direction: column;}");
  client.println(".shelf { margin-right: 20px;position: relative; }");
   client.println(".shelf h1, .details h2 { text-align: center; margin: 0 auto; }");
  client.println(".details { width: 500px;}");
  client.println(".button { border: none; color: white; padding: 20px 0px; text-decoration: none; font-size: 36px; margin: 10px; cursor: pointer; background-color: #4CAF50; width: 100%;}");
  client.println(".low { background-color: red;}");
  client.println(".normal { background-color: #4CAF50;}");
  client.println("</style></head>");
  client.println("<body><h1><u>Stock-Sense</u></h1>");

  client.println("<div class=\"container\">");
  client.println("<div class=\"shelf\">");
  displayShelfOptions(client);
  client.println("</div>");
  client.println("<div class=\"details\">");
  if (client.find("GET /shelf1")) {
    displayShelfDetails(client, "Shelf One");
  } else {
    displayShelfDetails(client, "Shelf One"); 
  }
  client.println("</div></div>");

  client.println("</body></html>");
}


void displayShelfOptions(WiFiClient client) {
  client.println("<p><a href=\"/shelf1\"><button class=\"button\">Shelf One</button></a></p>");
}

void displayShelfDetails(WiFiClient client, const char* shelfName) {
  client.print("<h2>");
  client.print(shelfName);
  client.println(" Details:</h2>");

  // Print sensor data
  printSensorData(client);

  //client.println("<p><a href=\"/\">Back</a></p>");
}


void printSensorData(WiFiClient client) {
  sensors_event_t event;

  dht.temperature().getEvent(&event);
  globalClient.print("<p>Temperature: ");
  globalClient.print(event.temperature);
  globalClient.println("°C</p>");

  dht.humidity().getEvent(&event);
  globalClient.print("<p>Humidity: ");
  globalClient.print(event.relative_humidity);
  globalClient.println("%</p>");

  long duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  /*globalClient.print("<p>Distance: ");
  globalClient.print(distance);
  globalClient.println(" cm</p>");*/

  qty_left = (max_dist - distance) / item_height;
  if (qty_left < 0) {
    qty_left = 0;
  }

  if (qty_left <= 1) {
    globalClient.println("<p class=\"low\">Low inventory! Additional stock required</p>");
    activateBuzzer();
  } else {
    globalClient.print("<p class=\"normal\">Quantity Left: ");
    globalClient.print(qty_left);
    globalClient.println("</p>");
    deactivateBuzzer();
  }
}
void activateBuzzer() {
  digitalWrite(buzzer, HIGH); 
}

void deactivateBuzzer() {
  digitalWrite(buzzer, LOW); 
}
