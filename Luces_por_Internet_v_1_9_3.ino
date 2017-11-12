

/* Control Arduino Ethernet
 *  1.9.3
 
1 Arduino UNO $200
1 Arduino Ethernet Shield W5100 $200
2 modulos relevadores para 4 canales electromecánicos $100 x 2 (recomiendo usar de estado sólido porque me empiezan a dar lata)
1 pulsador $1
1 protoboard $30
 
 Control, a través de Internet, el encendido y apagado de los Pines 2, 3, 5 y 6 conectados al módulo relé A; y 7 conectado al módulo relé B  del Arduino UNO con Ethernet Shield.
 El pin 7 también se controla mediante un pulsador en el pin A0.
 El pin 4 no se usa para evitar conflicto con la SD del Ethernet Shield.
 Se define el pin A3 como output digital para led de control.
 Se prueba control de Leds com multiplexación. 
 */

#include <SPI.h>
#include <Ethernet.h>
 
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED  
 };

EthernetServer server(80);                             //Se usa el puerto 80 del servidor     
String readString;
boolean algunprendido = false;
boolean Bailar = false;

int JARDIN_LATERAL = 2;
int JARDIN_CENTRAL = 3;
int CAMINADORA = 5;
int PRINCIPAL = 6;
int RECAMARA = 7;
int BOTON_RECAMARA = A0;
int LED_READY = A3;
int on = LOW;
int off = HIGH;
int PIN_A=8;
int PIN_B=9;
int PIN_C=A5;

void setup() {

  Serial.begin(9600);  // Inicializa el puerto serial 

  // Se configuran las salidas y entradas
  
  pinMode(JARDIN_LATERAL,OUTPUT);
  pinMode(JARDIN_CENTRAL,OUTPUT);
  pinMode(CAMINADORA,OUTPUT);
  pinMode(PRINCIPAL,OUTPUT);
  pinMode(RECAMARA,OUTPUT);
  pinMode(BOTON_RECAMARA,INPUT); //Pulsador de la recámara
  pinMode(LED_READY,OUTPUT); // Led que me avisa cuando el sistema está operacional

  // Valores predeterminado a off para que estén apagados
  digitalWrite(JARDIN_LATERAL,off);
  digitalWrite(JARDIN_CENTRAL,off);
  digitalWrite(CAMINADORA,off);
  digitalWrite(PRINCIPAL,off);
  digitalWrite(RECAMARA,off);
    
  Ethernet.begin(mac); // al incluir sólo el parámetro mac gestionará DHCP
  Ethernet.maintain();
  server.begin();
  Serial.print("El Servidor es: ");
  Serial.println(Ethernet.localIP());    // Imprime la direccion IP Local
  digitalWrite(LED_READY,HIGH); // prendo el led de control para saber que el servidior está listo
  Bailar=false;
  randomSeed(analogRead(1));
}

// Función que pondrá en los estados correctos para encender un LED (HIGH, LOW e INPUT) para multiplexación
void ponerEstados(int pinHigh, int pinLow, int pinInput)
{
    pinMode(pinHigh,OUTPUT);
    digitalWrite(pinHigh,HIGH);
    pinMode(pinLow,OUTPUT);
    digitalWrite(pinLow,LOW);
    pinMode(pinInput,INPUT);
}
 
// Esta función se va a encargar de aplicar la lógica dependiendo del LED que queramos encender a través de multiplexación
void encenderLed(int led_num)
{
    switch(led_num)
    {
        case 0:
            pinMode(PIN_A,OUTPUT);
            digitalWrite(PIN_A,LOW);
            pinMode(PIN_B,OUTPUT);
            digitalWrite(PIN_B,LOW);
            pinMode(PIN_C,OUTPUT);
            digitalWrite(PIN_C,LOW);
            break;
        case 1:
//            ponerEstados(PIN_B,PIN_A,PIN_C);
            ponerEstados(PIN_A,PIN_B,PIN_C);
            break;
        case 2:
//            ponerEstados(PIN_A,PIN_B,PIN_C);
            ponerEstados(PIN_B,PIN_A,PIN_C);
            break;
        case 3:
//            ponerEstados(PIN_C,PIN_B,PIN_A);
            ponerEstados(PIN_B,PIN_C,PIN_A);
            break;
        case 4:
//            ponerEstados(PIN_B,PIN_C,PIN_A);
            ponerEstados(PIN_C,PIN_B,PIN_A);
            break;
        case 5:
//            ponerEstados(PIN_C,PIN_A,PIN_B);
            ponerEstados(PIN_A,PIN_C,PIN_B);
            break;
        case 6:
//            ponerEstados(PIN_A,PIN_C,PIN_B);
            ponerEstados(PIN_C,PIN_A,PIN_B);
            break;
    }
    
}

