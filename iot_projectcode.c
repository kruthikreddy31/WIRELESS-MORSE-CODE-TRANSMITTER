#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#define LED_PIN 4   // D2 = GPIO4

const char* ssid = "iot";
const char* password = "12345678";

ESP8266WebServer server(80);

int dotDelay = 200;

// Morse blink
void blinkMorse(String code) {
  for (int i = 0; i < code.length(); i++) {
    digitalWrite(LED_PIN, HIGH);

    if (code[i] == '.') delay(dotDelay);
    else if (code[i] == '-') delay(dotDelay * 3);

    digitalWrite(LED_PIN, LOW);
    delay(dotDelay);
  }
}

// Morse conversion (basic)
String getMorse(char c) {
  switch (toupper(c)) {
    case 'S': return "...";
    case 'O': return "---";
    default: return "";
  }
}

// Send message
void sendMessage(String msg) {
  Serial.print("Message: ");
  Serial.println(msg);

  for (int i = 0; i < msg.length(); i++) {
    String code = getMorse(msg[i]);
    blinkMorse(code);
    delay(dotDelay * 2);
  }
}

// Web page
void handleRoot() {
  String html = "<h2>Morse Code LED</h2>";
  html += "<form action=\"/send\">";
  html += "Message: <input name=\"msg\">";
  html += "<input type=\"submit\" value=\"Send\">";
  html += "</form>";
  server.send(200, "text/html", html);
}

// Handle message
void handleSend() {
  String msg = server.arg("msg");
  sendMessage(msg);
  server.send(200, "text/html", "Sent!");
}

void setup() {
  Serial.begin(115200);
  Serial.println("\nStarting...");

  pinMode(LED_PIN, OUTPUT);

  WiFi.begin(ssid, password);
  Serial.print("Connecting");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/send", handleSend);
  server.begin();

  Serial.println("Server Started");
}

void loop() {
  server.handleClient();

