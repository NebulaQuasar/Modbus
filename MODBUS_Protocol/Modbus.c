/****************************************************************************
!-  File Name: Modbus.c
*****************************************************************************/

#include <Modbus.h>
/*
 *!-  The function code field of a MODBUS data unit is coded in one byte.
 *!-  Valid codes are in the range of 1..255 decimal (the range 128 – 255
 *!-  is reserved and used for exception responses).
 *!-  When a message is sent from a Client to a Server
 *!-  device the function code field tells the
 *!-  server what kind of action to perform.
 *!-  Function code "0" is not valid.
 *!-  Sub-function codes are added to some function codes
 *!-  to define multiple actions.
 *!-
 *!-  Each request from a master can perform a single
 *!-  operation on a single bank of data.
 *!-  That is, a single request can read from the
 *!-  coils bank or write to the coils bank, but not both.
 *!-  There are exceptions, however. For example, function code 23 allows
 *!-  a master to write holding registers and read holding registers in a
 *!-  single request/response cycle.
 *!-  However, this is not a commonly implemented function code.
 *!-  Both master and slave device documentation should be checked to
 *!-  verify whether this function code is available.
 */

/****************************************************************************
!-  LOCAL DEFINITIONS
*****************************************************************************/

/****************************************************************************
!-  LOCAL VARIABLES
*****************************************************************************/

//!-  Instance of MODBUS Packet for Request.
static Modbus_Packet REQPacket;

//!-  Instance of MODBUS Packet for Response.
static Modbus_Packet RSPPacket;


/*****************************************************************************
**  LOCAL FUNCTIONS
*****************************************************************************/

Bool Modbus_Init(void) {
    return TRUE;
}

void Modbus_Main(void) {

}

/*
 *!-  This Function will validate if the Function Code
 *!-  Received is Valid or Not.
 */
