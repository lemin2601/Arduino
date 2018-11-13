#include "SoftwareSerial.h"
String ssid ="Vinh Quang";
String password="0914300587";
SoftwareSerial client(6, 7);// RX, TX

String server = "http://webservices.iotdanang.xyz"; // www.example.com
String uri = "index.php";// our example is /clientpost.php

void setup() {

client.begin(9600);

Serial.begin(9600);

reset();

connectWifi();

}
//reset the client8266 module

void reset() {

client.println("AT+RST");

delay(5000);

if(client.find("OK") ) Serial.println("Module Reset");

}


//connect to your wifi network

void connectWifi() {

String cmd = "AT+CWJAP=\"" +ssid+"\",\"" + password + "\"";

client.println(cmd);

delay(10000);

client.println("AT+CIFSR");

String tmpRclient = client.readString();

Serial.println(tmpRclient);



// if(client.find("OK")) {

// Serial.println("Connected!");

// }

// else {

// connectWifi();

// Serial.println("Cannot connect to wifi"); }

}


void httppost () {

client.println("AT+CIPSTART=\"TCP\",\"" + server + "\",80");//start a TCP connection.

if( client.find("OK")) {

Serial.println("TCP connection ready");

} delay(1000);

String postRequest ="";

// "POST " + uri + " HTTP/1.0\r\n" +

// "Host: " + server + "\r\n" +

// "Accept: *" + "/" + "*\r\n" +

//"Content-Length: " + data.length() + "\r\n" +

//"Content-Type: application/x-www-form-urlencoded\r\n" +

//"\r\n" + data;

String sendCmd = "AT+CIPSEND=";//determine the number of caracters to be sent.

client.print(sendCmd);

client.println(postRequest.length() );

delay(500);

if(client.find(">")) { Serial.println("Sending.."); client.print(postRequest);

if( client.find("SEND OK")) { Serial.println("Packet sent");

while (client.available()) {

String tmpRclient = client.readString();

Serial.println(tmpRclient);

}

// close the connection

client.println("AT+CIPCLOSE");

}

}
}
String url ="index.php/?getinfo&keyequipment=73cc77f9c288694c0a959d5a5818b861&apikey=24c9e15e52afc47c225b757e7bee1f9d&format=xml";
void httpget(){
	  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + server + "\r\n" + 
  unsigned long timeout = millis();
               "Connection: close\r\n\r\n");
  String sendCmd = "AT+CIPSEND=";//determine the number of caracters to be sent.

//client.print(sendCmd);
 if (client.available()) {
    char c = client.read();
    Serial.print(c);
 
 }
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      //client.stop();
	  
client.println("AT+CIPCLOSE");

      return;
    }
  }
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
}


void loop () {
	delay(1000);
	//if (client.connect(server, 80)) {
		client.print(String("GET ") + "/search?q=arduino HTTP/1.1\r\n" +
               "Host: www.google.com \r\n" + 
               "Connection: close\r\n\r\n");
		 // Make a HTTP request:
		
		 client.println();
	 //}
	 while (client.available()) {
	 char c = client.read();
	 Serial.write(c);
	 }
	 delay(5000);
	  
	//httpget();
}