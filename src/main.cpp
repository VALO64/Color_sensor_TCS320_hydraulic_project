#include <Arduino.h>
#include <TCS320.h>
#include <BluetoothSerial.h>

// Oscar Alberto Valles Limas 

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
#define BLUE_DETECTION    4
#define RED_DETECTION     5
int mode = MODE_NORMAL; // Comienza en modo normal

// Objeto Bluetooth
BluetoothSerial SerialBT;

// Declaraciones de funciones
void colordiferences();

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
    switch (input) {
      case 'c':
        mode = MODE_CALIBRATION;
        Serial.println("Modo calibración seleccionado.");
        break;
      case 'u':
        mode = MODE_NORMAL;
        Serial.println("Modo normal seleccionado.");
        break;
      case 'v':
        mode = GREEN_DETECTION;
        break;
      case 'a':
        mode = BLUE_DETECTION;
        break;
      case 'r':
        mode = RED_DETECTION;
        break;
    }
  }

  // Verificar si hay datos disponibles en el Bluetooth
if (SerialBT.available() > 0) {
    String btInput = SerialBT.readStringUntil('\n'); // Leer el comando hasta el salto de línea
    if (btInput == "c") {
        mode = MODE_CALIBRATION;
        Serial.println("Modo calibración seleccionado.");
        SerialBT.println("Modo calibración seleccionado.");
    } else if (btInput == "u") {
        mode = MODE_NORMAL;
        Serial.println("Modo normal seleccionado.");
        SerialBT.println("Modo normal seleccionado.");
    } else if (btInput == "g") {
        mode = GREEN_DETECTION;
        Serial.println("Modo detección verde seleccionado.");
        SerialBT.println("Modo detección verde seleccionado.");
    } else if (btInput == "b") {
        mode = BLUE_DETECTION;
        Serial.println("Modo detección azul seleccionado.");
        SerialBT.println("Modo detección azul seleccionado.");
    } else if (btInput == "r") {
        mode = RED_DETECTION;
        Serial.println("Modo detección rojo seleccionado.");
        SerialBT.println("Modo detección rojo seleccionado.");
    } else {
        Serial.println("Comando desconocido.");
        SerialBT.println("Comando desconocido.");
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

  // Modo detección de color específico
  if (mode == GREEN_DETECTION) {
    colordiferences();
    if (greenColor > redColor && greenColor > blueColor) {
      digitalWrite(PISTON, HIGH);
      Serial.println(" - GREEN detected!");
      delay(1000);
    } else {
      digitalWrite(PISTON, LOW);
      Serial.println(" - not GREEN detected!");
    }
    delay(500);
  }
  if (mode == BLUE_DETECTION) {
    colordiferences();
    if (blueColor > redColor && blueColor > greenColor) {
      digitalWrite(PISTON, HIGH);
      Serial.println(" - BLUE detected!");
      delay(1000);
    } else {
      digitalWrite(PISTON, LOW);
      Serial.println(" - not BLUE detected!");
    }
    delay(500);
  }
  if (mode == RED_DETECTION) {
    colordiferences();
    if (redColor > greenColor && redColor > blueColor) {
      digitalWrite(PISTON, HIGH);
      Serial.println(" - RED detected!");
    } else {
      digitalWrite(PISTON, LOW);
      Serial.println(" - not RED detected!");
    }
    delay(500);
  }
}

void colordiferences() {
  colorSensor.readAll(redColor, greenColor, blueColor);
  // Remapear los valores de frecuencia a valores RGB de 0 a 255
  redColor = map(redColor, 134, 148, 169, 0);
  greenColor = map(greenColor, 185, 230, 284, 0);
  blueColor = map(blueColor, 210, 285, 333, 0);
}
