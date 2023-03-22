#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "Darta";
const char* password =  "lxc00123";
//const char* mqttServer = "broker.mqtt-dashboard.com";
const char* mqttServer = "test.mosquitto.org";
const int mqttPort = 1883;
//const char* mqttuser = "darta";
//const char* mqttpassword = "pass1";

const char* TopicInput = "input";
const char* TopicOutput = "output";
const char* TopicAlive = "alive";
const char* TopicStatusRequest = "StatusRequest";
const char* TopicJsonStatus = "JsonStatus";

WiFiClient espClient;
PubSubClient client(espClient);
WiFiClient clientHttp;

const String api= "http://worldtimeapi.org/api/timezone/";
String zonaHoraria;

//Wifi Connection kick off
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print(F("Connecting to ")) ;
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

String getDay(int number){
  switch (number) {
  case 0:
    return "Domingo";
    break;
  case 1:
    return "Lunes";
    break;
  case 2:
    return "Martes";
    break;
  case 3:
    return "Miercoles";
    break;
  case 4:
    return "Jueves";
    break;
  case 5:
    return "Viernes";
    break;
  case 6:
    return "Sabado";
    break;
  default:
    return "Dia";
    break;
  }
}

String getMonthLetters(String numero){
  if(numero.equals("01")){
    return "Enero";
  }else if (numero=="02"){
    return "Febrero";
  }else if (numero=="03"){
    return "Marzo";
  }else if (numero=="04"){
    return "Abril";
  }else if (numero=="05"){
    return "Mayo";
  }else if (numero=="06"){
    return "Junio";
  }else if (numero=="07"){
    return "Julio";
  }else if (numero=="08"){
    return "Agosto";
  }else if (numero=="09"){
    return "Septiembre";
  }else if (numero=="10"){
    return "Octubre";
  }else if (numero=="11"){
    return "Noviembre";
  }else if (numero=="12"){
    return "Diciembre";
  }else{
    return "Mes";
  }
}

String getYear(String string){
   char str[string.length()+1];
   int i=0;
   for (i=0;i<string.length();i++) {
      //Serial.println((char)cadena[i]);
      if((char)string[i] != '-'){
        str[i]=(char)string[i];
      }else {
        break;
      }

    }
    str[i] = 0; // Null termination
    Serial.println(str);
    return str;
}

String getMonth(String string){
  return string.substring(5,7);
}

String getDayNumber(String string){
  return string.substring(8,10);
}

String getHour(String string){
  return string.substring(11,16);
}

void constOutput(String dayLetters, String year, String dayNumber, String month, String hour){

String string;

  string = dayLetters + ", " + dayNumber + " de " + month + " de " + year + " -- " + hour;
  
  Serial.println(string);
  client.publish(TopicOutput, string.c_str());
  
}

void consumeApi(String api, String zonaHoraria){

    HTTPClient http;
    if (http.begin(clientHttp, api+zonaHoraria)) { 
    
    int httpCode = http.GET();  //Realizamos la petición
    String codigoStr = String(httpCode);
    
         
        if (httpCode > 0) { //código de retorno

           Serial.println(httpCode); // esperamos que sea 200
           if(httpCode==200){
            client.publish(TopicStatusRequest,"HTTP OK");
            String data = http.getString();
            client.publish(TopicJsonStatus,"JSON OK");
           
            
            Serial.println(data);
    
            StaticJsonDocument <256> doc;
            deserializeJson(doc,data);
    
            // deserializeJson(doc,str); can use string instead of payload
            const char* datetime = doc["datetime"];
            int day_of_week = doc["day_of_week"];
          
            Serial.print("datetime: ");
            Serial.println(datetime);
            
            Serial.println("day_of_week: "+day_of_week);
            constOutput(getDay(day_of_week),getYear(datetime),getDayNumber(datetime),getMonthLetters(getMonth(datetime)),getHour(datetime));
           }else if(httpCode == 404){
            client.publish(TopicStatusRequest, "Error 404");
            client.publish(TopicOutput, "Parece que hubo un error al consultar la zona horaria");
            client.publish(TopicJsonStatus, ":'(");
           }else if(httpCode == 400){
            client.publish(TopicStatusRequest, "Error 400");
            client.publish(TopicOutput, "Parece que hubo un error al consultar la zona horaria");
            client.publish(TopicJsonStatus, ":'(");
           }
           
           
          }
          else {
            Serial.println("Error en la petición HTTP");
          }
 
    http.end(); //cerramos conexión y liberamos recursos
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  
    Serial.println("callback se esta ejecutando: ");
    

    char str[length+1];
    Serial.print("topico: ");
    Serial.println(topic);
    Serial.print("mensaje: ");
    int i=0;
    for (i=0;i<length;i++) {
      Serial.print((char)payload[i]);
      str[i]=(char)payload[i];
    }
    Serial.println();
   
   
    str[i] = 0; // Null termination
    zonaHoraria = str;
    Serial.println("zona horaria: "+zonaHoraria);
    consumeApi(api,zonaHoraria);

}

void setup() {
  //Start Serial Communication
  Serial.begin(115200);
  
  //Connect to WiFi
  setup_wifi();

  //Connect to MQTT Broker
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
 
  //MQTT Connection Validation
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
 
    if (client.connect("Practica_2"/*,mqttuser,mqttpassword*/)) {
 
      Serial.println("connected");  
 
    } else {
 
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
 
    }
  }
  
  //Publish to desired topic and subscribe for messages
  client.publish(TopicAlive, "Estamos vivos!");
  client.subscribe(TopicInput);
}

void loop() {
  //MQTT client loop
  client.loop(); 
}