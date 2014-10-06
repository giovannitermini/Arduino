#ifndef BR32U4_H
#define BR32U4_H

#include "BR32u4_Config.h"

typedef enum { BR32U4_UCMD_READ_REQ = 0,BR32U4_UCMD_READ_RES, BR32U4_UCMD_WRITE_REQ, BR32U4_UCMD_WRITE_RES, BR32U4_UCMD_KEEPALIVE_REQ, BR32U4_UCMD_KEEPALIVE_RES } BR32U4_msg_cmds ;
typedef enum { BR32U4_VTYPE_INT8 = 0,BR32U4_VTYPE_UINT8, BR32U4_VTYPE_INT16, BR32U4_VTYPE_UINT16, BR32U4_VTYPE_INT32, BR32U4_VTYPE_UINT32, BR32U4_VTYPE_FLOAT, BR32U4_VTYPE_FLOAT16, BR32U4_VTYPE_FLOAT32, BR32U4_VTYPE_STRING } BR32U4_msg_vtypes ;


//typedef int (*MsgCallbackType)(float);



/**
 * Header which is sent with each message
 *
 * The frame put over the air consists of this header and a message
 */
struct BRMessage
{
  uint8_t valLen; // Mandatory
  uint8_t uReqID; // Mandatory
  uint8_t uDevID; // Mandatory destination device ID
  uint8_t uCmd; // Mandatory
  uint8_t uReg; // Mandatory
  uint8_t uValType; // Mandatory
  uint8_t uValue[18]; // Optional

  static uint8_t next_reqid;

  /**
   * Default constructor
   *
   * Simply constructs a blank header
   */
  BRMessage() {}

  /**
   * Send constructor
   *
   * Use this constructor to create a header and then send a message
   *
   * @code
   *  BRMessage header(recipient_deviceId,command ID,'t');
   *  network.write(header,&message,sizeof(message));
   * @endcode
   *
   * @param _to The logical node address where the message is going
   * @param _type The type of message which follows.  Only 0-127 are allowed for
   * user messages.
   */
  BRMessage(uint8_t _uDevID, uint8_t _uCmd, uint8_t _uReg, uint8_t _valLen, uint8_t _uValType, uint8_t* uValue): uDevID(_uDevID), uReqID(next_reqid++), uCmd(_uCmd), uReg(_uReg){}
  void setValue(uint8_t _valLen, uint8_t _uValType, const void* _uValue);
  const char* toString(void) const;
  void setValType(BR32U4_msg_vtypes);
  void setCmd(BR32U4_msg_cmds);
};

extern "C"
{
  typedef void (*MsgCallbackType) (const void*);
};

//typedef int (*MsgCallbackType)(const void*);


class BR32u4
{
    public:
//        BR32u4();
        BR32u4(uint8_t);
        static void begin(void);
        void setCB(MsgCallbackType  _msgInCB);
        void update();
    protected:
        void comHandler ();
        void errorMsg();
        void processMsg();
    private:
        MsgCallbackType msgInCB;
        BRMessage brMsg;

};

#endif // BR32U4_H
