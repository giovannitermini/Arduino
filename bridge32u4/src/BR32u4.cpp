#include "BR32u4_Config.h"
#include "BR32u4.h"
#include "HardwareSerial.h"

uint8_t BRMessage::next_reqid =0;

/*
BR32u4::BR32u4()
{
    Serial.println("Start BR32u4");
}
*/

BR32u4::BR32u4(uint8_t next_reqid)
{
//    Serial.println("Start BR32u4(uint8_t)");
    printf_P(PSTR("Start BR32u4(%2x)"),next_reqid);
}

int x = 0;
int lux = 0;

void BR32u4::begin(void)
{
  pinMode(13,OUTPUT);
  Serial1.begin(57600);
}

void BRMessage::setValue(uint8_t _valLen, uint8_t _uValType, const void* _uValue)
{
    valLen=_valLen;
    uValType=_uValType;

}

void BR32u4::setCB(MsgCallbackType _msgInCB)
{
    msgInCB=_msgInCB;
}


/******************************************************************/

const char* BRMessage::toString(void) const
{
  static char buffer[45];
  char *cur = buffer, * const end = buffer + sizeof buffer;;
  cur += snprintf_P(buffer,sizeof(buffer),PSTR("len:%02x req:%02x dev:%02x cmd:%02x reg:%02x vty:%02x"),valLen,uReqID,uDevID,uCmd,uReg,uValType);
/*
 * Define code for value toString
  if (cur < end) {
    cur += snprintf(cur, end-cur, "%s", uValue);
    }
    */
  return buffer;
}


void BR32u4::update()
{
    comHandler () ;
}

//(BRMessage brMsg();
byte sync,valLen,uReqID,uDevID,uCmd,uReg,uValType,chkSum;
byte valCou=0;
byte valBuf[23];
byte parseStatus=0;
byte d;

void BR32u4::comHandler () {
  while (Serial1.available() > 0)
{
	digitalWrite(13,HIGH);
        d=Serial1.read();
        Serial.print(d,HEX);
        if (parseStatus >0 & parseStatus <8) { chkSum += d; }
        switch (parseStatus) {
	case 0: sync=d; if (sync==0x55) {parseStatus=1; chkSum=0; uValType=0; memset(valBuf,0,sizeof(valBuf)); brMsg=BRMessage();} break;
	case 1: brMsg.valLen=d; parseStatus=2; break;
	case 2: brMsg.uReqID=d; parseStatus=3; break;
	case 3: brMsg.uDevID=d; parseStatus=4; break;
	case 4: brMsg.uCmd=d; parseStatus=5; break;
	case 5: brMsg.uReg=d; if (valLen>0) { parseStatus=6; } else { parseStatus=8; } break;
	case 6: brMsg.uValType=d; parseStatus=7; break;
	case 7: valBuf[valCou++]=d; if (valCou>=valLen) { valCou=0; parseStatus=8; } break;
	case 8: if(chkSum==d) { parseStatus=9; } else { parseStatus=0; errorMsg(); } break;
	case 9: byte stop=d; if (stop==0x00) { parseStatus=0; processMsg();} break;
	}
        digitalWrite(13,LOW);
}
  /*while (Serial1.available() > 0) {
    digitalWrite(13,HIGH);

    msgBuf[msgPointer] = Serial1.read();
    Serial.print(msgBuf[msgPointer]);
    if (msgBuf[msgPointer]==0x55) {msgPointer=0; parseStatus=1;}
    if (parseStatus==1) {
    if (msgBuf[msgPointer]==0x00) {msgPointer=0;}
    Serial1.print(c);
    digitalWrite(13,LOW);
  }
  */
}


void BR32u4::processMsg()
{
  msgInCB(&brMsg);
}

void BR32u4::errorMsg()
{
  Serial.print("-");
  Serial.print(chkSum,HEX);
  Serial.println(" Checksum error");
}

