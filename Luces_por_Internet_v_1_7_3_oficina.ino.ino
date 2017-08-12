/* Control Arduino Ethernet
 *  1.7.3
 
 controla el encendido o apagado de los Pin 2,3,4 ,5 y 6 del Arduino. (Me falta comprobar que los 7,8 y 9 los puedo usar)
 
 */

#include <SPI.h>
#include <Ethernet.h>

//byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };   //Direccion Fisica MAC
//byte mac[] = {0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02};  
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED  
 };
IPAddress ip(132,248,174,76);                       // IP para funcionar en oficina 
// byte gateway[] = { 132, 248, 174, 254 };                   // Puerta de enlace
// byte subnet[] = { 255, 255, 255, 0 };                  //Mascara de Sub Red
//byte pingAddr[] = {132,248,174,76}; // ip address to ping
//byte ip[] = { 192, 168, 1, 13 };                       // IP Local que usted debe configurar 
//byte gateway[] = { 192, 168, 1, 254 };                   // Puerta de enlace
//byte subnet[] = { 255, 255, 255, 0 };                  //Mascara de Sub Red



EthernetServer server(80);                             //Se usa el puerto 80 del servidor     
String readString;
boolean algunprendido = false;

void setup() {

  Serial.begin(9600);  // Inicializa el puerto serial 
//   while (!Serial) {   // Espera a que el puerto serial sea conectado, Solo necesario para el Leonardo
//    ; 
//  }




  
  pinMode(2,OUTPUT);        // Se configura como salidas los puertos del 2 al 6
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(8,OUTPUT);
  pinMode(9,OUTPUT);

  digitalWrite(2, LOW); // Por dafault pongo en LOW todos los pines para que estén apagados
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
 
 
//  Ethernet.begin(mac, ip, gateway, subnet); // Inicializa la conexion Ethernet y el servidor
   Ethernet.begin(mac, ip);
//   Ethernet.maintain();
   server.begin();
   Serial.print("El Servidor es: ");
   Serial.println(Ethernet.localIP());    // Imprime la direccion IP Local
   digitalWrite(9,HIGH);
}


void loop() {
  // Crea una conexion Cliente
  EthernetClient client = server.available();
  if (client) {
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
          Serial.println(readString); //Imprime en el monitor serial (recomendaciÃ³n encontrada)
          // Genero pagina   
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
          //   client.flush();
          }
          if (readString.indexOf("?button2off") >0){
               digitalWrite(2, LOW);
          //   client.flush();
          }
          if (readString.indexOf("?button3on") >0){
               digitalWrite(3, HIGH);
          //   client.flush();
          }
          if (readString.indexOf("?button3off") >0){
               digitalWrite(3, LOW);
          //   client.flush();
          }
          if (readString.indexOf("?button4on") >0){
               digitalWrite(4, HIGH);
          //   client.flush();
          }
          if (readString.indexOf("?button4off") >0){
               digitalWrite(4, LOW);
          //   client.flush();
          }
          if (readString.indexOf("?button5on") >0){
               digitalWrite(5, HIGH);
          //   client.flush();
          }
          if (readString.indexOf("?button5off") >0){
               digitalWrite(5, LOW);
          //   client.flush();
          }
          if (readString.indexOf("?buttontodoson") >0){
               digitalWrite(2, HIGH);
               digitalWrite(3, HIGH);
               digitalWrite(4, HIGH);
               digitalWrite(5, HIGH);
          //   client.flush();
          }
          if (readString.indexOf("?buttontodosoff") >0){
               digitalWrite(2, LOW);
               digitalWrite(3, LOW);
               digitalWrite(4, LOW);
               digitalWrite(5, LOW);
          //   client.flush();
          }  
          // Limpia el String para una nueva lectura
          readString="";
          algunprendido=false; 

          // Leo el estatus de cada pin para actualizar sus botones
          if (digitalRead(2)==LOW){
              client.println(F("<a href=\"/?button2on\"\"> ON&nbsp; Rec&aacute;mara &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;(2)</a><br /><br /> "));   
           }
           else {
              client.println(F("<a style='background-color:white;color:gray;'  href=\"/?button2off\"\"> OFF Rec&aacute;mara &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;(2)</a><br /><br /> "));   
              algunprendido=true;
           }

           if (digitalRead(3)==LOW){
              client.println(F("<a href=\"/?button3on\"\"> ON&nbsp; Jard&iacute;n central&nbsp;&nbsp;&nbsp; (3)</a><br /><br /> "));   
           }
           else {
              client.println(F("<a style='background-color:white;color:gray;' href=\"/?button3off\"\"> OFF Jard&iacute;n central &nbsp;&nbsp;&nbsp;(3)</a><br /><br /> "));   
              algunprendido=true;
           }
          
           if (digitalRead(4)==LOW){
              client.println(F("<a href=\"/?button4on\"\"> ON&nbsp; Jard&iacute;n derecho&nbsp; (4)</a><br /><br /> "));   
           }
           else {
              client.println(F("<a style='background-color:white;color:gray;' href=\"/?button4off\"\"> OFF Jard&iacute;n derecho&nbsp;&nbsp;(4)</a><br /><br /> "));   
              algunprendido=true;
           }
           
           if (digitalRead(5)==LOW){
              client.println(F("<a href=\"/?button5on\"\"> ON&nbsp; Jard&iacute;n izquierdo (5)</a><br /><br /> "));   
           }
           else {
              client.println(F("<a style='background-color:white;color:gray;' href=\"/?button5off\"\"> OFF Jard&iacute;n izquierdo (5)</a><br /><br /> "));   
              algunprendido=true;
           }
           if (algunprendido==false) {
              client.println(F("<a href=\"/?buttontodoson\"\"> ON&nbsp; Todos </a><br /><br /> "));   
           }
           else
          {
              client.println(F("<a style='background-color:white;color:gray;' href=\"/?buttontodosoff\"\"> OFF Todos ()</a><br /><br /> "));    
          }
           client.println(F("<p>By Rubencio Pipluto for Pinky</p>")); 
           client.println("</BODY>");
           client.println("</HTML>");
           // Se da tiempo para que el cliente lea     
           delay(1);
           //detiene el cliente servidor
           client.stop();
       
         }
       }
    }

  }
}

