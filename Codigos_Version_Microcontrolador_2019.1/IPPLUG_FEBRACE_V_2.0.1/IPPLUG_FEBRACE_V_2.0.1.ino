#include <SPI.h>
#include <Ethernet.h>
#include "ACS712.h"
#include <Ultrasonic.h>
#include <dht.h>
#define dht_dpin A1 //Pino DATA do Sensor ligado na porta Analogica A1
dht DHT; //Inicializa o sensor

int pino_chama = 7;
int pino_gas = 6; 
int pino_presenca = 5;

int pino_rele1 = 19;
int pino_rele2 = 18;
int pino_rele3 = 17;
int pino_rele4 = 16;

ACS712 sensor(ACS712_20A, A0);

//Define os pinos para o Ultrasom
#define pino_trigger 3
#define pino_echo 2

String readString;
char c;

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xDA, 0xFE, 0xEE };
//byte ip[] = { 192, 168, 1, 50 };
byte ip[] = { 169, 254, 97, 205 };
EthernetServer server(8080);

String codeweb;
String sideweb;


void setup() {
Serial.begin(9600);
Serial.println(" INICIALIZACAO - IPPLUG ");
sensor.calibrate();



pinMode(pino_chama, INPUT);
pinMode(pino_gas, INPUT);
pinMode(pino_presenca, INPUT);  

pinMode(pino_rele1, OUTPUT);
pinMode(pino_rele2, OUTPUT);
pinMode(pino_rele3, OUTPUT);
pinMode(pino_rele4, OUTPUT); 

digitalWrite(pino_rele1, HIGH);
digitalWrite(pino_rele2, HIGH);
digitalWrite(pino_rele3, HIGH);
digitalWrite(pino_rele4, HIGH);

Ethernet.begin(mac, ip);
server.begin();
SPI.begin();

}

