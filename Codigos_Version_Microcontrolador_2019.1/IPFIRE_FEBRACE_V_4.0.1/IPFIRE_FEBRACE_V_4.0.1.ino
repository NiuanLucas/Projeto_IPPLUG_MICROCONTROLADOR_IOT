#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>



// ============= Você precisa preencher com seus dados ===================================
const char* ssid = "Nilson_Junior";
const char* password = "143013597";

int pino_D0 = 4;
int pino_A0 = A0;
int valor_d = digitalRead(pino_D0);
int valor_a = analogRead(pino_A0);

String title = " 🆘 IP FIRE 🆘 ";
String msgu = "Teste INICIAL - NIUAN ";

const char* PushBulletAPIKEY = "o.SH8hX2NgjHhrNqBXRvGNRlbSkTUz9UzX"; //A sua chave de API. Veja na sua conta Pushbullet.
const String message_title = String (title);
const String message_body = String (msgu);

// ========================================================================================

const char* host = "api.pushbullet.com";
const int httpsPort = 443;
//Tenha em mente que este fingerprint muda toda vez que a Pusnbullet renovar seu certificado
const char* fingerprint = "E7 06 F1 30 B1 5F 25 72 00 4D 99 F0 ED 90 4D F9 60 D3 FB 75";

void printFreeRAM() {
  Serial.print("RAM livre: ");
  Serial.print(ESP.getFreeHeap());
  Serial.print(" bytes.");

}


