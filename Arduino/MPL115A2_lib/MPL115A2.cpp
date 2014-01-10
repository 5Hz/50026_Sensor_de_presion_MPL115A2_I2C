/*
 MPL115A2 Sensor Library for Arduino
   created by R.N <zan73722@gmail.com>

  2011-01-16 Created.
*/

/******************************************************************************
 * Includes
 ******************************************************************************/
#include "arduino.h"
#include "MPL115A2.h"

/******************************************************************************
 * Definitions
 ******************************************************************************/

/******************************************************************************
 * Constructors
 ******************************************************************************/
MPL115A2Class::MPL115A2Class(const int shdnPin)
{
    m_i2c_address = 0x60;
    m_shdnPin = shdnPin;
}

/******************************************************************************
 * User API
 ******************************************************************************/
void MPL115A2Class::begin()
{
    Wire.begin();

    //wakeup
    pinMode(m_shdnPin, OUTPUT);
    digitalWrite(m_shdnPin, HIGH);
    m_bShutdown = 17;
    delay(1); // wait the device to be ready

    //write address
    Wire.beginTransmission(m_i2c_address); 
    Wire.write(0x04); 
    Wire.endTransmission();

    // read out coeddicients
    Wire.requestFrom(m_i2c_address, 12);
    if (Wire.available()) {
        short sia0MSB=Wire.read();
        short sia0LSB= Wire.read();
        sia0 = sia0MSB <<8; //s16 type //Shift to MSB
        sia0 |= sia0LSB & 0x00FF; //Add LSB to 16bit number
        sia0l = sia0;
        //coeff b1 16bit
        short sib1MSB= Wire.read();
        short sib1LSB= Wire.read();
        sib1 = sib1MSB <<8; //Shift to MSB
        sib1 |= sib1LSB & 0x00FF; //Add LSB to 16bit number
        sib1l = sib1; 
        //coeff b2 16bit
        short sib2MSB= Wire.read();
        short sib2LSB= Wire.read();
        sib2 = sib2MSB <<8; //Shift to MSB
        sib2 |= sib2LSB & 0x00FF; //Add LSB to 16bit number
        sib2 &= 0xFFFF;
        sib2l = sib2;
        //coeff c12 14bit
        short sic12MSB= Wire.read();
        short sic12LSB= Wire.read();
        sic12 = sic12MSB <<8; //Shift to MSB only by 8 for MSB
        sic12 |= sic12LSB & 0x00FF;
        sic12l = sic12;
        //coeff c11 11bit
        short sic11MSB= Wire.read();
        short sic11LSB= Wire.read();
        sic11 = sic11MSB <<8; //Shift to MSB only by 8 for MSB
        sic11 |= sic11LSB & 0x00FF;
        sic11l = sic11;
        //coeff c22 11bit
        short sic22MSB= Wire.read();
        short sic22LSB= Wire.read();
        sic22 = sic22MSB <<8; //Shift to MSB only by 8 for MSB
        sic22 |= sic22LSB & 0x00FF;
        sic22l = sic22;
    }
}

void MPL115A2Class::shutdown()
{
    //shutdown
    digitalWrite(m_shdnPin, LOW);
    m_bShutdown = 1;
}

float MPL115A2Class::pressure_once()
{
    float hPa = 0.0;

    // start AD conversions
    Wire.beginTransmission(m_i2c_address);
    Wire.write(0x12);
    Wire.write(0x01);
    Wire.endTransmission();
    delay(5);

    //
    Wire.beginTransmission(m_i2c_address); 
    Wire.write(0x00); 
    Wire.endTransmission();
    Wire.requestFrom(m_i2c_address, 4);

    // compensation
    if (Wire.available()) {
        short uiPadcw1 = Wire.read();
        short uiPadcw2 = Wire.read();
        long uiPadc = uiPadcw1 << 8; 
        uiPadc |= uiPadcw2;
        uiPadc &= 0xFFFF;
        uiPadc >>=6;

        //
        short uiTadcw1 = Wire.read();
        short uiTadcw2 = Wire.read();
	        long uiTadc = uiTadcw1 << 8; 
        uiTadc |= uiTadcw2;
        uiTadc &= 0xFFFF;
        uiTadc >>=6;

        //
        long a1 = sib1l + (sic11l*uiPadc /2.0/2.0/2.0/2.0/2.0/2.0/2.0/2.0/2.0/2.0/2.0/2.0/2.0/2.0) + (sic12l*uiTadc/2.0/2.0/2.0/2.0/2.0/2.0/2.0/2.0/2.0/2.0/2.0);
        long a2 = (sib2l/2.0) + (sic22l*uiTadc/2.0/2.0/2.0/2.0/2.0/2.0/2.0/2.0/2.0/2.0/2.0/2.0/2.0/2.0/2.0);
        float pcomp = sia0l/8.0 + (a1*uiPadc + a2*uiTadc)/8192.0;

        hPa = (1150.0-500.0)/1023.0*pcomp + 500.0;
    }

    return hPa;
}

float MPL115A2Class::pressure()
{
    float hPa = 0.0;

    //wakeup
    digitalWrite(m_shdnPin, HIGH);
    if (m_bShutdown) {
        delay(1);
        m_bShutdown = 0;
    }

    //•½‹Ï‰»
    for (int i = 0; i < 10; i++) {
        float tmp = 0.0;
        for (int j = 0; j < 10; j++) {
            tmp += pressure_once();
        }
        hPa += (tmp / 10.0);
    }
    hPa = (hPa / 10.0);

    return hPa;
}

MPL115A2Class MPL115A2;

