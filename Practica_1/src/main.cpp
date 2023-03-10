#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

// Replace with your network credentials
const char* ssid = "RESTREPO";
const char* password = "1036402452";
boolean isTurnedOn1 = false;
boolean isTurnedOn2 = false;
//Web Page Variable
String webPage = "";

// WebServer Instantiation
ESP8266WebServer server(80);

void handlerLed1(){
  if(isTurnedOn1){
    isTurnedOn1 = false;
    digitalWrite(D1,LOW);
    server.send(200, "text/html", "Led 1 apagado");
  }else{
    isTurnedOn1 = true;
    digitalWrite(D1,HIGH); 
    server.send(200, "text/html", "Led 1 encendido");
  }
}

void handlerLed2(){
  if(isTurnedOn2){
    isTurnedOn2 = false;
    digitalWrite(D2,LOW);
    server.send(200, "text/html", "Led 2 apagado");
  }else{
    isTurnedOn2 = true;
    digitalWrite(D2,HIGH); 
    server.send(200, "text/html", "Led 2 encendido");
  }
}
 
void setup(void){
    //Build Basic Web Page using HTML
    webPage += "<h1>PRACTICA 1</h1>";
    webPage += "<body>";
    webPage +=	"<h1>Practica 1</h1>";
    webPage +=	"<p><a href=\"/led1/switch\"><button>Led 1</button></a></p>";
    webPage +=	"<p><a href=\"/led2/switch\"><button>Led 2</button></a></p>";
    webPage += "</body>";
  //Serial Comunnication and Wifi Connection kick off
  Serial.begin(115200);
  pinMode(D1,OUTPUT);
  pinMode(D2,OUTPUT); 
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  //Web Page handler on root "/" using .send() method from ESP8266WebServer class
  server.on("/", [](){
    server.send(200, "text/html", webPage);
  });

  //Start Web Server and notify via serial comm.
  server.begin();
  Serial.println("HTTP server started");

  // Configura las rutas del servidor web
  server.on("/led1/switch",handlerLed1);
  server.on("/led2/switch", handlerLed2);

}
 
void loop(void){
  //Handle client requests to web Server using .handleClient() method from ESP8266WebServer class
  server.handleClient();
}
