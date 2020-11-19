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
  pinMode(6,INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledverde, OUTPUT);
  pinMode(ledamarillo, OUTPUT);
  pinMode(ledrojo, OUTPUT);
  pinMode(activar, OUTPUT); 
  Serial.begin(9600);

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
  digitalWrite(activar, estado);
  digitalWrite(ledverde, LOW);
  digitalWrite(ledrojo, LOW);
  digitalWrite(ledamarillo, HIGH);
}
if (distancia>=15 && distancia <=150){
  digitalWrite(activar, estado);
  digitalWrite(ledverde, LOW);
  digitalWrite(ledamarillo, LOW);
  digitalWrite(ledrojo, HIGH);
}
}

void Controles(){
    if (Serial.available() >= 0) {

    input = Serial.read();

    switch(input){
      case '1':
          digitalWrite(activar,!estado);
          
      break;

      case '0':
          digitalWrite(activar,estado);
      break;

      default:
          digitalWrite(activar,estado);
      break;
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
