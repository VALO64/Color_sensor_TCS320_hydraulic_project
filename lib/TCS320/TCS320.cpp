#include "TCS320.h"

// Constructor
TCS320::TCS320(int s0, int s1, int s2, int s3, int out) 
    : S0(s0), S1(s1), S2(s2), S3(s3), sensorOut(out),
      redFrequency(0), greenFrequency(0), blueFrequency(0) {}

// Inicializar el sensor
void TCS320::begin() {
    pinMode(S0, OUTPUT);
    pinMode(S1, OUTPUT);
    pinMode(S2, OUTPUT);
    pinMode(S3, OUTPUT);
    pinMode(sensorOut, INPUT);
    
    // Configurar el escalado de frecuencia al 20%
    digitalWrite(S0, HIGH);
    digitalWrite(S1, LOW);
}

// Leer frecuencia del color rojo
int TCS320::readRed() {
    digitalWrite(S2, LOW);
    digitalWrite(S3, LOW);
    redFrequency = pulseIn(sensorOut, LOW);
    return redFrequency;
}

// Leer frecuencia del color verde
int TCS320::readGreen() {
    digitalWrite(S2, HIGH);
    digitalWrite(S3, HIGH);
    greenFrequency = pulseIn(sensorOut, LOW);
    return greenFrequency;
}

// Leer frecuencia del color azul
int TCS320::readBlue() {
    digitalWrite(S2, LOW);
    digitalWrite(S3, HIGH);
    blueFrequency = pulseIn(sensorOut, LOW);
    return blueFrequency;
}

// Leer todos los colores
void TCS320::readAll(int &red, int &green, int &blue) {
    red = readRed();
    green = readGreen();
    blue = readBlue();
}
