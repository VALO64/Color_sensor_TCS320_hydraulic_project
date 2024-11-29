#include <Arduino.h>
#include <TCS320.h>
#include <BluetoothSerial.h>
// Definir los pines para el sensor
#define S0 18
#define S1 19
#define S2 21
#define S3 22
#define PISTON 5
#define SENSOR_OUT 23

// Crear una instancia del sensor de color
TCS320 colorSensor(S0, S1, S2, S3, SENSOR_OUT);

// Variables para almacenar los valores de los colores
int redColor = 0;
int greenColor = 0;
int blueColor = 0;

// Variables para calibración
int red, green, blue;

// Definir los modos
#define MODE_CALIBRATION  1
#define MODE_NORMAL       2
#define GREEN_DETECTION   3
int mode = MODE_NORMAL; // Comienza en modo normal

//Objeto Bluetooth
BluetoothSerial SerialBT;
//Declaraciones de funciones 
void colordiferences ();

void setup() {
  // Inicializar la comunicación serial
  Serial.begin(9600);
  // Iniciar Bluetooth con el nombre del dispositivo
  SerialBT.begin("ESP32_Hidraulica"); // Cambia el nombre si lo deseas
  Serial.println("Bluetooth iniciado, listo para emparejar");
  // Inicializar el sensor
  colorSensor.begin();
  pinMode(PISTON, OUTPUT);
  // Esperar una instrucción para elegir el modo
  Serial.println("Selecciona el modo:");
  Serial.println("c - Modo calibración");
  Serial.println("u - Modo normal");
}

void loop() {
  // Verificar si hay datos disponibles en el puerto serial
  if (Serial.available() > 0) {
    char input = Serial.read();
    
    // Cambiar de modo según la entrada del usuario
    if (input == 'c') {
      mode = MODE_CALIBRATION;
      Serial.println("Modo calibración seleccionado.");
    } else if (input == 'u') {
      mode = MODE_NORMAL;
      Serial.println("Modo normal seleccionado.");
    } else if (input == 'g') {
      mode = GREEN_DETECTION;
    }
  if (SerialBT.available() > 0){
    String mode = SerialBT.readStringUntil('\n'); // Leer el comando hasta el salto de línea
    // Cambiar de modo según la entrada del usuario
    if (input == 'c') {
      mode = MODE_CALIBRATION;
      Serial.println("Modo calibración seleccionado.");
    } else if (input == 'u') {
      mode = MODE_NORMAL;
      Serial.println("Modo normal seleccionado.");
    } else if (input == 'g') {
      mode = GREEN_DETECTION;
    }
  }
}
  // Modo calibración
  if (mode == MODE_CALIBRATION) {
    // Leer los colores
    colorSensor.readAll(red, green, blue);

    // Imprimir los valores de calibración
    Serial.print("R = ");
    Serial.print(red);
    Serial.print(" G = ");
    Serial.print(green);
    Serial.print(" B = ");
    Serial.println(blue);

    delay(500);  // Espera de medio segundo para nuevas lecturas
  }

  // Modo normal (uso del sensor)
  if (mode == MODE_NORMAL) {
    // Leer los colores
    colordiferences();
    // Imprimir los valores de los colores
    Serial.print("R = ");
    Serial.print(redColor);
    Serial.print(" G = ");
    Serial.print(greenColor);
    Serial.print(" B = ");
    Serial.println(blueColor);

    // Detectar el color predominante
    if (redColor > greenColor && redColor > blueColor) {
      Serial.println(" - RED detected!");
    } else if (greenColor > redColor && greenColor > blueColor) {
      Serial.println(" - GREEN detected!");
    } else if (blueColor > redColor && blueColor > greenColor) {
      Serial.println(" - BLUE detected!");
    }

    delay(500);  // Espera de medio segundo para la siguiente lectura
  }
    if(mode == GREEN_DETECTION){

    colordiferences();

      if(greenColor > redColor && greenColor > blueColor){
        digitalWrite(PISTON, HIGH);
        Serial.println(" - GREEN detected!");
      }else{
        digitalWrite(PISTON, LOW);
        Serial.println(" - not GREEN detected!");
      }
    delay(500);
    }
}
void colordiferences (){
    colorSensor.readAll(redColor, greenColor, blueColor);

    // Remapear los valores de frecuencia a valores RGB de 0 a 255
    redColor = map(redColor, 91, 246, 221, 0);
    greenColor = map(greenColor, 158, 194, 198, 0); //17, 84, 108, 0
    blueColor = map(blueColor, 167, 170, 139, 0); //121, 127, 146, 0

}
