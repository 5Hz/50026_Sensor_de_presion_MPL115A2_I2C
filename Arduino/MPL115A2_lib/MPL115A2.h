/*
 MPL115A2 Sensor Library for Arduino
   created by R.N <zan73722@gmail.com>

  2011-01-16 Created.
*/
#ifndef __MPL115A2_H__
#define __MPL115A2_H__

#include <Wire.h>
#include <inttypes.h>

//extern "C" {
//#include <math.h>
//}

class MPL115A2Class
{
private:
    int m_shdnPin;
    int m_bShutdown;
    int m_i2c_address;
    short sic11;
    short sic12;
    short sic22;
    short sib1;
    short sib2;
    short sia0;
    long sic11l;
    long sic12l;
    long sic22l;
    long sib1l;
    long sib2l;
    long sia0l;

    float pressure_once();     /* hPa */

public:
    MPL115A2Class(const int shdnPin = 16);

    void begin();
    void shutdown();
    float pressure();     /* hPa */
};

extern MPL115A2Class MPL115A2;

#endif
	