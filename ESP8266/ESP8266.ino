 
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

char ssid[] = "Kalla Kundu";
char pass[] = "*******";

int status = WL_IDLE_STATUS;

WiFiServer server(80);

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


#include <dht11.h>

dht11 DHT11;

#define DHT11PIN 4
//Dht11 dht11(DHT11PIN);

double Fahrenheit(double celsius) 
{ 
  return 1.8 * celsius + 32; 
}

 
void setup() {
  

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
  

 char linebuf[255], getvalue[255], *lineptr;
  int linecount;
  
  // listen for incoming clients
  WiFiClient client = server.available();
  if (client) {
#ifdef sdebug
    Serial.println("new client");
#endif
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    lineptr = linebuf;
    linecount = 0;
    
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
#ifdef sdebug
        Serial.write(c);
#endif
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");

           int chk = DHT11.read(DHT11PIN); 
  client.print("Read sensor: "); 
  /*
  switch (chk) 
  { 
    case dht11::DHTLIB_OK: 
      client.println("OK"); 
      break; 
    case dht11::DHTLIB_ERROR_CHECKSUM: 
      client.println("Checksum error"); 
      break; 
    case dht11::DHTLIB_ERROR_TIMEOUT: 
      client.println("Time out error"); 
      break; 
    default: 
      client.println("Unknown error"); 
      break; 
    }
    */ 
    client.print("Humidity (%): "); 
    client.println((float)DHT11.humidity, 2); 
    client.print("<br>Temperature (&deg;C): "); 
    client.println((float)DHT11.temperature, 2); 
    client.print("<br>Temperature (&deg;F): "); 
    client.println(Fahrenheit(DHT11.temperature), 2); 
          
          client.println("<br></html>");
           break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
          if (memcmp(linebuf, "GET ", 4) == 0)
          {
            strcpy(getvalue, &(linebuf[4]));
#ifdef sdebug
            Serial.print("GET buffer = ");
            Serial.println(getvalue);
#endif
          }
          lineptr = linebuf;
          linecount = 0;
        } 
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
          if (linecount < sizeof(linebuf))
          {
            *lineptr++ = c;
            linecount++;
          }
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    
    // close the connection:
    client.stop();
#ifdef sdebug
    Serial.println("client disonnected");
#endif
  }
}


