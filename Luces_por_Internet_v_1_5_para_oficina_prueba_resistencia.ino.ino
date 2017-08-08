/* Control Arduino Ethernet
 
 controla el encendido o apagado de los Pin 2,3,4 ,5 y 6 del Arduino. (Me falta comprobar que los 7,8 y 9 los puedo usar)
 
 */

#include <SPI.h>
#include <Ethernet.h>


//Direccion Fisica MAC
//byte mac[] = {0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02};
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED  
 };


EthernetServer server(80);                             //Se usa el puerto 80 del servidor     
String readString;

void setup() {

  Serial.begin(9600);  // Inicializa el puerto serial 
//   while (!Serial) {   // Espera a que el puerto serial sea conectado, Solo necesario para el Leonardo
//    ; 
//  }

// Se configura como salidas los puertos del 2 al 9
  pinMode(2,OUTPUT);        
  pinMode(3,OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8,OUTPUT);
  pinMode(9,OUTPUT);

// Por default prendo los pin para que se apaguen focos
// tengo que probar digitalWrite(2,Relay_ON);
  digitalWrite(2, HIGH); 
  digitalWrite(3, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);
  digitalWrite(6, HIGH);
  digitalWrite(7, HIGH);
  digitalWrite(8, HIGH);
  digitalWrite(9, HIGH);
 
 
//  Ethernet.begin(mac, ip, gateway, subnet); // Inicializa la conexion Ethernet y el servidor
  Ethernet.begin(mac);
  Ethernet.maintain();
  server.begin();
  Serial.print("El Servidor es: ");
  Serial.println(Ethernet.localIP());    // Imprime la direccion IP Local
}


void loop() {
  // Crea una conexion Cliente
  EthernetClient client = server.available();
  if (client) {
    Serial.println("Un nuevo cliente conectado");
    while (client.connected()) {   
      if (client.available()) {
        char c = client.read();
        
        Serial.write(c);
     
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
//           client.println("Connection: close");  // the connection will be closed after completion of the response
//          client.println("Refresh: 5");  // refresh the page automatically every 5 sec
           client.println();
           client.println("<!DOCTYPE HTML>");
           client.println("<HTML>");
           client.println("<HEAD>");
           client.println(F("<STYLE>")); //uso la función F() para que las variables strings sean almacenadas en la memoria flash
           client.println(F("p {font-family:verdana;text-align:center;outline: hotpink dotted thick;}"));
           client.println(F("a:link, a:visited {background-color:cornflowerblue;color:white;padding:14px 25px;text-align:center;"));
           client.println(F("text-decoration: none; display: inline-block; border: 2px solid cornflowerblue; font-family:verdana; font-size:30px;position: relative; left: 300px;}"));
           client.println(F("a:hover, a:active { background-color: white; color: gray;}"));
           client.println(F("</STYLE>"));
           client.println(F("<TITLE>Controlar de luces de Winterfell</TITLE>"));
           client.println("</HEAD>");
           client.println("<BODY>");
           client.println(F("<p><b>Control de luces de Winterfell</b></p>"));
           

         //control del arduino si un boton es presionado
                   
           if (readString.indexOf("?button2on") >0){
               digitalWrite(2, HIGH);
           //    client.println(F("<a href=\"/?button2off\"\"> ON1&nbsp; Rec&aacute;mara &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;(2)</a><br /><br /> "));   
    
              // client.flush();
              
              
           }
           if (readString.indexOf("?button2off") >0){
               digitalWrite(2, LOW);
             //   client.println(F("<a style='background-color:white;color:gray;'  href=\"/?button2on\"\"> OFF1 Rec&aacute;mara &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;(2)</a><br /><br /> "));   

              // client.flush();
              
           }
            // Limpia el String para una nueva lectura
           readString=""; 
           
          
           if (digitalRead(2)==1){
              client.println(F("<a href=\"/?button2off\"\"> ON&nbsp; Rec&aacute;mara &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;(2)</a><br /><br /> "));   
              
           }
           else {
              client.println(F("<a style='background-color:white;color:gray;'  href=\"/?button2on\"\"> OFF Rec&aacute;mara &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;(2)</a><br /><br /> "));   
           }

                     
           client.println("<p>Por Rubencio para Pinky</p>"); 
           client.println("</BODY>");
           client.println("</HTML>");
           
 //          break;   
           delay(1);
//           detiene el cliente servidor
           client.stop();
           
//           Serial.println("Dentro del c==n");
  
       
         }
//         Serial.println("Salgo del c==n y esto en el client available  ");
         
       }
//       Serial.println("Salgo del client available y estoy en client connected");
    }
 //   delay(1);
 //   client.stop();
 //   Serial.println("Cliente desconectado");
  }
}
