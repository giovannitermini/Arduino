#include "nrf32u4_Config.h"
#include "nrf32u4.h"


nrf32u4::nrf32u4(uint8_t ce, uint8_t csn): radio(ce,csn), network(radio)
{
//    Serial.println("RFNetwork init ce=");
    printf_P(PSTR("RFNetwork init ce=%d csn=%d\n\r"),ce,csn);
}

void nrf32u4::begin(uint8_t ce, uint8_t csn)
{
    // nRF24L01(+) radio attached using Getting Started board
//    radio(9,10);

// Network uses that radio
//    network(radio);
}

nrf32u4::~nrf32u4()
{
    //dtor
}
