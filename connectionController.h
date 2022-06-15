#include <WiFi.h>
#include "memoryController.h"
#include "httpController.h"


// Update these with values suitable for your network.
//char ssid[] = "Highlander Kost Lt.1";
//char password[] = "Sagitarius1";

//const char* mqtt_server = "192.168.0.201";

//#define MQTT_SERIAL_PUBLISH_CH "iot/005/qlog"
//#define MQTT_SERIAL_RECEIVER_CH "iot/005/qwoidstatus"



boolean setup_wifi(char* ssid, char* password) {
  delay(10);
  int timeout = 0;
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
//  if(strcmp(ssid, "") == 0){
//    ESP.restart();
//  }
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    timeout++;
    Serial.print(".");
    if (timeout == 20) {
      timeout = 0;
      return false;
    }
  }
  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  return true;
}

char* gatewayIPData() {
  static char gateway[20] = {0};
  IPAddress ip = WiFi.gatewayIP();
  sprintf(gateway, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
  char* gateIp = gateway;
  return gateIp;
}

boolean checkConn() {
  if (WiFi.status() != WL_CONNECTED) {
    return false;
  }
  else {
    return true;
  }
}