void loop(){


int valor_chama = digitalRead(pino_chama);
int valor_gas = digitalRead(pino_gas);
int valor_presenca = digitalRead(pino_presenca); 
DHT.read11(dht_dpin);

Ultrasonic ultrasonic(pino_trigger, pino_echo);
float cmMsec;
long microsec = ultrasonic.timing();
cmMsec = ultrasonic.convert(microsec, Ultrasonic::CM);

sensor.calibrate();
float V = 110;
float I = sensor.getCurrentAC(60);
float P = V * I;

EthernetClient client = server.available();

if (client) {
boolean currentLineIsBlank = true;
while (client.connected()) {
if (client.available()) {
char c = client.read();

if (readString.length() < 100) {
readString += c;
}

if (c == '\n') {
if(readString.indexOf("?") > 0){


//##SEPARANDO O VALOR DA VARIAVEL SIDE##//
int acao1I = readString.indexOf("side=") + 5;
int acao1F = readString.indexOf("&");
sideweb = readString.substring(acao1I,acao1F);
int side = sideweb.toInt();

Serial.print("LADO RECEBIDO: ");
Serial.println(side);

//##SEPARANDO O VALOR DA VARIAVEL CODE##//
int acao2I = readString.indexOf("code=") + 5;
int acao2F = readString.indexOf("HTTP/1.1");
codeweb = readString.substring(acao2I,acao2F);
int code = codeweb.toInt();

Serial.print("CODIGO RECEBIDO: ");
Serial.println(code);

}

//##PAGINA WEB QUE SERÁ EXIBIDA PARA O CLIENTE
client.println(F("HTTP/1.1 200 OK"));
client.println("GET 169.254.97.205/Controle/teste.php?q=arduino HTTP/1.1 200 OK");
client.println(F("Content-Type: text/html"));
client.println("Refresh: 2");
client.println();

client.println(F("<HTML>"));
client.println(F("<HEAD>"));
client.println("<meta charset='utf-8'>");
client.println(F("</HEAD>"));

int code = codeweb.toInt();
int side = sideweb.toInt();

client.println("<body>");

client.println("<br><b>CODIGO RECEBIDO: </b>");
client.println(code);
client.println("<br>");

client.println("<br><b>LADO RECEBIDO: </b>");
client.println(side);
client.println("<br>");


client.println("<br><H4> INFORMAÇÕES DO ARDUINO <HR></H4>"); 

client.println("<form action='https://ipplug.xyz/index.php?page=sensor&action=valores' method='post' id='envia1'>");
client.println("</br> Status Tomada #1: " + String(digitalRead(pino_rele1)) + ".");
client.println("<input type='text' name='tomada1' value='"+ String(digitalRead(pino_rele1)) +"'>");
client.println("</br> Status Tomada #2: " + String(digitalRead(pino_rele2)) + ".");
client.println("<input type='text' name='tomada2' value='"+ String(digitalRead(pino_rele2)) +"'>");
client.println("</br> Status Tomada #3: " + String(digitalRead(pino_rele3)) + ".");
client.println("<input type='text' name='tomada3' value='"+ String(digitalRead(pino_rele3)) +"'>");
client.println("</br> Status Tomada #4: " + String(digitalRead(pino_rele4)) + ".");;
client.println("<input type='text' name='tomada4' value='"+ String(digitalRead(pino_rele4)) +"'>");
client.println("</br></br> Corrente Elétrica: " + String(I) + "Amperes.");
client.println("<input type='text' name='corrente' value='"+ String(I) +"'>");
client.println("</br> Tensão Elétrica: " + String(V) + " Volts.");
client.println("<input type='text' name='tensao' value='"+ String(V) +"'>");
client.println("</br> Potência Elétrica: " + String(P) + " Watts.");
client.println("<input type='text' name='potencia' value='"+ String(P) +"'>");
client.println("</br></br> Sensor de Temperatura: " + String(DHT.temperature) + " *C.");
client.println("<input type='text' name='temperatura' value='"+ String(DHT.temperature) +"'>");
client.println("</br> Sensor de Umidade: " + String(DHT.humidity) + " %.");
client.println("<input type='text' name='umidade' value='"+ String(DHT.humidity) +"'>");
client.println("</br> Sensor de Fogo/Chama: " + String(valor_chama) + ".");
client.println("<input type='text' name='chama' value='"+ String(valor_chama) +"'>");
client.println("</br> Sensor de Presença: " + String(valor_presenca) + ".");
client.println("<input type='text' name='presenca' value='"+ String(valor_presenca) +"'>");
client.println("</br> Sensor de Gás: " + String(valor_gas) + ".");
client.println("<input type='text' name='gas' value='"+ String(valor_gas) +"'>");
client.println("</br> Sensor de Distância: " + String(cmMsec) + " Cm.");
client.println("<input type='text' name='distancia' value='"+ String(cmMsec) +"'>");
client.println("</br></br><input type='submit' value='Gravar na Base' >");

if(code == 990){client.println("<script type='text/javascript'> document.getElementById('envia1').submit();</script>");}
client.println("</form>");


client.println("</br></br></br>");
client.println("<br><H4> COMANDOS DE VOZ <HR></H4>"); 

if(side == 301){
client.println("<form action='https://ipplug.xyz/index.php?' method='get' id='envia2'>");
client.println("<br><input type='text' name='page' value='usuario'>");
client.println("<br><input type='text' name='action' value='userpage'>");
}

if(side == 401){client.println("<form action='https://ipplug.xyz/voz/fale.php' method='get' id='envia2'>");}


//## OPERAÇÕES DAS TOMADAS
if ( code == 211) 
{client.println("<br><H4> LIGAR TOMADA 1 </br></H4>");
digitalWrite(pino_rele1, LOW);
client.println("<input type='text' name='tomada1' value='"+ String(digitalRead(pino_rele1)) +"'>");}

if ( code == 210) 
{client.println("<br><H4> DESLIGAR TOMADA 1 </br></H4>");
digitalWrite(pino_rele1, HIGH);
client.println("<input type='text' name='tomada1' value='"+ String(digitalRead(pino_rele1)) +"'>");}

if ( code == 221) 
{client.println("<br><H4> LIGAR TOMADA 2 </br></H4>");
digitalWrite(pino_rele2, LOW);
client.println("<input type='text' name='tomada2' value='"+ String(digitalRead(pino_rele2)) +"'>");}

if ( code == 220) 
{client.println("<br><H4> DESLIGAR TOMADA 2 </br></H4>");
digitalWrite(pino_rele2, HIGH);
client.println("<input type='text' name='tomada2' value='"+ String(digitalRead(pino_rele2)) +"'>");}

if ( code == 231) 
{client.println("<br><H4> LIGAR TOMADA 3 </br></H4>");
digitalWrite(pino_rele3, LOW);
client.println("<input type='text' name='tomada3' value='"+ String(digitalRead(pino_rele3)) +"'>");}

if ( code == 230) 
{client.println("<br><H4> DESLIGAR TOMADA 3 </br></H4>");
digitalWrite(pino_rele3, HIGH);
client.println("<input type='text' name='tomada3' value='"+ String(digitalRead(pino_rele3)) +"'>");}

if ( code == 241) 
{client.println("<br><H4> LIGAR TOMADA 4 </br></H4>");
digitalWrite(pino_rele4, LOW);
client.println("<input type='text' name='tomada4' value='"+ String(digitalRead(pino_rele4)) +"'>");}

if ( code == 240) 
{client.println("<br><H4> DESLIGAR TOMADA 4 </br></H4>");
digitalWrite(pino_rele4, HIGH);
client.println("<input type='text' name='tomada4' value='"+ String(digitalRead(pino_rele4)) +"'>");}


//## OPERAÇÕES DOS SENSORES
if ( code == 703) 
{client.println("<br><H4> EXIBIR SENSOR TEMPERATURA </br></H4></br><hr>");
client.print("Temperatura = ");
client.print(DHT.temperature); 
client.println(" Celsius  ");
client.println("<br><input type='text' name='temperatura' value='"+ String(DHT.temperature) +"'>");}

if ( code == 704) 
{client.println("<br><H4> EXIBIR SENSOR UMIDADE </br></H4></br><hr>");
client.print("Umidade = ");
client.print(DHT.humidity);
client.print(" % ");
client.println("<br><input type='text' name='umidade' value='"+ String(DHT.humidity) +"'>");}

if ( code == 705) 
{client.println("<br><H4> EXIBIR SENSOR FOGO/CHAMA </br></H4></br><hr>");
if (valor_chama == 1) { client.println(" Ambiente Seguro");}
if (valor_chama == 0) { client.println(" Fogo Detectado");}
client.println("<br><input type='text' name='chama' value='"+ String(valor_chama) +"'>");}

if ( code == 706) 
{client.println("<br><H4> EXIBIR SENSOR PRESENCA </br></H4></br><hr>");
if (valor_presenca == 0) {client.println(" Parado ");}
if (valor_presenca == 1){client.println(" Movimento Detectado ");}
client.println("<br><input type='text' name='presenca' value='"+ String(valor_presenca) +"'>");}

if ( code == 707) 
{client.println("<br><H4> EXIBIR SENSOR GAS </br></H4></br><hr>");
if (valor_gas == 1) {client.println("Ambiente Seguro");}
if (valor_gas == 0) {client.println("Gas Detectado");}
client.println("<br><input type='text' name='gas' value='"+ String(valor_gas) +"'>");}

if ( code == 708) 
{client.println("<br><H4> EXIBIR SENSOR DISTANCIA </br></H4></br><hr>");
if (cmMsec > 200) {client.println("Distância Longa");}
if (cmMsec > 50 && cmMsec < 150) {client.println("Distância Media");}
if (cmMsec > 1 && cmMsec < 50) {client.println("Distância Curta");}
client.println("<br><input type='text' name='distancia' value='"+ String(cmMsec) +"'>");}

if ( code == 803) 
{client.println("<br><H4> EXIBIR SENSOR TENSÃO ELETRICA </br></H4></br><hr>");
client.print("Tensão Elétrica = ");
client.print(V);
client.print(" Volts.");
client.println("<br><input type='text' name='tensao' value='"+ String(V) +"'>");}

if ( code == 804) 
{client.println("<br><H4> EXIBIR SENSOR CORRENTE ELETRICA </br></H4></br><hr>");
client.print("Corrente Elétrica = ");
client.print(I);
client.print(" Amperes.");
client.println("<br><input type='text' name='corrente' value='"+ String(I) +"'>");}

if ( code == 805) 
{client.println("<br><H4> EXIBIR SENSOR POTENCIA ELETRICA </br></H4></br><hr>");
client.print("Potência Elétrica = ");
client.print(P);
client.print(" Watts.");
client.println("<br><input type='text' name='potencia' value='"+ String(P) +"'>");}


client.println("<input type='submit' value='Comando de Voz' >");

if(side == 301){client.println("<script type='text/javascript'> document.getElementById('envia2').submit(); </script>");}
if(side == 401){client.println("<script type='text/javascript'> document.getElementById('envia2').submit(); </script>");}


client.println("</form>");
client.println(F("</body>"));

client.println(F("</html>"));
client.flush();
client.stop();
readString="";


}
}
}
}
}
