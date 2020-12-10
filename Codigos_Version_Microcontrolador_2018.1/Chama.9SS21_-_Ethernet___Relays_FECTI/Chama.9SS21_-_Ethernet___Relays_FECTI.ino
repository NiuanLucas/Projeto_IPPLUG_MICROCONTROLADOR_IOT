#include <SPI.h>
#include <Ethernet.h>

// Definicoes dos pinos dos leds 
int pin_led_1 =10;
int pin_led_2 = 11;
int pin_led_3 =9;

// Definicoes dos pinos ligados ao sensor 
int pino_D0 = 6;
int pino_A0 = A0;
int valor_a = 0;
int valor_d = 0;
int nivel_sensor = 850;

// Definicoes dos pinos das Tomadas
int pino_rele1 = 2;
int pino_rele2 = 3;
int pino_rele3 = 4;
int pino_rele4 = 5;

//Informacoes de endereco IP, gateway, mascara de rede
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0F, 0x28, 0x0B };
byte ip[] = { 169, 254, 97, 205};
byte gateway[] = { 192, 168, 1, 1 };
byte subnet[] = { 255, 255, 255, 0 };
EthernetServer server(80);

void setup()
{

  pinMode(pino_A0, INPUT);
  pinMode(pino_D0, INPUT);
  
  pinMode(pin_led_1, OUTPUT);
  pinMode(pin_led_2, OUTPUT);
  pinMode(pin_led_3, OUTPUT);
  
  pinMode(pino_rele1, OUTPUT);
  pinMode(pino_rele2, OUTPUT);
  pinMode(pino_rele3, OUTPUT);
  pinMode(pino_rele4, OUTPUT); 
  Serial.begin(9600);
  Ethernet.begin(mac, ip, gateway, subnet);
  server.begin();
}

void loop()
{
  int valor_a = analogRead(pino_A0);
  int valor_d = digitalRead(pino_D0);
  
 if (valor_a > nivel_sensor)
  {  
  digitalWrite(pin_led_1, HIGH);
  digitalWrite(pin_led_2, LOW);
  digitalWrite(pin_led_3, LOW);
  digitalWrite(pino_rele1, LOW);
  digitalWrite(pino_rele2, LOW);
  digitalWrite(pino_rele3, LOW);
  digitalWrite(pino_rele4, LOW);     
  Serial.print("Fogo NÃO Detectado ... ");
  Serial.println("/nTOMADAS LIGADAS");
  Serial.print("Porta analogica: ");
  Serial.print(valor_a);
  Serial.print(" Porta digital: ");
  Serial.print(valor_d);
  Serial.println(); 
  }
  if (valor_a < nivel_sensor)
  {
    digitalWrite(pin_led_1, HIGH);
    digitalWrite(pin_led_2, HIGH);
    digitalWrite(pin_led_3, HIGH);
    digitalWrite(pino_rele1, HIGH);
    digitalWrite(pino_rele2, HIGH);
    digitalWrite(pino_rele3, HIGH);
    digitalWrite(pino_rele4, HIGH); 
   Serial.println("Fogo Detectado !!!");
   Serial.println("/nTOMADAS DESLIGADAS");
   digitalWrite(pino_rele1, HIGH);
   digitalWrite(pino_rele2, HIGH);
   digitalWrite(pino_rele3, HIGH);
   digitalWrite(pino_rele4, HIGH); 
   Serial.print("Porta analogica: ");
   Serial.print(valor_a);
   Serial.print(" Porta digital: ");
   Serial.print(valor_d);
   Serial.println(); 
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
          client.println("<link rel='shortcut icon' href='http://www.iconj.com/ico/b/c/bc634hfzo1.ico' type='image/x-icon'/>");
          client.println("<br />");
          client.println("<center> <div id='wrapper'><img alt= ' ' src=' https://image.ibb.co/iFazW7/Ip_plug_2_0.png '/><br/> </center>");
          client.println("<h3> <center> NIUAN & LUCAS - Tomadas Inteligentes </center> </h3>");  
          client.println("<center>");
          //Configura o texto e imprime o titulo no browser
          client.print("<font color=#2369db><b><u>");
          client.print("Envio de informacoes do Sensor de Chama (9SS21) pela Rede utilizando Arduino");
          client.print("</u></b></font>");
          client.println("<br />");
          client.println("<br />");
          client.println("</center>");
          
          //Mostra as informacoes lidas pelo sensor  CHAMA
          client.println("<center>");
          if (valor_a > nivel_sensor) {
          client.print("<b>");
          client.print("<font color=#45cc26>");
          client.print("TOMADAS LIGADAS");
          client.print("</font>");
          client.print("</b>");}
          if (valor_a < nivel_sensor) {
           client.print("<b>");
          client.print("<font color=#e02323>");
          client.print("TOMADAS DESLIGADAS");
          client.print("</font>");
          client.print("</b>");}
          client.println("<br />");
          client.println("<br />");
          client.print("Porta analogica: ");
          client.print("<b>");
          client.print(valor_a);
          client.print("");
          client.print("</b>");
          client.println("<br />");
          client.print("Porta digital: ");
          client.print("<b>");
          client.print(valor_d);
          client.print("");
          client.print("</b>");
          client.println("<br />");
          client.print("Status: ");
          client.print("<b>");
          if (valor_a > nivel_sensor) {
          client.print("FOGO NAO DETECTADO");}
          if (valor_a < nivel_sensor) {
          client.print("<font color=#f97807>");
          client.print("FOGO DETECTADO");}
          client.println("<br />");
          client.println("<div id  ='wrapper'><img alt='FILIPEFLOP' src='https://i.giphy.com/media/CjV3S78MgUcNO/giphy.webp'/><br/>");
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
