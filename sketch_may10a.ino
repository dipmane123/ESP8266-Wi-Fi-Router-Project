#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "ESP8266_Router";  // Wi-Fi AP name
const char* password = "123456789";   // Wi-Fi AP password

ESP8266WebServer server(80);  // Create a web server object that listens on port 80

void setup() {
  Serial.begin(115200);
  WiFi.softAP(ssid, password);
  Serial.println("Access Point Started");
  Serial.println("IP Address: " + WiFi.softAPIP().toString());

  server.on("/", HTTP_GET, [](){
    String html = "<html><head><style>body { font-family: Arial; background: #222; color: #fff; text-align: center;}h1 { color: #4CAF50; }</style></head><body><h1>ESP8266 Wi-Fi Router</h1><p>SSID: <strong>" + String(ssid) + "</strong></p><p>Password: <strong>" + String(password) + "</strong></p><button onclick=\"window.location.href='/devices';\">Show Device Info</button><br><br><button onclick=\"window.location.reload();\">Refresh</button></body></html>";
    server.send(200, "text/html", html);
  });

  server.on("/devices", HTTP_GET, [](){
    String html = "<html><head><style>body { font-family: Arial; background: #222; color: #fff; text-align: center;}ul { list-style: none; }li { background: #333; padding: 10px; margin: 5px 0; }</style></head><body><h1>Connected Devices</h1>";
    int n = WiFi.softAPgetStationNum();
    html += "<p>Total Devices: <strong>" + String(n) + "</strong></p><ul>";

    struct station_info *station = wifi_softap_get_station_info();

    while (station) {
      html += "<li>MAC: ";
      for (int i = 0; i < 6; i++) {
        html += String(station->bssid[i], HEX);
        if (i < 5) html += ":";
      }
      html += " | IP: " + IPAddress(station->ip.addr).toString();
      html += " | Signal: " + String(WiFi.RSSI()) + " dBm";
      html += "</li>";
      station = STAILQ_NEXT(station, next);
    }

    html += "</ul><br><button onclick=\"window.location.href='/';\">Back</button></body></html>";
    server.send(200, "text/html", html);
  });

  server.begin();
}

void loop() {
  server.handleClient();
}