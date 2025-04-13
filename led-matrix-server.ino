#include <WiFiS3.h>
#include <ArduinoGraphics.h>
#include <Arduino_LED_Matrix.h>
#include "arduino_secrets.h"  // Your Wi-Fi credentials

ArduinoLEDMatrix matrix;
WiFiServer server(80);

String message = "";  // Will hold the IP or custom message
unsigned long lastScroll = 0;
const int scrollSpeed = 100; // ms

// URL decoding function: converts percent-encoded sequences and '+' to spaces.
String decodeURL(String input) {
  String decoded = "";
  char tempChar;
  for (int i = 0; i < input.length(); i++) {
    if (input[i] == '%') {
      if (i + 2 < input.length()) {
        char hexStr[3];
        hexStr[0] = input[i+1];
        hexStr[1] = input[i+2];
        hexStr[2] = '\0';
        long hexValue = strtol(hexStr, NULL, 16);
        tempChar = static_cast<char>(hexValue);
        decoded += tempChar;
        i += 2; // Skip the hex digits
      } else {
        decoded += input[i];
      }
    }
    else if (input[i] == '+') {
      decoded += ' ';
    }
    else {
      decoded += input[i];
    }
  }
  return decoded;
}

void setup() {
  Serial.begin(115200);
  matrix.begin();

  connectToWiFi(); // Waits until real IP

  IPAddress ip = WiFi.localIP();
  message = "   http://" + ipToString(ip);
  Serial.print("Connected! IP Address: ");
  Serial.println(ip);

  server.begin();
}

void loop() {
  handleClient();

  if (millis() - lastScroll > scrollSpeed) {
    scrollText();
    lastScroll = millis();
  }
}

void connectToWiFi() {
  Serial.print("Connecting to WiFi");

  WiFi.begin(SECRET_SSID, SECRET_PASS);

  // Wait until connected
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Wait until IP is not 0.0.0.0
  while (WiFi.localIP() == IPAddress(0, 0, 0, 0)) {
    delay(200);
    Serial.print("#");
  }

  Serial.println("\nWiFi connected!");
}

void handleClient() {
  WiFiClient client = server.available();
  if (!client) return;

  String req = "";
  while (client.connected()) {
    if (client.available()) {
      char c = client.read();
      req += c;
      if (req.endsWith("\r\n\r\n")) break;
    }
  }

  // Parse GET /?msg=...
  if (req.indexOf("GET /?msg=") >= 0) {
    int start = req.indexOf("GET /?msg=") + 10;
    int end = req.indexOf(' ', start);
    String msg = req.substring(start, end);
    // Decode URL-encoded string using our custom function
    message = "   " + decodeURL(msg);  // Add 2 spaces padding
    Serial.print("New message: ");
    Serial.println(message);
  }

  // Serve HTML form
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html\n");
  client.println("<!DOCTYPE html><html><body>");
  client.println("<h2>Send message to LED matrix</h2>");
  client.println("<form method='GET'>");
  client.println("Message: <input name='msg' type='text'>");
  client.println("<input type='submit' value='Send'></form>");
  client.println("</body></html>");
  delay(1);
  client.stop();
}

void scrollText() {
  matrix.beginDraw();
  matrix.textScrollSpeed(scrollSpeed);
  matrix.textFont(Font_5x7);
  matrix.beginText(0, 1, 0xFFFFFF);
  matrix.println(message.c_str());
  matrix.endText(SCROLL_LEFT);
  matrix.endDraw();
}

String ipToString(IPAddress ip) {
  return String(ip[0]) + "." + String(ip[1]) + "." + String(ip[2]) + "." + String(ip[3]);
}