void Baile1(){
       
     // Bucle para encender los LEDs de menor a mayor
      for(int i=1; i<=6; i++)
      {
        encenderLed(i);
        delay(100);
      }
    
      // Bucle para encender los LEDs de mayor a menor eliminando lo extremos
      for(int i=5; i>1; i--)
      {
        encenderLed(i);
        delay(100);
      }
     
}


void Baile2(){
for (int veces=1; veces<=2; veces++) {  
 for(int i=1; i<=10; i++){
    encenderLed(3);
    delay(10);
    encenderLed(6);
    delay(10);
 }
 for(int i=1; i<=10; i++){
    encenderLed(2);
    delay(10);
    encenderLed(5);
    delay(10);
 }
 for(int i=1; i<=10; i++){
    encenderLed(1);
    delay(10);
    encenderLed(4);
    delay(10);
 }
} 
}


void Baile3(){
for (int veces=1; veces<=2; veces++) {  
 for(int i=1; i<=10; i++){
    encenderLed(1);
    delay(10);
    encenderLed(6);
    delay(10);
 }
 for(int i=1; i<=10; i++){
    encenderLed(2);
    delay(10);
    encenderLed(5);
    delay(10);
 }
 for(int i=1; i<=10; i++){
    encenderLed(3);
    delay(10);
    encenderLed(4);
    delay(10);
 }
} 
}


// función para que aleatoriamente se ejecute un baile diferente
void BaileLed(){
  byte QueBaile=2; 

  QueBaile=random(1,4);
  switch (QueBaile){
    case 1:
      Baile1();
      break;
    case 2:
      Baile2();
      break;
    case 3:
      Baile3();
      break;  
  }
  
}


void LeePulsador(){
  //Leo el Pulsador de la recámara cuando se oprime
  if (digitalRead(BOTON_RECAMARA)==1){
    if (digitalRead(RECAMARA)==off){
      digitalWrite(RECAMARA,on);
    }
    else {
      digitalWrite(RECAMARA,off);
    }
    delay(300); //se requiere para la lectura del pulsador
  }
}

