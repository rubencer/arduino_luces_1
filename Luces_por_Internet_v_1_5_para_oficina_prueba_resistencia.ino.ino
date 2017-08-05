//#include <ICMPPing.h>

/* Control Arduino Ethernet
 
 controla el encendido o apagado de los Pin 2,3,4 ,5 y 6 del Arduino. (Me falta comprobar que los 7,8 y 9 los puedo usar)
 
 */

#include <SPI.h>
#include <Ethernet.h>


int cambio = 0; 
//byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };   //Direccion Fisica MAC
byte mac[] = {0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02};  

 byte ip[] = { 132, 248, 174, 76 };                       // IP para funcionar en oficina 
 byte gateway[] = { 132, 248, 174, 254 };                   // Puerta de enlace
 byte subnet[] = { 255, 255, 255, 0 };                  //Mascara de Sub Red
//byte pingAddr[] = {132,248,174,76}; // ip address to ping
//byte ip[] = { 192, 168, 1, 13 };                       // IP Local que usted debe configurar 
//byte gateway[] = { 192, 168, 1, 254 };                   // Puerta de enlace
//byte subnet[] = { 255, 255, 255, 0 };                  //Mascara de Sub Red

//SOCKET pingSocket = 3;

//char buffer [128];


EthernetServer server(8000);                             //Se usa el puerto 80 del servidor     
String readString;

void setup() {

  Serial.begin(9600);  // Inicializa el puerto serial 
//   while (!Serial) {   // Espera a que el puerto serial sea conectado, Solo necesario para el Leonardo
//    ; 
//  }




  
  pinMode(2,OUTPUT);        // Se configura como salidas los puertos del 2 al 6
  pinMode(3,OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8,OUTPUT);
  pinMode(9,OUTPUT);

  digitalWrite(2, HIGH); // Por dafault prendo los pin para que se apaguen focos
  digitalWrite(3, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);
  digitalWrite(6, HIGH);
  digitalWrite(7, HIGH);
  digitalWrite(8, HIGH);
  digitalWrite(9, HIGH);
 
 
//  Ethernet.begin(mac, ip, gateway, subnet); // Inicializa la conexion Ethernet y el servidor
  Ethernet.begin(mac, ip, gateway, subnet);
//  Ethernet.maintain();
  server.begin();
  Serial.print("El Servidor es: ");
  Serial.println(Ethernet.localIP());    // Imprime la direccion IP Local
}


void loop() {
  // Crea una conexion Cliente
  EthernetClient client = server.available();
  if (client==true) {
    while (client.connected()) {   
      if (client.available()) {
        char c = client.read();
     
        //Lee caracter por caracter HTTP
        if (readString.length() < 100) {
          //Almacena los caracteres a un String
          readString += c;
          
         }

         // si el requerimiento HTTP fue finalizado
        if (c == '\n') {          
           Serial.println(readString); //Imprime en el monitor serial (recomendación encontrada)
     
           client.println("HTTP/1.1 200 OK");           //envia una nueva pagina en codigo HTML
           client.println("Content-Type: text/html");
           client.println();     
           client.println("<HTML>");
           client.println("<HEAD>");
           client.println("<STYLE>");
          client.println(F("p {font-family:verdana;text-align:center;outline: hotpink dotted thick;}"));
           client.println(F("a:link, a:visited {background-color:cornflowerblue;color:white;padding:14px 25px;text-align:center;"));
         client.println(F("text-decoration: none; display: inline-block; border: 2px solid cornflowerblue; font-family:verdana; font-size:30px;position: relative; left: 300px;}"));
       client.println(F("a:hover, a:active { background-color: white; color: gray;}"));
           client.println("</STYLE>");
           client.println("<TITLE>Controlar de luces de Winterfell</TITLE>");
           client.println("</HEAD>");
           client.println("<BODY>");
           client.println(F("<p><b>Control de Luces</b></p>"));
           

          
        
         //control del arduino si un boton es presionado
                   
           if (readString.indexOf("?button2on") >0){
               digitalWrite(2, HIGH);
            client.flush();
            }
           if (readString.indexOf("?button2off") >0){
               digitalWrite(2, LOW);
            client.flush();
          
           }
           
             if (readString.indexOf("?button3on") >0){
               digitalWrite(3, HIGH);
            client.flush();
            }
           if (readString.indexOf("?button3off") >0){
               digitalWrite(3, LOW);
            client.flush();
          
           }

           if (readString.indexOf("?button4on") >0){
               digitalWrite(4, HIGH);
            client.flush();
            }
           if (readString.indexOf("?button4off") >0){
               digitalWrite(4, LOW);
            client.flush();
           }

           if (readString.indexOf("?button5on") >0){
               digitalWrite(5, HIGH);
            client.flush();
            }
           if (readString.indexOf("?button5off") >0){
               digitalWrite(5, LOW);
            client.flush();
           }
            
            // Limpia el String para una nueva lectura
            readString=""; 
            
           
                       
           
          
           if (digitalRead(2)==1){
              client.println(F("<a href=\"/?button2off\"\"> ON&nbsp; Rec&aacute;mara &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;(2)</a><br /><br /> "));   
           }
           else {
              client.println(F("<a style='background-color:white;color:gray;'  href=\"/?button2on\"\"> OFF Rec&aacute;mara &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;(2)</a><br /><br /> "));   
           }

          
           if (digitalRead(3)==1){
              client.println(F("<a href=\"/?button3off\"\"> ON&nbsp; Jard&iacute;n central&nbsp;&nbsp;&nbsp; (3)</a><br /><br /> "));   
           }
           else {
              client.println(F("<a style='background-color:white;color:gray;' href=\"/?button3on\"\"> OFF Jard&iacute;n central &nbsp;&nbsp;&nbsp;(3)</a><br /><br /> "));   
           }

          
           if (digitalRead(4)==1){
              client.println(F("<a href=\"/?button4off\"\"> ON&nbsp; Jard&iacute;n derecho&nbsp; (4)</a><br /><br /> "));   
           }
           else {
              client.println(F("<a style='background-color:white;color:gray;' href=\"/?button4on\"\"> OFF Jard&iacute;n derecho&nbsp;&nbsp;(4)</a><br /><br /> "));   
           }

           
           if (digitalRead(5)==1){
              client.println(F("<a href=\"/?button5off\"\"> ON&nbsp; Jard&iacute;n izquierdo (5)</a><br /><br /> "));   
           }
           else {
              client.println(F("<a style='background-color:white;color:gray;' href=\"/?button5on\"\"> OFF Jard&iacute;n izquierdo (5)</a><br /><br /> "));   
           }
           

// ICMPPing ping(pingSocket);
//  ping(4, pingAddr, buffer);
//  Serial.println(buffer);

                     
           client.println(F("<p>Por Rubencio Pipluto</p>")); 
           client.println("</BODY>");
           client.println("</HTML>");
     
           delay(1);
           //detiene el cliente servidor
           client.stop();
           
  
       
         }
       }
    }
//     ICMPPing ping(pingSocket);
//  ping(4, pingAddr, buffer);
//  Serial.println(buffer);
}
}
