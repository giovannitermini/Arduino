#line 1
#include <Arduino.h>
/*
 Copyright (C) 2012 James Coliz, Jr. <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

/**
 * Simplest possible example of using RF24Network
 *
 * TRANSMITTER NODE
 * Every 2 seconds, send a payload to the receiver node.
 */

#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
#include <HardwareSerial.h>

// nRF24L01(+) radio attached using Getting Started board
RF24 radio(9,10);

// Network uses that radio
RF24Network network(radio);

// Address of our node
const uint16_t this_node = 5;

// Address of the other node
const uint16_t other_node = 0;

// How often to send 'hello world to the other unit
const unsigned long interval = 1000; //ms

// When did we last send?
unsigned long last_sent;

// How many have we sent already
unsigned long packets_sent;

// Structure of our payload
struct payload_t
{
  byte valLen; // Mandatory
  byte uReqID; // Mandatory
  byte uDevID; // Mandatory
  byte uCmd; // Mandatory
  byte uReg; // Mandatory
  byte uValType; // Mandatory
  byte uValue[23]; // Optional
};

void msgCB (const void *pmsg)
{
//    BRMessage msg = *(BRMessage*)pmsg;
    Serial.print("Call megCB Message=");
//    Serial.println((msg).toString());
}

void setup(void)
{
  Serial.begin(57600);
  printf_P(PSTR("RF24Network/examples/helloworld_tx/"));
//  Serial.println("RF24Network/examples/helloworld_tx/");

  Serial.println("----");
  SPI.begin();
  radio.begin();
  network.begin(/*channel*/ 90, /*node address*/ this_node);
}

void loop(void)
{
  // Pump the network regularly
  network.update();

  // If it's time to send a message, send it!
  unsigned long now = millis();
  if ( now - last_sent >= interval  )
  {
    last_sent = now;

    Serial.print("Sending...");
    payload_t payload = { 18, packets_sent++ , 0x11, 0x22, 0x33, 0x44, 0x55,0x66,0x77,0x88, 0x99,0xAA,0xBB,0xCC, 0xDD,0xEE,0xFF,0x00, 0x11,0x22,0x33,0x44, 0x55,0x66,0x77,0x88, 0x99,0xAA,0xBB };
    RF24NetworkHeader header(/*to node*/ other_node);
    byte pktSize = payload.valLen+6;
    Serial.print(pktSize,HEX);
    pktSize = (pktSize> sizeof(payload)) ? sizeof(payload) : pktSize;
    bool ok = network.write(header,&payload,pktSize);
    if (ok)
    {
      Serial.print('-');
      Serial.print(pktSize,HEX);
      Serial.print(':');
      Serial.print(payload.uReqID,HEX);
      Serial.println("ok.");
    }
    else
      Serial.println("failed.");
  }

  // Pump the network regularly
  network.update();


//  delay(2000);

  while (network.available())
  {
    RF24NetworkHeader header;
    payload_t payload;
    byte pktSize = network.read(header,&payload,sizeof(payload));
    Serial.print(header.from_node,HEX);Serial.print(':');
    Serial.print(header.id,HEX);Serial.print(':');
    Serial.print(pktSize,HEX);Serial.print(':');
    Serial.print("[");
    Serial.print(payload.valLen,HEX);Serial.print(':');
    Serial.print(payload.uReqID,HEX);Serial.print(':');
    Serial.print(payload.uDevID,HEX);Serial.print(':');
    Serial.print(payload.uCmd,HEX);Serial.print(':');
    Serial.print(payload.uReg,HEX);Serial.print(':');
    Serial.print(payload.uValType,HEX);Serial.print(':');
    for (int i=0; i < payload.valLen; i++){
    Serial.print(payload.uValue[i],HEX);Serial.print('-');
    }
    Serial.println("]");
  }
}
// vim:ai:cin:sts=2 sw=2 ft=cpp