Bool Validate_Function_Code(const uint8_t FunctionCode) {
    Bool Ckeck_OK = FALSE;
    switch(FunctionCode) {

        case Fun_Code01:
        case Fun_Code02:
        case Fun_Code03:
        case Fun_Code04:
        case Fun_Code05:
        case Fun_Code06:
        case Fun_Code07:
        case Fun_Code08:
        case Fun_Code11:
        case Fun_Code12:
        case Fun_Code15:
        case Fun_Code16:
        case Fun_Code17:
        case Fun_Code20:
        case Fun_Code21:
        case Fun_Code22:
        case Fun_Code23:
        case Fun_Code24:
        case Fun_Code43:
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
 *!-  This Function will validate if the Starting Address
 *!-  Received is Valid or Not.
 */
Bool Validate_Starting_Address(const uint16_t StartAddress) {
    Bool Ckeck_OK = FALSE;

    //!-  Validation check only for Holding Register.
    if ((StartAddress > MinStartingAddress) &&
        (StartAddress < MaxStartingAddress)) {
        Ckeck_OK = TRUE;
    }
    return Ckeck_OK;
}

/*
 *!-  This Function will validate if the Starting Address
 *!-  Received is Valid or Not.
 */
Bool Validate_Registers_Quantity(const uint16_t RegisterNo) {
    Bool Ckeck_OK = FALSE;

    //!- Validation check only for Holding Register.
    if ((RegisterNo >= MinRegisterQuantity) &&
        (RegisterNo <= MaxRegisterQuantity)) {
        Ckeck_OK = TRUE;
    }
    return Ckeck_OK;
}

/*
 *!-  This Function will generate Exceptional Function Code
 *!-  By Adding 1 in MSB of Function Code.
 */
uint8_t Get_Exception_FunctCode(const uint8_t FunctionCode) {

    uint8_t exception = 0;

    exception = ((FunctionCode) || MSB1);
    return exception;
}

/*
 *!-  The configuration like BaudRate, Parity
 *!-  and COM Port required has to set here.
 */
Bool Setup_RTU_Connection(RTU_Configuration *const ConfigPtr) {

    Bool Ckeck_OK = FALSE;

    Ckeck_OK = UART_Connect(&ConfigPtr);

    return Ckeck_OK;
}

/*
 *!-  unsigned char *MsgBuffer:
 *!-  Message to Calculate CRC Upon.
 *!-  unsigned uint16_t DataLength:
 *!-  Quantity of Bytes in Message.
 */
uint16_t Calculate_CRC16(uint8_t *MsgBuffer,
    uint16_t DataLength) {
    //!-  High Byte of CRC initialized
    unsigned char CRCHi = 0xFF;
    //!-  Low Byte of CRC initialized
    unsigned char CRCLo = 0xFF;
    //!-  Will index into CRC lookup table
    unsigned  uIndex;

    //!-  Pass through message buffer
    while(DataLength --) {
        //!-  Calculate the CRC
        uIndex = (CRCHi ^ *MsgBuffer++);
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
void Unpack16bits_8bits(const uint16_t u16Value,
        uint8_t *const u8Byte1Ptr,
        uint8_t *const u8Byte0Ptr) {
    if ( u8Byte1Ptr != NULL ) {
        (*u8Byte1Ptr) = (uint8_t) ((u16Value >> 8) & (uint16_t)0x00FF);
    }
    if ( u8Byte0Ptr != NULL ) {
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
        const uint8_t u8Byte0) {
    uint16_t u16Value = 0;

    u16Value = ( ((uint16_t) u8Byte1) << 8) |
               ((uint16_t) u8Byte0);

    return u16Value;
}

/*
 *!-  API to Set the Valid Device ID.
 */

Bool Set_Device_ID(Modbus_Packet *UsrPacket,
    uint8_t UsrDevID) {

    Bool Is_Valid = FALSE;

    //!-  The ID 0 is reserved as the Broadcast ID/Address.
    //!-  ID/Address are Reserved From 248 to 255
    if ((UsrDevID > 0) && (UsrDevID < 247)) {
        UsrPacket->Device_ID = UsrDevID;
        Is_Valid = TRUE;
    }
    //!-  ID 0 is Kept Fixed for Master.
    else if (UsrDevID == MASTER) {
        UsrPacket->Device_ID = MASTER;
        Is_Valid = TRUE;
    }
    else {
        Is_Valid = FALSE;
    }
    return Is_Valid;
}

/*
 *!-  API to Set the Valid Function Code.
 */

Bool Set_Function_Code(Modbus_Packet *UsrPacket,
    uint8_t UsrFunctCode) {

    Bool Is_Valid = FALSE;

    Is_Valid = Validate_Function_Code(UsrFunctCode);

    if (Is_Valid != FALSE) {
        UsrPacket->Function_Code = UsrFunctCode;
        Is_Valid = TRUE;
    }
    return Is_Valid;
}

/*
 *!-  API to Set the Valid Sub-Function Code.
 */

Bool Set_SubFunction_Code(Modbus_Packet *UsrPacket,
    uint8_t UsrSubFunctCode) {
    Bool Is_Valid = FALSE;
    UsrPacket->Subfunction_Code = UsrSubFunctCode;
    return Is_Valid;
}

/*
 *!-  API to Set the Valid Starting Address.
 */

Bool Set_StartAddress(Modbus_Packet *UsrPacket,
    uint16_t UsrStartAddress) {

    Bool Is_Valid = FALSE;
    uint8_t Address[2] ={0};

    Is_Valid = Validate_Starting_Address(UsrStartAddress);

    if (Is_Valid == TRUE) {
        //!-  Unpack 16Bit Address into 8Bit-MSB & 8Bit-LSB.
        Unpack16bits_8bits(UsrStartAddress, &Address[1], &Address[0]);
        //!-  Fill MSB in Address_Hi.
        UsrPacket->Address_Hi = Address[1];
        //!-  Fill LSB in Address_Low.
        UsrPacket->Address_Low = Address[0];
    }
    return Is_Valid;
}

/*
 *!-  API to Set the Valid Starting Address.
 */

Bool Set_Register_Quantity(Modbus_Packet *UsrPacket,
    uint16_t UsrRegQuant) {

    Bool Is_Valid = FALSE;
    uint8_t RegisterNo[2] ={0};

    Is_Valid = Validate_Registers_Quantity(UsrRegQuant);

    if (Is_Valid == TRUE) {
        //!-  Unpack 16Bit Address into 8Bit-MSB & 8Bit-LSB.
        Unpack16bits_8bits(UsrRegQuant, &RegisterNo[1], &RegisterNo[0]);
        //!-  Fill MSB in Registers_Quantity_Hi.
        UsrPacket->Registers_Quantity_Hi = RegisterNo[1];
        //!-  Fill LSB in Registers_Quantity_Low.
        UsrPacket->Registers_Quantity_Low = RegisterNo[0];
    }
    return Is_Valid;
}

/*
 *!-  API to Clear the Entire Packet.
 */

void Clear_Frame(Modbus_Packet *const PacketPtr) {
    PacketPtr->Device_ID = 0;
    PacketPtr->Function_Code = 0;
    PacketPtr->Subfunction_Code = 0;
    PacketPtr->Address_Hi = 0;
    PacketPtr->Address_Low = 0;
    PacketPtr->Registers_Quantity_Hi = 0;
    PacketPtr->Registers_Quantity_Low = 0;
    PacketPtr->Byte_Count = 0;
    PacketPtr->Holding_Register->HRegister = 0;
    PacketPtr->CRC_Hi = 0;
    PacketPtr->CRC_Low = 0;
}

/*
 *!-  API for Sending Request to Slave.
 */

void Modbus_Request(Modbus_Packet *const REQPacket) {

    uint8_t Frame[256] = {0};
    unsigned int Index = 0;
    uint16_t DataLength = 0;
    uint16_t CalculatedCRC = 0;
    uint8_t CRC[2] = {0};

    //!-  Fill Frame[0] with Device ID.
    Frame[Index] = REQPacket->Device_ID;
    Index++;

    //!-  Fill Frame[1] with Function Code.
    Frame[Index] = REQPacket->Function_Code;
    Index++;

    //!-  Fill Frame[2] with Sub-Function Code (If any Present).
    if (REQPacket->Subfunction_Code != 0) {
        Frame[Index] = REQPacket->Subfunction_Code;
        Index++;
    }

    //!-  Fill Frame[3] with MSB of Address.
    Frame[Index] = REQPacket->Address_Hi;
    Index++;
    //!-  Fill Frame[4] with LSB of Address.
    Frame[Index] = REQPacket->Address_Low;
    Index++;

    //!-  Fill Frame[5] with Sub-Function Code (If any Present).
    if (REQPacket->Holding_Register->HRegister != 0) {
        Frame[Index] = REQPacket->Subfunction_Code;
        Index++;
    }

    //!-  Fill Frame[6] with MSB of Quantity.
    Frame[Index] = REQPacket->Registers_Quantity_Hi;
    Index++;
    //!-  Fill Frame[7] with LSB of Quantity.
    Frame[Index] = REQPacket->Registers_Quantity_Low;
    Index++;

    DataLength = Index;

    //!-  Calculate the CRC and Add it to Frame.
    CalculatedCRC = Calculate_CRC16(Frame, DataLength);
    Unpack16bits_8bits(CalculatedCRC,
        &(REQPacket->CRC_Hi),
        &(REQPacket->CRC_Low));

    //!-  Fill Frame[8] with MSB of CRC.
    Frame[Index] = REQPacket->CRC_Hi;
    Index++;
    //!-  Fill Frame[9] with LSB of CRC.
    Frame[Index] = REQPacket->CRC_Low;
}

void Modbus_Response(void) {

}

/*
 *!-  API for Getting the Byte Counts.
 */

uint8_t Get_Byte_Counts(const uint8_t ReqFunctionCode,
        const uint16_t ReqRegistersQuantity) {

    uint8_t Counts = 0;
    Bool Is_Valid = FALSE;

    Is_Valid = Validate_Function_Code(ReqFunctionCode);

    if (Is_Valid != FALSE){
        switch (ReqFunctionCode) {
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

