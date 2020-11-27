//Pines para comunicacion Wifi
#include <SoftwareSerial.h>
SoftwareSerial BT1(13, 12); // RX | TX
//--------------------------------------
//-----Sensor de efecto hall------
double periodo = 60000; //Periodo de un minuto
int contador = 0;
unsigned long tiempoAhora = 0;
int valorFijado = 0;
int rpm = 0;
//-------------------------------

//----sensor ultrasonico-------
const int trigPin = 9;  //triger del sensor ultrasonico
const int echoPin = 10; //echo del sensor ultrasonico
long duracion;
int distancia;
//-----------------------------

//-----leds......
int ledverde = 2; 
int ledamarillo = 3;
int ledrojo = 4;
//---------------

//-----Recibimiento de datos------
int activar = 5;  //pin que activa los motores y sensor de efecto hall
int input; //variable donde se guarda lo que recibe el pin 5
//--------------------------------------

//---------Multitarea con Millis()----------
bool estado=false;
unsigned long tiempo=0;

void setup() {
  BT1.begin(9600);
  pinMode(6,INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledverde, OUTPUT);
  pinMode(ledamarillo, OUTPUT);
  pinMode(ledrojo, OUTPUT);
  pinMode(activar, OUTPUT); 
  Serial.begin(9600);

  //Configuracion de parametros para conectarse a la red

  BT1.println("AT+CWMODE=3");
  delay(1000);
  BT1.println("AT+CWJAP=\"Villeda\",\"FamVilleda2020\"");
  delay(10000);
  BT1.println("AT+CIPMUX=1");
  delay(1000);
  BT1.println("AT+CIPSERVER=1,80");
  delay(1000);

}

void loop() {

   Controles();
   Distancia();
   Medidor_RPM();

}

void Distancia(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duracion = pulseIn(echoPin, HIGH);
  distancia = (0.034 * duracion)/2;
 
if (distancia>=5 && distancia <=7){
  digitalWrite(ledverde, HIGH);
  digitalWrite(ledamarillo, LOW);
  digitalWrite(ledrojo, LOW);
}
if (distancia>=11 && distancia <=15){
  digitalWrite(ledverde, LOW);
  digitalWrite(ledrojo, LOW);
  digitalWrite(ledamarillo, HIGH);
}
if (distancia>=15 && distancia <=150){
  
  digitalWrite(ledverde, LOW);
  digitalWrite(ledamarillo, LOW);
  digitalWrite(ledrojo, HIGH);
}
}

void Controles(){

  if (BT1.available()){
      delay(1000);
      if(BT1.find("+IPD,")){
        int conexion = BT1.read()-48;
        if(BT1.find("motor=")){
          int lectura = BT1.read()-48;

          String pagina = "<!doctype html><html><head></head><body>";

          if(lectura == 1){
            digitalWrite(activar,!estado);
            pagina += "<h1>Motores = encendidos</h1></body></html>";
          }
          else if (lectura == 0){
            digitalWrite(activar,estado);
            pagina += "<h1>Motores = apagado</h1></body></html>";
          }

          String enviar = "AT+CIPSEND=";
          enviar+=conexion;
          enviar+=",";
          enviar+=pagina.length();
          BT1.println(enviar);
          delay(1000);
          BT1.println(pagina);
          delay(1000);

          String cerrar = "AT+CIPCLOSE=";
          cerrar+=conexion;
          BT1.println(cerrar);
          delay(2000);
        }
      }
   }
}

void Medidor_RPM(){
  //Cada un minuto imprime lo que este aca
  
    if(millis() > (periodo + tiempoAhora)){
    valorFijado = contador; //Valor fijado se vuelve contador para dejar fijado por un minuto lo que estaba.
    tiempoAhora = millis();
    contador=0; //Reiniciamos contador
  }

  //Detecto iman lo cual es igual a una vuelta
  if(digitalRead(6) == 0){
      //Espero 300 milisegundos para que no se acumulen muchas vueltas.
      delay(300);
      //Aumento el contador de vueltas en uno
      contador++;
  }
   rpm = contador;
   
   Serial.println(rpm);
 
}
