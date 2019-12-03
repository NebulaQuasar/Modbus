/****************************************************************************
!-  File Name: Modbus.c
*****************************************************************************/

//!-  Headers
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

void Unpack16bits_8bits(
        uint16_t const u16Value,
        uint8_t *const u8Byte1Ptr,
        uint8_t *const u8Byte0Ptr);

uint16_t Pack8bits_16bits(
        const uint8_t u8Byte1,
        const uint8_t u8Byte0);

Bool Validate_Function_Code(
        uint8_t const FunctionCode);

Bool Validate_SubFunction_Code(
        uint8_t const SubFunctionCode);

void CreatePacket_ReadHoldingRegister(
        Modbus_Packet *const UsrPacket);

void CreatePacket_WriteSingleRegister(
        Modbus_Packet *const UsrPacket);

/****************************************************************************
!-  LOCAL VARIABLES
*****************************************************************************/

/****************************************************************************
!-  LOCAL FUNCTIONS
*****************************************************************************/

Bool Modbus_Init(
        void) {

    return TRUE;
}

void Modbus_Main(
        void) {

}

/*
 *!-  This Function will validate if the Function Code
 *!-  Received is Valid or Not.
 */
Bool Validate_Function_Code(
        uint8_t const FunctionCode) {

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
 *!-  This Function will validate if the Sub Function Code
 *!-  Received is Valid or Not.
 */
Bool Validate_SubFunction_Code(
        uint8_t const SubFunctionCode) {

    Bool Ckeck_OK = FALSE;
    switch(SubFunctionCode) {
        case SubFun_Code00:
        case SubFun_Code01:
        case SubFun_Code02:
        case SubFun_Code03:
        case SubFun_Code04:
        case SubFun_Code10:
        case SubFun_Code11:
        case SubFun_Code12:
        case SubFun_Code13:
        case SubFun_Code14:
        case SubFun_Code15:
        case SubFun_Code16:
        case SubFun_Code17:
        case SubFun_Code18:
        case SubFun_Code20:
            Ckeck_OK = TRUE;
            break;

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
Bool Validate_Starting_Address(uint16_t const StartAddress) {
    Bool Ckeck_OK = FALSE;

    //!-  Validation check only for Holding Register.
    if ((StartAddress >= MINSTARTADDRESS) &&
        (StartAddress <= MAXSTARTADDRESS)) {
        Ckeck_OK = TRUE;
    }
    return Ckeck_OK;
}

/*
 *!-  This Function will validate if the Starting Address
 *!-  Received is Valid or Not.
 */
Bool Validate_Registers_Quantity(uint16_t const RegisterNo) {
    Bool Ckeck_OK = FALSE;

    //!- Validation check only for Holding Register.
    if ((RegisterNo >= MINREGISTERQUANTITY) &&
        (RegisterNo <= MAXREGISTERQUANTITY)) {
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
 *!-  unsigned char *MsgBuffer:
 *!-  Message to Calculate CRC Upon.
 *!-  unsigned uint16_t DataLength:
 *!-  Quantity of Bytes in Message.
 */
uint16_t Calculate_CRC16(
        uint8_t *MsgBuffer,
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
void Unpack16bits_8bits(
        const uint16_t u16Value,
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
                //!-  Byte Counts are Not Send as Response in this case.
                Counts = 0;
                break;

            default:
                break;
        }
    }
    return Counts;
}

/*
 *!-  API for Creating the Request Packet for
 *!-  Reading Holding Register.
 */

void CreatePacket_ReadHoldingRegister(
        Modbus_Packet *const UsrPacket) {

    uint8_t Index = 0;
    uint8_t DataLength = 6;
    uint16_t CalculatedCRC = 0;

    ReqPacket[Index+0] = UsrPacket->Device_ID;
    ReqPacket[Index+1] = UsrPacket->Function_Code;
    ReqPacket[Index+2] = UsrPacket->Address_Hi;
    ReqPacket[Index+3] = UsrPacket->Address_Low;
    ReqPacket[Index+4] = UsrPacket->Registers_Quantity_Hi;
    ReqPacket[Index+5] = UsrPacket->Registers_Quantity_Low;

    //!-  Calculate the CRC and Add it to Frame.
    CalculatedCRC = Calculate_CRC16(ReqPacket, DataLength);
    Unpack16bits_8bits(CalculatedCRC,&(UsrPacket->CRC_Hi),
        &(UsrPacket->CRC_Low));

    ReqPacket[Index+6] = UsrPacket->CRC_Hi;
    ReqPacket[Index+7] = UsrPacket->CRC_Low;
}

/*
 *!-  API for Creating the Request Packet for
 *!-  Writing Single Register.
 */

void CreatePacket_WriteSingleRegister(
        Modbus_Packet *const UsrPacket) {

    uint8_t Data[2] = {0};
    uint16_t UsrData = 0;
    uint8_t Index = 0;
    uint8_t DataLength = 8;
    uint16_t CalculatedCRC = 0;

    ReqPacket[Index+0] = UsrPacket->Device_ID;
    ReqPacket[Index+1] = UsrPacket->Function_Code;
    ReqPacket[Index+2] = UsrPacket->Address_Hi;
    ReqPacket[Index+3] = UsrPacket->Address_Low;
    ReqPacket[Index+4] = UsrPacket->Registers_Quantity_Hi;
    ReqPacket[Index+5] = UsrPacket->Registers_Quantity_Low;

    //!-  Set High & Low Bit of Data into Frame.
    UsrData = UsrPacket->Holding_Register->HRegister;
    Unpack16bits_8bits(UsrData, &Data[1], &Data[0]);
    ReqPacket[Index+6] = Data[1];
    ReqPacket[Index+7] = Data[0];

    //!-  Calculate the CRC and Add it to Frame.
    CalculatedCRC = Calculate_CRC16(ReqPacket, DataLength);
    Unpack16bits_8bits(CalculatedCRC,&(UsrPacket->CRC_Hi),
        &(UsrPacket->CRC_Low));

    ReqPacket[Index+8] = UsrPacket->CRC_Hi;
    ReqPacket[Index+9] = UsrPacket->CRC_Low;
}


/****************************************************************************
!-  GLOBAL FUNCTIONS
*****************************************************************************/
/*
 *!-  API to Clear the Entire Packet.
 */

void Clear_Frame(
        Modbus_Packet *const PacketPtr) {

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
 *!-  API to Set the Valid Device ID.
 */

Bool Set_Device_ID(
        Modbus_Packet *const UsrPacket,
        uint8_t const UsrDevID) {

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

Bool Set_Function_Code(
        Modbus_Packet *const UsrPacket,
        uint8_t const UsrFunctCode) {

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

Bool Set_SubFunction_Code(
        Modbus_Packet *const UsrPacket,
        uint8_t const UsrSubFunctCode) {

    Bool Is_Valid = FALSE;
    Bool Check_OK = FALSE;

    uint8_t FCode = 0;

    FCode = UsrPacket->Function_Code;

    if ((FCode != Fun_Code08) || (FCode != Fun_Code43)) {
        Is_Valid = FALSE;
    }
    else {
        Check_OK = Validate_SubFunction_Code(UsrSubFunctCode);
        if (Check_OK != FALSE) {
            UsrPacket->Subfunction_Code = UsrSubFunctCode;
            Is_Valid = TRUE;
        }
        else {
            Is_Valid = FALSE;
        }
    }
    return Is_Valid;
}

/*
 *!-  API to Set the Valid Starting Address.
 */

Bool Set_StartAddress(
        Modbus_Packet *const UsrPacket,
        uint16_t const UsrStartAddress) {

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

Bool Set_Register_Quantity(
        Modbus_Packet const *UsrPacket,
        uint16_t const UsrRegQuantity) {

    Bool Is_Valid = FALSE;
    uint8_t RegisterNo[2] ={0};

    Is_Valid = Validate_Registers_Quantity(UsrRegQuantity);

    if (Is_Valid == TRUE) {
        //!-  Unpack 16Bit Address into 8Bit-MSB & 8Bit-LSB.
        Unpack16bits_8bits(UsrRegQuantity, &RegisterNo[1], &RegisterNo[0]);
        //!-  Fill MSB in Registers_Quantity_Hi.
        UsrPacket->Registers_Quantity_Hi = RegisterNo[1];
        //!-  Fill LSB in Registers_Quantity_Low.
        UsrPacket->Registers_Quantity_Low = RegisterNo[0];
    }
    return Is_Valid;
}



Bool Set_WriteSingleRegister(
        Modbus_Packet *const UsrPacket,
        uint16_t const UsrData) {

    Bool Is_Valid = FALSE;
    uint8_t FCode = 0;

    FCode = UsrPacket->Function_Code;

    if (FCode == Fun_Code06) {
        UsrPacket->Holding_Register->HRegister = UsrData;
        Is_Valid = TRUE;
    }
    else {
        Is_Valid = FALSE;
    }
    return Is_Valid;
}

/*
 *!-  API for Creating Request Packet that is to
 *!-  be send to Slave Device.
 */

uint8_t * Modbus_Request(
        Modbus_Packet *const UsrPacket) {

    uint8_t Frame[256] = {0};
    uint8_t Index = 0;
    uint16_t DataLength = 0;
    uint16_t CalculatedCRC = 0;
    uint8_t CRC[2] = {0};

    uint8_t FCode = 0;

    FCode = UsrPacket->Function_Code;

    switch (FCode) {
        case Fun_Code03:
            CreatePacket_ReadHoldingRegister(UsrPacket);
            break;
        case Fun_Code06:
            CreatePacket_WriteSingleRegister(UsrPacket);
            break;
        default:
            break;
    }
    return ReqPacket;
}
