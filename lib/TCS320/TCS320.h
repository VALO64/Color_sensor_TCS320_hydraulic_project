#ifndef TCS320_H
#define TCS320_H

#include <Arduino.h>

class TCS320 {
private:
    int S0, S1, S2, S3, sensorOut;
    int redFrequency, greenFrequency, blueFrequency;

public:
    // Constructor
    TCS320(int s0, int s1, int s2, int s3, int out);

    // Métodos
    void begin();
    int readRed();
    int readGreen();
    int readBlue();
    void readAll(int &red, int &green, int &blue);
};

#endif
