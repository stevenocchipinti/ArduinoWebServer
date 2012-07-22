/*
 * Arduino Web Server
 *
 * A simple web server that serves files from the MicroSD card reader
 * using an Arduino Wiznet Ethernet compatible Freetronics EtherMega 2560.
 *
 * Written by Steven Occhipinti
 *    http://stevenocchipinti.com
 *    https://github.com/stevenocchipinti/ArduinoWebServer
 *
 * This sketch is based on the work of:
 *    David A. Mellis   (created 18 Dec 2009)
 *    Tom Igoe          (modified 4 Sep 2010)
 *
 */



#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>

// Setup file to be read from the SD card
File myFile;
char* defaultFilename = "index.htm";

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192,168,1,177);

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);

// The pin that has the LED
int LED = 13;



void setup()
{
  // Setup the serial port so we can print debug info
  Serial.begin(9600);

  // Set up the LED
  pinMode(LED, OUTPUT);

  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();

  // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
  // Note that even if it's not used as the CS pin, the hardware SS pin
  // (10 on most Arduino boards, 53 on the Mega) must be left as an output
  // or the SD library functions will not work.
  pinMode(53, OUTPUT);
  if (!SD.begin(4)) {
    Serial.println("ERROR: SD Card initialization failed!");
    return;
  }
}



void loop()
{
  // listen for incoming clients
  EthernetClient client = server.available();

  // Store the request in a string
  String request = String(" ");

  // If a client is available, start parsing the request
  if (client) {
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {

          // Turn on the LED
          digitalWrite(LED, HIGH);

          // Do something with the request
          handleRequest(request, client);

          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        }
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
          request += c;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    // wait a second and turn off the LED
    delay(1000);
    digitalWrite(LED, LOW);
  }
}



// Work out what to do with the request
void handleRequest(String request, EthernetClient client) {

  if (request.indexOf("GET /status ") > 0) {
    serveStatusJSON(client);
  } else {
    serveHTMLFile(defaultFilename, client);
  }

}



// Serve HTTP headers (the content will need to come separately)
void serveHTTPHeaders(String status, String contentType, EthernetClient client) {
  client.print("HTTP/1.1 ");
  client.println(status);
  client.print("Content-Type: ");
  client.println(contentType);
  client.println();
}



// Serve a JSON dump to represent some kind of status
void serveStatusJSON(EthernetClient client) {
  // Send the HTTP headers
  serveHTTPHeaders("200 OK", "application/json", client);
  // Generate some content
  client.print("{\"aThing\":true,\"anotherThing\":false}");
}



// Serve a HTML file from the SD card to the given HTTP client
void serveHTMLFile(char* filename, EthernetClient client) {
  // Open the file for reading
  myFile = SD.open(filename);
  if (myFile) {
    // Send the HTTP headers
    serveHTTPHeaders("200 OK", "text/html", client);

    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      client.print((char)myFile.read());
    }

    // close the file:
    myFile.close();

  } else {
    // if the file didn't open, print an error:
    Serial.println("ERROR: Could not open file!");
  }
}