void setup() {
  Serial.begin(115200);
  Serial.println(" INICIADOR - NIUAN ");
  pinMode(pino_D0, INPUT);
  pinMode(pino_A0, INPUT);
  
  //Serial.setDebugOutput(true);
  printFreeRAM();
  WiFi.mode(WIFI_STA); //Apenas para me certificar de que o AP não seja ativado por uma configuração anterior gravada na flash
  Serial.println();
  Serial.print("Conectando a: ");
  Serial.println(ssid);
  Serial.print("Com a senha: ");
  Serial.println(password);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi conetado.");
  Serial.println("Endereco IP do modulo: ");
  Serial.println(WiFi.localIP());

  // Use WiFiClientSecure class to create TLS connection
  WiFiClientSecure client;
  Serial.print("Conectando a: ");
  Serial.println(host);
  if (!client.connect(host, httpsPort)) {
    Serial.println("Falha na conexao.");
    return;
  }

  if (client.verify(fingerprint, host)) {
    Serial.println("O certificado confere.");
  } else {
    Serial.println("O certificado não confere.");
  }

  printFreeRAM(); //A RAM livre cai uns 20KB neste ponto.

  Serial.println("Enviando POST.");

  String url = "/v2/pushes";
  String messagebody = "{\"type\": \"note\", \"title\": \"" + message_title + "\", \"body\": \"" + message_body + "\"}\r\n";
  Serial.print("requesting URL: ");
  Serial.println(url);

  client.print(String("POST ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Authorization: Bearer " + PushBulletAPIKEY + "\r\n" +
               "Content-Type: application/json\r\n" +
               "Content-Length: " +
               String(messagebody.length()) + "\r\n\r\n");
  client.print(messagebody);


  Serial.println("Esperando resposta.");

  while (client.available() == 0);

  while (client.available()) {
    String line = client.readStringUntil('\n');
    Serial.println(line);
  }

}

void loop() {

  Serial.println(" ");


  String msg1 = "Tudo Seguro!";
  String msg2 = " 🔥🔥🔥 Fogo Detectado | Local: Quarto Niuan | Evacuem Imediatamente a Região! 🔥🔥🔥 ";
  String msg3 = "  💨💨💨 Vazamento de Gas Metano Detectado | Local: Quarto Niuan | Evacuem a Região Imediatamente! 💨💨💨 ";
  String tle1 = " 🆘 IP FIRE 🆘 ";
  String tle2 = " 🚨 IP FIRE 🚨 ";
  String tle3 = " ⚠ IP FIRE ⚠ ";
  
  int pino_D0 = 4;
  int pino_A0 = A0;
  int valor_d = digitalRead(pino_D0);
  int valor_a = analogRead(pino_A0);

  int valor_gas = 500;

  Serial.print(" Porta Digital (Fogo): ");
  Serial.print(valor_d);
  Serial.print(" Porta Analogica (Gas): ");
  Serial.print(valor_a);

  Serial.println(" ");

  if (valor_d == 1) {
    Serial.print("  | Fogo NAO Detectado!  |  ");
    String msgu = String (msg1);
    Serial.println(msgu);
  }
  if (valor_d == 0) {
    Serial.print("  | Fogo SIM Detectado!  |  ");
    String msgu = String (msg2);
    Serial.println(msgu);
  }
  if (valor_a < valor_gas) {
    Serial.print("  | Gas NAO Detectado!  |  ");
    String msgu = String (msg1);
    Serial.println(msgu);
  }
  if (valor_a >= valor_gas) {
    Serial.print("  | Gas SIM Detectado!  |  ");
    String msgu = String (msg3);
    Serial.println(msgu);
  }

  Serial.println(" ");
  Serial.print(title);
  Serial.println(" ");

  if (valor_d == 0 ) {
    Serial.println(" ");
    Serial.begin(115200);
    //Serial.setDebugOutput(true);
    printFreeRAM();
    WiFi.mode(WIFI_STA); //Apenas para me certificar de que o AP não seja ativado por uma configuração anterior gravada na flash
    Serial.println();
    Serial.print("Conectando a: ");
    Serial.println(ssid);
    Serial.print("Com a senha: ");
    Serial.println(password);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi conetado.");
    Serial.println("Endereco IP do modulo: ");
    Serial.println(WiFi.localIP());

    // Use WiFiClientSecure class to create TLS connection
    WiFiClientSecure client;
    Serial.print("Conectando a: ");
    Serial.println(host);
    if (!client.connect(host, httpsPort)) {
      Serial.println("Falha na conexao.");
      return;
    }

    if (client.verify(fingerprint, host)) {
      Serial.println("O certificado confere.");
    } else {
      Serial.println("O certificado não confere.");
    }

    printFreeRAM(); //A RAM livre cai uns 20KB neste ponto.

    Serial.println("Enviando POST.");

    String url = "/v2/pushes";
    String messagebody = "{\"type\": \"note\", \"title\": \"" +  String(tle2) + "\", \"body\": \"" + String(msg2) + "\"}\r\n";
    Serial.print("requesting URL: ");
    Serial.println(url);

    client.print(String("POST ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Authorization: Bearer " + PushBulletAPIKEY + "\r\n" +
                 "Content-Type: application/json\r\n" +
                 "Content-Length: " +
                 String(messagebody.length()) + "\r\n\r\n");
    client.print(messagebody);


    Serial.println("Esperando resposta.");

    while (client.available() == 0);

    while (client.available()) {
      String line = client.readStringUntil('\n');
      Serial.println(line);
    }


  }


  if (valor_a >= valor_gas ) {
    Serial.println(" ");
    Serial.begin(115200);
    //Serial.setDebugOutput(true);
    printFreeRAM();
    WiFi.mode(WIFI_STA); //Apenas para me certificar de que o AP não seja ativado por uma configuração anterior gravada na flash
    Serial.println();
    Serial.print("Conectando a: ");
    Serial.println(ssid);
    Serial.print("Com a senha: ");
    Serial.println(password);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi conetado.");
    Serial.println("Endereco IP do modulo: ");
    Serial.println(WiFi.localIP());

    // Use WiFiClientSecure class to create TLS connection
    WiFiClientSecure client;
    Serial.print("Conectando a: ");
    Serial.println(host);
    if (!client.connect(host, httpsPort)) {
      Serial.println("Falha na conexao.");
      return;
    }

    if (client.verify(fingerprint, host)) {
      Serial.println("O certificado confere.");
    } else {
      Serial.println("O certificado não confere.");
    }

    printFreeRAM(); //A RAM livre cai uns 20KB neste ponto.

    Serial.println("Enviando POST.");

    String url = "/v2/pushes";
    String messagebody = "{\"type\": \"note\", \"title\": \"" + String(tle3) + "\", \"body\": \"" + String(msg3) + "\"}\r\n";
    Serial.print("requesting URL: ");
    Serial.println(url);

    client.print(String("POST ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Authorization: Bearer " + PushBulletAPIKEY + "\r\n" +
                 "Content-Type: application/json\r\n" +
                 "Content-Length: " +
                 String(messagebody.length()) + "\r\n\r\n");
    client.print(messagebody);


    Serial.println("Esperando resposta.");

    while (client.available() == 0);

    while (client.available()) {
      String line = client.readStringUntil('\n');
      Serial.println(line);
    }


  }

  Serial.println(" ");
  delay(1000);


}
