/****************************************************************************/
/*!-  File Name: Modbus.c
 ****************************************************************************/

#include <Modbus.h>
/*
 *!-  The function code field of a MODBUS data unit is coded in one byte.
 *!-  Valid codes are in the range of 1 ... 255 decimal (the range 128 – 255 is
 *!-  reserved and used for exception responses).
 *!-  When a message is sent from a Client to a Server device the function code
 *!-  field tells the server what kind of action to perform
 *!-  Function code "0" is not valid.
 *!-  Sub-function codes are added to some function codes to define
 *!-  multiple actions.
 */

/*****************************************************************************
** LOCAL DEFINITIONS
*****************************************************************************/

/*****************************************************************************
** LOCAL VARIABLES
*****************************************************************************/

static uint8_t        DEVICE_ID;
static uint8_t        FUNCTION_CODE;
static uint8_t        SUBFUNCTION_CODE;
static uint16_t       ADDRESS;
static uint16_t       REGISTERS_QUANTITY;
static uint8_t        BYTE_COUNT;

/*****************************************************************************
**  LOCAL FUNCTIONS
*****************************************************************************/

Bool Modbus_Init(void){

    return TRUE;
}

void Modbus_Main(void){


}
/*
 *!-  This Function will validate if the Function Code
 *!-  Received is Valid or Not.
 */
Bool Validate_Function_Code(const uint8_t FunctionCode){
    Bool Ckeck_OK   = FALSE;
    switch (FunctionCode){
        case Fun_Code03:
        case Fun_Code16:
        case Fun_Code01:
        case Fun_Code02:
        case Fun_Code04:
        case Fun_Code05:
        case Fun_Code06:
        case Fun_Code07:
        case Fun_Code24:
        case Fun_Code23:
        case Fun_Code22:
        case Fun_Code21:
        case Fun_Code20:
        case Fun_Code15:
            Ckeck_OK = TRUE;
            break;
        case Fun_Code00:
        default:
            Ckeck_OK = FALSE;
            break;
    }
    return Ckeck_OK;
}

/*
 *!-  This Function will generate Exceptional Function Code
 *!-  By Adding 1 in MSB of Function Code.
 */
uint8_t Get_Exception_FunctCode(const uint8_t FunctionCode){

    uint8_t exception = 0;

    exception = ((FunctionCode) || MSB1);
    return exception;
}

/*
 *!-  The configuration like BaudRate, Parity
 *!-  and COM Port required has to set here.
 */
Bool Setup_RTU_Connection(RTU_Config *const ConfigPtr){


 return TRUE;
}
/*
 *!-  unsigned char *MsgBuffer:
 *!-  Message to Calculate CRC Upon.
 *!-  unsigned uint16_t DataLength:
 *!-  Quantity of Bytes in Message.
 */
uint16_t Calculate_CRC16(MsgBuffer, DataLength)
{
    //!-  High Byte of CRC initialized
    unsigned char CRCHi = 0xFF;
    //!-  Low Byte of CRC initialized
    unsigned char CRCLo = 0xFF;
    //!-  Will index into CRC lookup table
    unsigned      uIndex;

    //!-  Pass through message buffer
    while (DataLength --)
    {
        //!-  Calculate the CRC
        uIndex   = (CRCHi ^ *MsgBuffer++);
        CRCHi = (CRCLo ^ auchCRCHi[uIndex]);
        CRCLo = auchCRCLo[uIndex];
    }
    return (CRCHi << 8 | CRCLo);
}

/*!-  Unpacks an Uint16 into two Uint8 values.
 *!-  u16Value:
 *!-  16-bit value to be unpacked into 8-bit parts.
 *!-  u8Byte1Ptr:
 *!-  Pointer to 8-bit destination for
 *!-  Most Significant Byte [MSB].
 *!-  u8Byte0Ptr:
 *!-  Pointer to 8-bit destination for
 *!-  Least Significant Byte [LSB].
 */
void Unpack16bits_8bits(
        const uint16_t u16Value,
        uint8_t *const u8Byte1Ptr,
        uint8_t *const u8Byte0Ptr){
    if ( u8Byte1Ptr != NULL )
    {
        (*u8Byte1Ptr) = (uint8_t) ((u16Value >> 8) & (uint16_t)0x00FF);
    }
    if ( u8Byte0Ptr != NULL )
    {
        (*u8Byte0Ptr) = (uint8_t) ((u16Value) & (uint16_t)0x00FF);
    }
}

