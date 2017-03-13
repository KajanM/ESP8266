

#include <ESP8266WiFi.h>
#include <WiFiClient.h>

char ssid[] = "Kalla Kundu";
char pass[] = "*******";

int status = WL_IDLE_STATUS;

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}


 
void setup() {
  WiFiServer server(80);

  Serial.begin(9600); 

  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present"); 
    // don't continue:
    while(true);
  }

  // attempt to connect to Wifi network:
  status = WiFi.begin(ssid, pass);
  while ( status != WL_CONNECTED) { 
    status = WiFi.status();
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:    

    // wait 10 seconds for connection:
    delay(10000);
  } 
  server.begin();
  // you're connected now, so print out the status:
  printWifiStatus();
}


void loop() {
  // put your main code here, to run repeatedly:

}
