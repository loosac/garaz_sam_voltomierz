//#include "DHT.h"
#include <SPI.h>
#include <Ethernet.h>

#define DHTPIN 2     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)

//DHT dht(DHTPIN, DHTTYPE);

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 225, 31);
//IPAddress gateway(192,168,225,1);
//IPAddress subnet(255, 255, 255, 0);
//IPAddress DNS(192, 168, 225, 1);
// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}


void loop() {
  // listen for incoming clients
    // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
//  float h = dht.readHumidity();
//  float t = dht.readTemperature();
  float voltage = 0.0;
  float vtemp=0.0;

  int analog_value;

// Pomiar napiecia na A0
  analog_value = analogRead(A0);
  vtemp = (analog_value * 5.0) / 1024.0; 

  voltage = vtemp/(0.092);
  if (voltage < 0.1) 
  {
    voltage=0.0;
   } 





  // check if returns are valid, if they are NaN (not a number) then something went wrong!
//  if (isnan(t) || isnan(h)) {
//    Serial.println("Failed to read from DHT");
//  } else {
    //Serial.print("Hum: "); 
//    Serial.print(h);
//    Serial.print(" %\t");
//    Serial.print("Temp: "); 
//    Serial.print(t);
//    Serial.print(" *C");
    Serial.print(" Volt: ");
    Serial.print(voltage);
    //Serial.print();
    Serial.println(" V");
  //}
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");

//          client.print("Hum: ");
//          client.print(h);
//          client.print("%");
//          client.print("Temp: ");
//          client.print(t);
//          client.println(" *C");
          client.print(voltage);
          client.println(" V");
          
          client.println("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
  delay(500);
}

