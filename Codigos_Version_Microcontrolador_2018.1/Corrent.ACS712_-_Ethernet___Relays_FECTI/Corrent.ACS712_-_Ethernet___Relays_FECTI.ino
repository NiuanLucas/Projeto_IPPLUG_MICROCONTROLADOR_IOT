#include <SPI.h>
#include <Ethernet.h>

// Definicoes dos pinos dos leds 
int pin_led_1 =10;
int pin_led_2 = 11;
int pin_led_3 =9;

// Definicoes dos pinos ligados ao sensor 
int VQ;
int ACSPin = A3;
float pmax = 5;
float pmin = 0; 
int volt = 127;

// Definicoes dos pinos das Tomadas
int pino_rele1 = 2;
int pino_rele2 = 3;
int pino_rele3 = 4;
int pino_rele4 = 5;

//Informacoes de endereco IP, gateway, mascara de rede
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0F, 0x28, 0x0B };
byte ip[] = { 192, 168, 1, 99 };
byte gateway[] = { 192, 168, 1, 1 };
byte subnet[] = { 255, 255, 255, 0 };
EthernetServer server(80);


void setup() 
{
pinMode(pino_rele1, OUTPUT);
pinMode(pino_rele2, OUTPUT);
pinMode(pino_rele3, OUTPUT);
pinMode(pino_rele4, OUTPUT); 
pinMode(pin_led_1, OUTPUT);
pinMode(pin_led_2, OUTPUT); 
pinMode(pin_led_3, OUTPUT);

pinMode(2, OUTPUT); 
analogReference(DEFAULT);

VQ = determineVQ(ACSPin);  Serial.begin(9600);
Ethernet.begin(mac, ip, gateway, subnet);
server.begin();
}


void loop() 
{
  
//digitalWrite(2, HIGH);  
Serial.print("Corrente: ");
Serial.print(readCurrent(ACSPin),3);
Serial.println(" A");
float pot = (readCurrent(ACSPin));
Serial.print("Corrente: ");
Serial.print(pot);
Serial.println(" A");


if ( pot > pmin && pot < pmax) {    
    digitalWrite(pin_led_1, LOW);
    digitalWrite(pin_led_2, LOW);
    digitalWrite(pin_led_3, LOW);
    digitalWrite(pino_rele1, HIGH);
    digitalWrite(pino_rele1, LOW);
    digitalWrite(pino_rele2, LOW);
    digitalWrite(pino_rele3, LOW);
    digitalWrite(pino_rele4, LOW); 
    Serial.println("Corrente no Permitido !!!");
    Serial.println("/nTOMADAS DESLIGADAS");
}

else {
    digitalWrite(pin_led_1, HIGH);
    digitalWrite(pin_led_2, HIGH);
    digitalWrite(pin_led_3, HIGH);
    digitalWrite(pino_rele1, HIGH);
    digitalWrite(pino_rele2, HIGH);
    digitalWrite(pino_rele3, HIGH);
    digitalWrite(pino_rele4, HIGH); 
    Serial.println("Corrente Acima do Permitido !!!");
    Serial.println("/nTOMADAS DESLIGADAS");
    delay(30000);
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
          client.print("Envio de informacoes do Sensor de Corrente (ACS712) pela Rede utilizando Arduino");
          client.print("</u></b></font>");
          client.println("<br />");
          client.println("<br />");
          
//Mostra as informacoes lidas pelo sensor CORRENTE
if ( pot > pmin && pot < pmax) {
          client.println("<center>");
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
          client.print("</b>");
          client.println("<br />");
          client.print("Aguarde 30 segundos para a verificacao da taxa de potencia...");
}
          client.println("<br />");
          client.println("<br />");
          client.print("Corrente MAXIMA: ");
          client.print("<b>");
          client.print(pmax);
          client.print(" A");
          client.print("</b>");
          client.println("<br />");
          client.print("Corrente Atual: ");
          client.print("<font color=#2369db>");
          client.print("<b>");
          client.print(pot);
          client.print(" A");
          client.print("</b>");
          client.print("</font>");
          client.println("<br />");
          client.print("Corrente MINIMA: ");
          client.print("<b>");
          client.print(pmin);
          client.print(" A");
          client.print("</b>");
          client.println("<br />");  
          client.println("<br />"); 
          client.print("Potencia Atual Estimada: ");
          client.print("<font color=#d83b27>");
          client.print("<b>");
          client.print(pot * volt);
          client.print(" W");
          client.print("</b>");
          client.print("</font>");
          client.println("<br />");
          client.println("<br />");
          client.println("<div id='wrapper'><img alt= ' ' src=' https://raw.githubusercontent.com/anastr/SpeedView/master/images/SpeedView.gif '/><br/>");
          client.println("</b>");
          client.println("</center>");


          
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










int determineVQ(int PIN) 
{
Serial.print("Estimando a Media de coeficiente de tensao:");
long VQ = 0;

//le 1000 amostra para estabilizar o valor
for (int i=0; i<10000; i++) {
VQ += abs(analogRead(PIN));
delay(1);                   }



VQ /= 10000;
Serial.print(map(VQ, 0, 1023, 0, 5000));Serial.println(" mV");
return int(VQ);
}

float readCurrent(int PIN) {
int current = 0;
int sensitivity = 100;//muda para 100 sefor ACS712-20A ou para 66 for ACS712-5A
//le 200 vezes
for (int i=0; i<200; i++) {
current += abs(analogRead(PIN) - VQ);
delay(1);
}
current = map(current/200, 0, 1023, 0, 5000);
return float(current)/sensitivity;
}
