/******************************************************************
** Código creado en Electrónica 5Hz                              **
** www.5hz-electronica.com                                       **
** Por: Baruc                                                    **
** Basado en el código de: Manuel                                **
** Descripción del código:                                       **
**                                                               **
** Ejemplo para el sensor de presion MPL115A2                    **
** Programa sencillo que lee la presion atmosférica              **
** y la imprime en el puerto serial                              **
*******************************************************************

Conexiones para la comunicacion i2c usando el breakout de 5Hz con un Arduino Uno / Duemilanove:
Arduino <-> MPL115A2
Gnd      -  GND
5v       -  VCC
A4       -  SDA
A5       -  SCL
A3/17    -  RST
*/

//Libreria para comunicacion I2C
#include <Wire.h>
//Libreria para comunicarse con el sensor (hace uso de Wire.h)
#include <MPL115A2.h>

//Pin de rest del sensor
int rstPin =17;
//Constructor de la clase del sensor MPL115A2, requiere saber donde se conecta el pin de Rest
MPL115A2Class miSensor(rstPin);

void setup()
{
    //inicializa el sensor
    miSensor.begin();
    //inicializa el puerto serial
    Serial.begin(9600);
}

void loop()
{
    Serial.print("Presion = ");
    //Lee el valor de presion del sensor
    Serial.println(miSensor.pressure());
    //Pone al sensor en estado de bajo consumo para ahorrar energia
    miSensor.shutdown();
    delay(1000);
}
