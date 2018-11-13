#include "SoftwareSerial.h"
String ssid ="NOKIA"; // ssid
String password="cntt2017"; // password
SoftwareSerial client(6, 7);// RX, TX

// link get
String linkGet = "http://webservices.iotdanang.xyz/?getinfo&keyequipment=73cc77f9c288694c0a959d5a5818b861&apikey=24c9e15e52afc47c225b757e7bee1f9d&format=json";
String linkUpdate = "http://webservices.iotdanang.xyz/?updateinfo&keyequipment=73cc77f9c288694c0a959d5a5818b861&apikey=24c9e15e52afc47c225b757e7bee1f9d&state_one=3&state_two=4&state_three=5&format=xml";
String server  = "webservices.iotdanang.xyz";
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
  delay(5000);
  if(client.find("OK") ) Serial.println("Connect with " + ssid +" Successful");
  
  if(client.find("OK") ) Serial.println("Connect with " + ssid +" Successful");
  else { connectWifi(); Serial.println("Connect again");}
   
  client.println("AT+CIFSR");
  String ip = client.readString();
  Serial.println(ip);
}

void httpGet() {
  client.println("AT+CIPSTART=\"TCP\",\"" + server + "\",80");//start a TCP connection.
  if( client.find("OK")) {
     Serial.println("TCP connection ready");
  } 
  delay(1000);
  
  String sendCmd = "AT+CIPSEND=";//determine the number of caracters to be sent.
  client.print(sendCmd);
  client.println(linkGet.length() );
  delay(500);
  if(client.find(">")) { Serial.println("Sending.."); client.print(linkGet);
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
void httpUpdate(int i){
  String send1 = "http://webservices.iotdanang.xyz/?updateinfo&keyequipment=73cc77f9c288694c0a959d5a5818b861&apikey=24c9e15e52afc47c225b757e7bee1f9d&state_one=3&state_two=4&state_three="+i+"&format=xml";
  client.println("AT+CIPSTART=\"TCP\",\"" + server + "\",80");//start a TCP connection.
  if( client.find("OK")) {
     Serial.println("TCP connection ready");
  } 
  delay(1000);
  
  String sendCmd = "AT+CIPSEND=";//determine the number of caracters to be sent.
  client.print(send1.length());
  client.println("");
  delay(500);
  if(client.find(">")) { Serial.println("Sending.."); client.print(send1);
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

void loop () {
  int i = 0;
  delay(1000);
  httpGet();
  httpUpdate(i++);
}