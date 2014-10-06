#ifndef NRF32U4_H
#define NRF32U4_H

#include "nrf32u4_Config.h"
#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>

class nrf32u4
{
    public:
        nrf32u4(uint8_t,uint8_t);
        ~nrf32u4();
        void begin(uint8_t,uint8_t);
    protected:
    private:
        RF24 radio;
        RF24Network network;
};

#endif // NRF32U4_H
