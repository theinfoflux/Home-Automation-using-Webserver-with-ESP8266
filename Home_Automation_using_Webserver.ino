#include <ESP8266WiFi.h>

// Enter your wifi network name and Wifi Password
const char* ssid = "theinfoflux";
const char* password = "12345678";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// These variables store current output state of LED
String outputLed1State = "off";
String outputLed2State = "off";
String outputLed3State = "off";
String outputLed4State= "off";
// Assign output variables to GPIO pins
const int Led1 = D2;
const int Led2 = D3;
const int Led3 = D4;
const int Led4 = D5;
// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

void setup() {
Serial.begin(115200);
// Initialize the output variables as outputs
pinMode(Led1, OUTPUT);
pinMode(Led2, OUTPUT);
pinMode(Led3,OUTPUT);
pinMode(Led4,OUTPUT);
// Set outputs to LOW
digitalWrite(Led1, LOW);
digitalWrite(Led2, LOW);
digitalWrite(Led3, LOW);
digitalWrite(Led4, LOW);
// Connect to Wi-Fi network with SSID and password
Serial.print("Connecting to ");
Serial.println(ssid);
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}
// Print local IP address and start web server
Serial.println("");
Serial.println("WiFi connected.");
Serial.println("IP address: ");
Serial.println(WiFi.localIP());
server.begin();
}

void loop(){
WiFiClient client = server.available(); // Listen for incoming clients

if (client) { // If a new client connects,
Serial.println("New Client."); // print a message out in the serial port
String currentLine = ""; // make a String to hold incoming data from the client
currentTime = millis();
previousTime = currentTime;
while (client.connected() && currentTime - previousTime <= timeoutTime) { // loop while the client's connected
currentTime = millis(); 
if (client.available()) { // if there's bytes to read from the client,
char c = client.read(); // read a byte, then
Serial.write(c); // print it out the serial monitor
header += c;
if (c == '\n') { // if the byte is a newline character
// if the current line is blank, you got two newline characters in a row.
// that's the end of the client HTTP request, so send a response:
if (currentLine.length() == 0) {
// HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
// and a content-type so the client knows what's coming, then a blank line:
client.println("HTTP/1.1 200 OK");
client.println("Content-type:text/html");
client.println("Connection: close");
client.println();

// turns the GPIOs on and off
if (header.indexOf("GET /2/on") >= 0) {
Serial.println("Led1 is on");
outputLed1State = "on";
digitalWrite(Led1, HIGH);
} else if (header.indexOf("GET /2/off") >= 0) {
Serial.println("Led1 is off");
outputLed1State = "off";
digitalWrite(Led1, LOW);
} else if (header.indexOf("GET /4/on") >= 0) {
Serial.println("Led2 is on");
outputLed2State = "on";
digitalWrite(Led2, HIGH);
} else if (header.indexOf("GET /4/off") >= 0) {
Serial.println("Led2 is off");
outputLed2State = "off";
digitalWrite(Led2, LOW);
} else if (header.indexOf("GET /5/on") >= 0) {
Serial.println("Led3 is on");
outputLed3State = "on";
digitalWrite(Led3, HIGH);
} else if (header.indexOf("GET /5/off") >= 0) {
Serial.println("Led3 is off");
outputLed3State = "off";
digitalWrite(Led3, LOW);
}
else if (header.indexOf("GET /6/on") >= 0) {
Serial.println("Led4 is on");
outputLed4State = "on";
digitalWrite(Led4, HIGH);
} else if (header.indexOf("GET /6/off") >= 0) {
Serial.println("Led4 is off");
outputLed4State = "off";
digitalWrite(Led4, LOW);
}
// Display the HTML web page
client.println("<!DOCTYPE html><html>");
client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
client.println("<link rel=\"icon\" href=\"data:,\">");
// CSS to style the on/off buttons 
client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
client.println(".buttonRed { background-color: #ff0000; border: none; color: white; padding: 16px 40px; border-radius: 60%;");
client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
client.println(".buttonGreen { background-color: #00ff00; border: none; color: white; padding: 16px 40px; border-radius: 60%;");
client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
client.println(".buttonYellow { background-color: #feeb36; border: none; color: white; padding: 16px 40px; border-radius: 60%;");
client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
client.println(".buttonBlue { background-color: #24a0ed; border: none; color: white; padding: 16px 40px; border-radius: 60%;");
client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");


client.println(".buttonOff { background-color: #77878A; border: none; color: white; padding: 16px 40px; border-radius: 70%;");
client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}</style></head>");

// Web Page Heading
client.println("<body><h1>Home Automation using Webserver</h1>");

// Display current state, and ON/OFF buttons for GPIO 2 Red LED 
client.println("<p>LED1 is " + outputLed1State + "</p>");
// If the outputLed1State is off, it displays the OFF button 
if (outputLed1State=="off") {
client.println("<p><a href=\"/2/on\"><button class=\"button buttonOff\">OFF</button></a></p>");
} else {
client.println("<p><a href=\"/2/off\"><button class=\"button buttonRed\">ON</button></a></p>");
} 

// Display current state, and ON/OFF buttons for GPIO 4 Led2 
client.println("<p>Led2 is " + outputLed2State + "</p>");
// If the outputLed2State is off, it displays the OFF button 
if (outputLed2State =="off") {
client.println("<p><a href=\"/4/on\"><button class=\"button buttonOff\">OFF</button></a></p>");
} else {
client.println("<p><a href=\"/4/off\"><button class=\"button buttonGreen\">ON</button></a></p>");
}
client.println("</body></html>");

// Display current state, and ON/OFF buttons for GPIO 5 Led3 
client.println("<p>Led3 is " + outputLed3State + "</p>");
// If the outputLed3State is off, it displays the OFF button 
if (outputLed3State =="off") {
client.println("<p><a href=\"/5/on\"><button class=\"button buttonOff\">OFF</button></a></p>");
} else {
client.println("<p><a href=\"/5/off\"><button class=\"button buttonYellow\">ON</button></a></p>");
}

// Display current state, and ON/OFF buttons for GPIO 5 Led3 
client.println("<p>Led4 is " + outputLed4State + "</p>");
// If the outputLed3State is off, it displays the OFF button 
if (outputLed4State =="off") {
client.println("<p><a href=\"/6/on\"><button class=\"button buttonOff\">OFF</button></a></p>");
} else {
client.println("<p><a href=\"/6/off\"><button class=\"button buttonBlue\">ON</button></a></p>");
}

client.println("</body></html>");

// The HTTP response ends with another blank line
client.println();
// Break out of the while loop
break;
} else { // if you got a newline, then clear currentLine
currentLine = "";
}
} else if (c != '\r') { // if you got anything else but a carriage return character,
currentLine += c; // add it to the end of the currentLine
}
}
}
// Clear the header variable
header = "";
// Close the connection
client.stop();
Serial.println("Client disconnected.");
Serial.println("");
}
}
