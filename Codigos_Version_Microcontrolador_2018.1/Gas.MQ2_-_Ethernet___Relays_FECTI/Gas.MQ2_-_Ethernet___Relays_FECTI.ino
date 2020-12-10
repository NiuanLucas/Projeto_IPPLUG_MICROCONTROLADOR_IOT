#include <SPI.h>
#include <Ethernet.h>

// Definicoes dos pinos dos leds 
int pin_led_1 =10;
int pin_led_2 = 11;
int pin_led_3 =9;

// Definicoes dos pinos ligados ao sensor 
int pin_d0 = 8;
int pin_a0 = A2;
int nivel_sensor = 150;

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
  pinMode(pin_d0, INPUT);
  pinMode(pin_a0, INPUT);
  
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

  
  // Le os dados do pino digital D0 do sensor
  int valor_digital = digitalRead(pin_d0);
  // Le os dados do pino analogico A0 do sensor
  int valor_analogico = analogRead(pin_a0);
  // Mostra os dados no serial monitor
  Serial.print("Pino D0 : ");
  Serial.print(valor_digital);
  Serial.print(" Pino A0 : ");
  Serial.println(valor_analogico);

  
  // Verifica o nivel de gas/fumaca detectado
  if (valor_analogico > nivel_sensor)
  {
    // Liga o buzzer e o led vermelho, e
    // desliga o led verde
    digitalWrite(pin_led_1, HIGH);
    digitalWrite(pin_led_2, HIGH);
    digitalWrite(pin_led_3, HIGH);
    digitalWrite(pino_rele1, HIGH);
    digitalWrite(pino_rele2, HIGH);
    digitalWrite(pino_rele3, HIGH);
    digitalWrite(pino_rele4, HIGH);   
    Serial.print("Gás Detectado !!! ");
    Serial.println("/nTOMADAS DESLIGADAS");
  }
  else
  {
    
    digitalWrite(pin_led_1, LOW);
    digitalWrite(pin_led_2, LOW);
    digitalWrite(pin_led_3, LOW);
    digitalWrite(pino_rele1, LOW);
    digitalWrite(pino_rele2, LOW);
    digitalWrite(pino_rele3, LOW);
    digitalWrite(pino_rele4, LOW);    
    Serial.println("Gás NÃO Detectado ...");
    Serial.println("/nTOMADAS LIGADAS"); 
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
          client.print("Envio de informacoes do Sensor de Chama (9SS21) pela Rede utilizando Arduino");
          client.print("</u></b></font>");
          client.println("<br />");
          client.println("<br />");

          
          //Mostra as informacoes lidas pelo sensor ultrasonico
          if (valor_analogico < nivel_sensor) {
          client.print("<b>");
          client.print("<font color=#45cc26>");
          client.print("TOMADAS LIGADAS");
          client.print("</font>");
          client.print("</b>");}
          if (valor_analogico >= nivel_sensor) {
           client.print("<b>");
          client.print("<font color=#e02323>");
          client.print("TOMADAS DESLIGADAS");
          client.print("</font>");
          client.print("</b>");}
          client.println("<br />");
          client.println("<br />");
          client.print("Porta analogica: ");
          client.print("<b>");
          client.print(valor_analogico);
          client.print("");
          client.print("</b>");
          client.println("<br />");
          client.print("Porta digital: ");
          client.print("<b>");
          client.print(valor_digital);
          client.print("");
          client.print("</b>");
          client.println("<br />");
          client.print("Status: ");
          client.print("<b>");
          if (valor_analogico < nivel_sensor) {
          client.print("GAS NAO DETECTADO");}
          if (valor_analogico >= nivel_sensor) {
          client.print("<font color=#a807ba>");
          client.print("GAS DETECTADO");}
          client.println("<br />");
          client.println("<center> <div id  ='wrapper'><img alt=' ' src='http://backyardbbqstore.com/imageserver/UserMedia/backyardbbq/smoke.gif.pagespeed.ce.l9kAyPISQq.gif'/><br/> </center>");
          client.println("</b>");



          
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
