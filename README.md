# arduino_luces_1_9_3
Arduino control de luces por Internet 
Items:
1 Arduino UNO $200
1 Arduino Ethernet Shield W5100 $200
2 modulos relevadores para 4 canales electromecánicos $100 x 2 (recomiendo usar de estado sólido porque me empiezan a dar lata)
1 pulsador $1
1 protoboard $30

Descripción:
 Control, a través de Internet, el encendido y apagado de los Pines 2, 3, 5 y 6 conectados al módulo relé A; y 7 conectado al módulo relé B del Arduino UNO con Ethernet Shield.
 El pin 7 también se controla mediante un pulsador en el pin A0.
 El pin 4 no se usa para evitar conflicto con la SD del Ethernet Shield.
 Se define el pin A3 como output digital para led de control.
 Se prueba control de Leds con multiplexación. 

Para el deployment se usa No-IP como servicio ddns y en el modem se establecen las reglas de asignación de IP por control de MAC.
