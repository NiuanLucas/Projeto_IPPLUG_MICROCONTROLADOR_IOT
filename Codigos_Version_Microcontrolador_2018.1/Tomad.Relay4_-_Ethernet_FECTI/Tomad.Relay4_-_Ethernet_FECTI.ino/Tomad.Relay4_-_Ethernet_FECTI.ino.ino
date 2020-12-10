#include <SPI.h>
#include <Ethernet.h>

//Informacoes de endereco IP, gateway, mascara de rede
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0F, 0x28, 0x0B };
byte ip[] = { 192, 168, 1, 99 };
byte gateway[] = { 192, 168, 1, 1 };
byte subnet[] = { 255, 255, 255, 0 };
EthernetServer server(80);

int numPins = 4;
int pins[] = { 2, 3, 4, 5 };    // Botões para relés
int pinState[] = {0, 0, 0, 0};  // Status dos pinos

void setup()
{
// Inicialmente desligue todos os relés
  for (int i = 0; i < numPins; i++)
  {
    pinMode(pins[i], OUTPUT);
    digitalWrite(pins[i], 1);
  }
  
  Serial.begin(9600);
  Ethernet.begin(mac, ip);
  server.begin();
}

void loop()
{
  EthernetClient client = server.available();
  if (client)
  {
// Verifique se o cliente está conectado ao servidor
    while (client.connected())
    {
// Verifique se a solicitação é enviada para o servidor
      int dataCount = client.available();
      if (dataCount > 0)
      {
// Leia os dados transmitidos ao servidor a partir do cliente (navegador)
        for (int i = 0; i < dataCount; i++)
        {
          char ch = client.read();
// Se o dado for passado, ele será passado pela solicitação POST, que começa com o caractere 'P'
          if (i == 0 && ch != 'P')
            break;
          if (ch == '\n' && i < dataCount - 1)
          {
// Encontre a seqüência de caracteres que contém os dados a serem enviados
             char chNext = client.read();
// Formato da linha r2 = on & r3 = on & r4 = on (Exemplo se 2,3,4 relés estiverem ativados)
             if (chNext == 'r')
             {
// Desligue todos os relés
               pinState[0] = 0;
               pinState[1] = 0;
               pinState[2] = 0;
               pinState[3] = 0;
               
// Leia o primeiro número do relé a ser ativado
               char relayNum = client.read();
               pinState[relayNum-'0'] = 1;
               Serial.write(relayNum);
               
// Leia as informações auxiliares (= on &)
               relayNum = client.read();
               relayNum = client.read();
               relayNum = client.read();
               relayNum = client.read();
                
// Enquanto houver dados nos relés restantes, leia e escreva para o conjunto pinState
               while (relayNum != -1)
               {
                  relayNum = client.read();
                  relayNum = client.read();
                  pinState[relayNum-'0'] = 1;
                  Serial.write(relayNum);
                  relayNum = client.read();
                  relayNum = client.read();
                  relayNum = client.read();
                  relayNum = client.read();
               }
             } else
             {
// Se nenhum dado foi transferido, desative todos os relés
                pinState[0] = 0;
                pinState[1] = 0;
                pinState[2] = 0;
                pinState[3] = 0;
             }
          }
        }
      }
      
// De acordo com os dados transmitidos, ativamos o relé
      for (int i = 0; i < 4; i++)
      {
         digitalWrite(pins[i], !pinState[i]);
      }
      
// Exibe a página HTML onde o usuário pode ligar ou desligar o relé
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println();
      client.println("<html>");
      client.println("<head>");
      client.println("<title>NIUAN & LUCAS - Tomadas Inteligentes.</title>");
      client.println("</head>");
      client.println("<body>"); 
      client.println("<br/>");
      client.println("<center> <div id='wrapper'><img alt='FILIPEFLOP' src='http://ap.imagensbrasil.org/images/2017/07/28/333.png'/><br/> </center>");
      client.println("<br/>");
      client.println("<h3> <center> NIUAN & LUCAS - Tomadas Inteligentes </center> </h3>");  
      client.println("<center>");

      
      client.println("<form method='post'>");
      
      client.print("<div>TOMADA 1 <input type='checkbox' ");
      if (pinState[0] == 1)
        client.print("checked");
      client.println(" name='r0'></div>");
      client.println("<br/>");
      client.print("<div>TOMADA 2 <input type='checkbox' ");
      if (pinState[1] == 1)
        client.print("checked");
      client.println(" name='r1'></div>");
      client.println("<br/>");
      client.print("<div>TOMADA 3 <input type='checkbox' ");
      if (pinState[2] == 1)
        client.print("checked");
      client.println(" name='r2'></div>");
      client.println("<br/>");
      client.print("<div>TOMADA 4 <input type='checkbox' ");
      if (pinState[3] == 1)
        client.print("checked");
      client.println(" name='r3'></div>");
      client.println("<br/>");
      client.println("<input type='submit' value='Refresh'>");
      client.println("</form>");

      client.println("</center>");
      client.println("<br />");
      client.println("<center> <div id='wrapper'><img alt= ' ' src=' https://www.kernelsphere.com/wp-content/uploads/2013/11/Green-IoT.gif '/><br/> </center>");
      client.println("</body>");
      client.println("</html>");
      client.stop(); 
    }
  }
}