void GeneraPaginaPrincipal(EthernetClient &client){
  // Genero pagina Web
  client.println("HTTP/1.1 200 OK");           //envia una nueva pagina en codigo HTML
  client.println("Content-Type: text/html");
  client.println();     
  client.println("<HTML>");
  client.println("<HEAD>");
  // Se refresca cada 30 segundos para saber el estado de la RECAMARA ya que pudo ser cambiado por pulsador
  client.println("<meta http-equiv='refresh' content='30;url=/' />");
  client.println("<STYLE>");
  // se usa la funcion F() para que se haga la asignación en la memoria flash
  client.println(F("p.puntos {font-family:'Open Sans',sans-serif; font-size:28px; text-align:center;outline: hotpink ridge thick; border: 5px dotted hotpink;background-color:white; padding: 20px;}"));
  client.println(F("a:link, a:visited {font-family:'Open Sans', sans-serif; background-color:cornflowerblue;color:white;padding:30px 30px 30px 60px; text-align:left; width:510px; text-transform: uppercase;"));
  client.println(F("text-decoration: none; display: inline-block; border: 2px outset cornflowerblue; font-size: 2em;position: relative; left: 200px;}"));
  client.println(F("a:hover, a:active { background-color: white; color: gray;}"));
  client.println("</STYLE>");
  client.println("<TITLE>Control de Winterfell</TITLE>");
  client.println("</HEAD>");
  client.println("<BODY>");
  client.println(F("<p class='puntos'><b>Control de Luces</b></p>"));
           
  //control del arduino si un boton web es presionado
                   
  if (readString.indexOf("?button1on") >0){
    digitalWrite(RECAMARA, on);
  }
  if (readString.indexOf("?button1off") >0){
    digitalWrite(RECAMARA, off);
  }
  if (readString.indexOf("?button2on") >0){
    digitalWrite(JARDIN_LATERAL, on);
  }
  if (readString.indexOf("?button2off") >0){
    digitalWrite(JARDIN_LATERAL, off);
  }
  if (readString.indexOf("?button3on") >0){
    digitalWrite(JARDIN_CENTRAL, on);
  }
  if (readString.indexOf("?button3off") >0){
    digitalWrite(JARDIN_CENTRAL, off);
  }
  if (readString.indexOf("?button4on") >0){
    digitalWrite(CAMINADORA, on);
  }
  if (readString.indexOf("?button4off") >0){
    digitalWrite(CAMINADORA, off);
  }
  if (readString.indexOf("?button5on") >0){
    digitalWrite(PRINCIPAL, on);
    Bailar=true;
  }
  if (readString.indexOf("?button5off") >0){
    digitalWrite(PRINCIPAL, off);
    Bailar=false;
  }
  if (readString.indexOf("?buttontodoson") >0){
    digitalWrite(RECAMARA, on);
    delay(2000);
    digitalWrite(CAMINADORA, on);
    delay(2000);
    digitalWrite(JARDIN_CENTRAL, on);
    delay(2000);
    digitalWrite(JARDIN_LATERAL, on);
    delay(2000);
    digitalWrite(PRINCIPAL, on);
    Bailar=true;
  }
  if (readString.indexOf("?buttontodosoff") >0){
    digitalWrite(RECAMARA, off);
    digitalWrite(JARDIN_LATERAL, off);
    digitalWrite(JARDIN_CENTRAL, off);
    digitalWrite(CAMINADORA, off);
    digitalWrite(PRINCIPAL, off);
    Bailar=false;
  }  
        
  // Limpia el String para una nueva lectura
  readString="";
  algunprendido=false; 
  
  // Leo el estatus de cada pin para actualizar sus botones
  if (digitalRead(RECAMARA)==off){
    client.println(F("<a href=\"/?button1on\"\">Rec&aacute;mara &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;ON(2)</a><br /><br /> "));   
  }
  else {
    client.println(F("<a style='background-color:white;color:gray;'  href=\"/?button1off\"\">Rec&aacute;mara&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;OFF(2)</a><br /><br /> "));   
    algunprendido=true;
  }

  if (digitalRead(JARDIN_LATERAL)==off){
    client.println(F("<a href=\"/?button2on\"\">Jard&iacute;n Lateral&nbsp;&nbsp;&nbsp;&nbsp;ON (3)</a><br /><br /> "));   
  }
  else {
    client.println(F("<a style='background-color:white;color:gray;' href=\"/?button2off\"\">Jard&iacute;n Lateral &nbsp;&nbsp;&nbsp;OFF (3)</a><br /><br /> "));   
    algunprendido=true;
  }
          
  if (digitalRead(JARDIN_CENTRAL)==off){
    client.println(F("<a href=\"/?button3on\"\">Jard&iacute;n Central&nbsp;ON(5)</a><br /><br /> "));   
  }
  else {
    client.println(F("<a style='background-color:white;color:gray;' href=\"/?button3off\"\">Jard&iacute;n Central OFF (5)</a><br /><br /> "));   
    algunprendido=true;
  }
           
  if (digitalRead(CAMINADORA)==off){
    client.println(F("<a href=\"/?button4on\"\">Caminadora&nbsp;ON (6)</a><br /><br /> "));   
  }
  else {
    client.println(F("<a style='background-color:white;color:gray;' href=\"/?button4off\"\">Caminadora OFF (6)</a><br /><br /> "));   
    algunprendido=true;
  }
  
  if (digitalRead(PRINCIPAL)==off){
    client.println(F("<a href=\"/?button5on\"\">Entrada principal&nbsp;ON (6)</a><br /><br /> "));   
  }
  else {
    client.println(F("<a style='background-color:white;color:gray;' href=\"/?button5off\"\">Entrada principal OFF (6)</a><br /><br /> "));   
    algunprendido=true;
  }

  // Botón para control de todas las luces
  if (algunprendido==false) {
    client.println(F("<a href=\"/?buttontodoson\"\">Todas las luces ON&nbsp; </a><br /><br /> "));   
  }
  else {
    client.println(F("<a style='background-color:white;color:gray;' href=\"/?buttontodosoff\"\">Todas las luces OFF</a><br /><br /> "));    
  }
  
  client.println(F("<p class='puntos'>By Dodgson Pipluto for Pinky V 1.9.3</p>")); 
  client.println("</BODY>");
  client.println("</HTML>");

}


void loop() {
  // compruebo si se opime el pulsador  
  LeePulsador();       
    
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
          Serial.println(readString); //Imprime en el monitor serial 
           
          GeneraPaginaPrincipal(client);  
          // Se da tiempo para que el cliente lea     
           delay(1);
           //detiene el cliente servidor
           client.stop();
       
        }
      }
    }
  }
  
  if (Bailar==true) {BaileLed();}
}

