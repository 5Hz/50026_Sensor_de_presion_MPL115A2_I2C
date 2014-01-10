

/*
5Hz-electronica
Ejemplo del uso del sensor de presión MPL11A2
usando la libreria MPL11A2
*/
#include <Wire.h>
#include <MPL115A2.h>

void setup()
{
    MPL115A2.begin();
    Serial.begin(9600);
}

void loop()
{
    Serial.print("Pressure = ");
    Serial.println(MPL115A2.pressure());
    MPL115A2.shutdown();
    delay(1000);
}