/*!-  Packs two Uint8 values into one Uint16.
 *!-  u8Byte1:
 *!-  Most Significant Byte.
 *!-  u8Byte0:
 *!-  Least Significant Byte.
 */
uint16_t Pack8bits_16bits(
        const uint8_t u8Byte1,
        const uint8_t u8Byte0)
{
    uint16_t u16Value = 0;

    u16Value =
            ( ((uint16_t) u8Byte1) << 8) |
              ((uint16_t) u8Byte0);

    return u16Value;
}

Bool Set_Device_ID(uint8_t UsrDevID){

    Bool Is_Valid = FALSE;

    if ((UsrDevID > 0) && (UsrDevID < 257)){
        DEVICE_ID = UsrDevID;
        Is_Valid = TRUE;
    }
    //!-  ID 0 is Fixed for Master.
    else if (UsrDevID == MASTER) {
        DEVICE_ID = MASTER;
        Is_Valid = TRUE;
    }
    else {
        Is_Valid = FALSE;
    }
    return Is_Valid;
}

Bool Set_Function_Code(uint8_t UsrFunctCode){

    Bool Is_Valid = FALSE;

    Is_Valid = Validate_Function_Code(UsrFunctCode);

    if (Is_Valid != FALSE){
        FUNCTION_CODE = UsrFunctCode;
        Is_Valid = TRUE;
    }

    return Is_Valid;
}

Bool Set_SubFunction_Code(uint8_t UsrSubFunctCode){

    Bool Is_Valid = FALSE;

    SUBFUNCTION_CODE = UsrSubFunctCode;

    return Is_Valid;
}

void Clear_Frame(Modbus_Packet *const FramePtr){
    FramePtr->Device_ID                      =  0;
    FramePtr->Function_Code                  =  0;
    FramePtr->Subfunction_Code               =  0;
    FramePtr->Address_Hi                     =  0;
    FramePtr->Address_Low                    =  0;
    FramePtr->Registers_Quantity_Hi          =  0;
    FramePtr->Registers_Quantity_Low         =  0;
    FramePtr->Byte_Count                     =  0;
    FramePtr->Holding_Register->HRegister    =  0;
    FramePtr->CRC_Check                      =  0;
}

void Modbus_Request(void){

    //!-  Instance of MODBUS Packet.
    Modbus_Packet  REQPacket;

    //!-  Clear the Frame by Setting All Members to 0.
    Clear_Frame(&REQPacket);

    //!-  Start Construction of Frame as per User Inputs.
    REQPacket->Device_ID                      =  DEVICE_ID;
    REQPacket->Function_Code                  =  FUNCTION_CODE;
    REQPacket->Subfunction_Code               =  SUBFUNCTION_CODE;
    REQPacket->Address_Hi                     =  0;
    REQPacket->Address_Low                    =  0;
    REQPacket->Registers_Quantity_Hi          =  0;
    REQPacket->Registers_Quantity_Low         =  0;
    REQPacket->Byte_Count                     =  0;
    REQPacket->Holding_Register->HRegister    =  0;
    REQPacket->CRC_Check                      =  0;
}

void Modbus_Response(void){

    uint8_t        BytesCount;

    //!-  Instance of MODBUS Packet
    Modbus_Packet  RSPPacket;

    //!-  Clear the Frame by Setting All Members to 0.
    Clear_Frame(&RSPPacket);

    //!-  Start Construction of Frame as per User Inputs.
    RSPPacket->Device_ID                      =  0;
    RSPPacket->Function_Code                  =  0;
    RSPPacket->Subfunction_Code               =  0;
    RSPPacket->Address_Hi                     =  0;
    RSPPacket->Address_Low                    =  0;
    RSPPacket->Registers_Quantity_Hi          =  0;
    RSPPacket->Registers_Quantity_Low         =  0;
    RSPPacket->Byte_Count                     =  0;
    RSPPacket->Holding_Register->HRegister    =  0;
    RSPPacket->CRC_Check                      =  0;
}


uint8_t Get_Byte_Counts(const uint8_t ReqFunctionCode,
        const uint16_t ReqRegistersQuantity){

    uint8_t Counts = 0;
    Bool Is_Valid = FALSE;

    Is_Valid = Validate_Function_Code(ReqFunctionCode);

    if (Is_Valid != FALSE){

        switch (ReqFunctionCode){

            case Fun_Code03:
                Counts = (ReqRegistersQuantity * 2);
                break;

            case Fun_Code06:
                //!-  Byte Counts are Not Send as Response in this Case.
                Counts = 0;
                break;

            default:
                break;
        }
    }
    return Counts;
}





































