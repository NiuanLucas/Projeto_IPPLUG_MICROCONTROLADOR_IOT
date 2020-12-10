#include <DHT.h>
#include <DHT_U.h>
#include <SPI.h>
#include <Ethernet.h>
#include "DHT.h"

//Define a temperatura máxima e mínima  
int tmax = 45;
int tmin = 18; 

// Definicoes dos pinos dos leds 
int pin_led_1 =10;
int pin_led_2 = 11;
int pin_led_3 =9;

// Definicoes dos pinos ligados ao sensor 
int pinoledverm = 9; //Pino ligado ao led vermelho
#define DHTPIN A1 // pino que estamos conectado
#define DHTTYPE DHT11 // DHT 11
DHT dht(DHTPIN, DHTTYPE);

// Definicoes dos pinos das Tomadas
int pino_rele1 = 2;
int pino_rele2 = 3;
int pino_rele3 = 4;
int pino_rele4 = 5;
 
//Informacoes de endereco IP, gateway, mascara de rede
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0F, 0x28, 0x0B };
byte ip[] = { 169, 254, 97, 201};
byte gateway[] = { 192, 168, 1, 1 };
byte subnet[] = { 255, 255, 255, 0 };
EthernetServer server(80);
 
void setup()
{
  //Inicializa a interface de rede 
  pinMode(pin_led_1, OUTPUT);
  pinMode(pin_led_2, OUTPUT);
  pinMode(pin_led_3, OUTPUT);
  pinMode(pino_rele1, OUTPUT);
  pinMode(pino_rele2, OUTPUT);
  pinMode(pino_rele3, OUTPUT);
  pinMode(pino_rele4, OUTPUT); 
  Serial.begin(9600);
  Serial.println("DHTxx test!");
  dht.begin();
  Ethernet.begin(mac, ip, gateway, subnet);
  server.begin();

}
 
void loop() {

float h = dht.readHumidity();
float t = dht.readTemperature();
if (t > tmin && t < tmax) {
  digitalWrite(pin_led_1, LOW);
  digitalWrite(pin_led_2, LOW);
  digitalWrite(pin_led_3, LOW);
  digitalWrite(pino_rele1, LOW);
  digitalWrite(pino_rele2, LOW);
  digitalWrite(pino_rele3, LOW);
  digitalWrite(pino_rele4, LOW); 
}
else {
    digitalWrite(pin_led_1, HIGH);
    digitalWrite(pin_led_2, HIGH);
    digitalWrite(pin_led_3, HIGH);
    digitalWrite(pino_rele1, HIGH);
    digitalWrite(pino_rele2, HIGH);
    digitalWrite(pino_rele3, HIGH);
    digitalWrite(pino_rele4, HIGH); 
}



  
 
//Aguarda conexao do browser
  EthernetClient client = server.available();
  if (client) {
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (c == 'n' && currentLineIsBlank) {
          
          //Codigo HTML5
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");
          client.println("Refresh: 2"); //Recarrega a pagina a cada 2seg
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.println("<br />");
          client.println("<center> <div id='wrapper'><img alt='FILIPEFLOP' src='http://ap.imagensbrasil.org/images/2017/07/28/333.png'/><br/> </center>");
          client.println("<h3> <center> NIUAN & LUCAS - Tomadas Inteligentes </center> </h3>");  
          client.println("<center>");
          //Configura o texto e imprime o titulo no browser
          client.print("<font color=#2369db><b><u>");
          client.print("Envio de informacoes do Sensor de Temperatura (DHT11) pela Rede utilizando Arduino");
          client.print("</u></b></font>");
          client.println("<br />");
          client.println("<br />");
          
          //Mostra as informacoes lidas pelo sensor  
if (t > tmin && t < tmax) {
          client.print("<b>");
          client.print("<font color=#45cc26>");
          client.print("TOMADAS LIGADAS");
          client.print("</font>");
          client.print("</b>");}
else {
           client.print("<b>");
          client.print("<font color=#e02323>");
          client.print("TOMADAS DESLIGADAS");
          client.print("</font>");
          client.print("</b>");}    
          client.println("<br />"); 
          client.println("<br />");
          client.print("Temperatura MAXIMA: ");
          client.print("<b>");
          client.print(tmax);
          client.print(" *C");
          client.print("</b>");
          client.println("<br />");
          client.print("Temperatura Atual: ");
          client.print("<font color=#2369db>");
          client.print("<b>");
          client.print(t);
          client.print(" *C");
          client.print("</b>");
          client.print("</font>");
          client.println("<br />");
          client.print("Temperatura MINIMA: ");
          client.print("<b>");
          client.print(tmin);
          client.print(" *C");
          client.print("</b>");
          client.println("<br />");
          client.println("<br />");
          client.print("Umidade: ");
          client.print("<b>");
          client.print(h);
          client.print(" %");
          client.println("<br />");
          client.println("<center> <div id='wrapper'><img alt= ' ' src=' https://s-media-cache-ak0.pinimg.com/originals/e9/e9/90/e9e99039ad9f8445e9b747f8e0b159ab.gif '/><br/> </center>");



          
          client.println("</html>");
          break;
        }
        if (c == 'n') {
          currentLineIsBlank = true;
        }
        else if (c != 'r') {
          currentLineIsBlank = false;
        }
      }
    }
    delay(1);
    // close the connection:
    client.stop();
    }
}
