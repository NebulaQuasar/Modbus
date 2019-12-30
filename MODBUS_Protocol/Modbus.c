/*
 * Modbus.c
 *
 *  Created on: 22-Dec-2019
 *      Author: root
 */

/****************************************************************************
!-  File Name: Modbus.c
*****************************************************************************/

//!-  Headers
#include "Modbus.h"

/****************************************************************************
!-  LOCAL DEFINITIONS
*****************************************************************************/

void Clear_AllData(
        void);

void Clear_Frame(
        Modbus_Packet *const PacketPtr);

void Unpack16bits_8bits(
        const uint16_t u16Value,
        uint8_t *const u8Byte1Ptr,
        uint8_t *const u8Byte0Ptr);

uint16_t Pack8bits_16bits(
        const uint8_t u8Byte1,
        const uint8_t u8Byte0);

Bool Set_Device_ID(
        Modbus_Packet *const UsrPacket,
        uint8_t const UsrDevID);

Bool Validate_Function_Code(
        uint8_t const FunctionCode);

Bool Validate_Starting_Address(
        uint8_t  const FunctionCode,
        uint16_t const StartAddress);

Bool Validate_Registers_Quantity(
        uint8_t  const FunctionCode,
        uint16_t const StartAddress,
        uint16_t const RegisterNo);

Bool Set_StartAddress(
        Modbus_Packet *const UsrPacket,
        uint16_t const UsrStartAddress,
        uint8_t  const FunctionCode);

Bool Set_Register_Quantity(
        Modbus_Packet const *UsrPacket,
        uint16_t const UsrRegQuantity,
        uint8_t  const FunctionCode,
        uint16_t const StartAddress);

/****************************************************************************
!-  LOCAL VARIABLES
*****************************************************************************/

/****************************************************************************
!-  LOCAL FUNCTIONS
*****************************************************************************/

//!-  Clear_AllData() to clear all Values in Database.
void Clear_AllData(
        void) {

    uint16_t Index = 0;
    uint8_t const CleanValue = 0;
    if (COILS_NUMBERS > MIN_COILS_NUMBERS) {
        for (Index = 0; Index <= COILS_ADDR; Index++) {
            MODBUS_DATABASE->Coils[Index] = CleanValue;
        }
    }
    if (DISCRETE_INPUTS_NUMBERS > MIN_DISCRETE_INPUTS_NUMBERS) {
        for (Index = 0; Index <= DISCRETE_INPUTS_ADDR; Index++) {
            MODBUS_DATABASE->DInputs[Index] = CleanValue;
        }
    }
    if (INPUT_REGISTERS_NUMBERS > MIN_INPUT_REGISTERS_NUMBERS) {
        for (Index = 0; Index <= INPUT_REGISTERS_ADDR; Index++) {
            MODBUS_DATABASE->IRegisters[Index] = CleanValue;
        }
    }
    if (HOLDING_REGISTERS_NUMBERS > MIN_HOLDING_REGISTERS_NUMBERS) {
        for (Index = 0; Index <= HOLDING_REGISTERS_ADDR; Index++) {
            MODBUS_DATABASE->HRegisters[Index] = CleanValue;
        }
    }
}

//!-  Clear_Frame() to Clear entire Packet Frame.
void Clear_Frame(
        Modbus_Packet *const PacketPtr) {

    PacketPtr->Device_ID = 0;
    PacketPtr->Function_Code = 0;
    PacketPtr->Address_Hi = 0;
    PacketPtr->Address_Low = 0;
    PacketPtr->Registers_Quantity_Hi = 0;
    PacketPtr->Registers_Quantity_Low = 0;
    PacketPtr->Byte_Count = 0;
    PacketPtr->Data->Coils = 0;
    PacketPtr->Data->DInputs = 0;
    PacketPtr->Data->IRegisters = 0;
    PacketPtr->Data->HRegisters = 0;
    PacketPtr->CRC_Hi = 0;
    PacketPtr->CRC_Low = 0;
}

/*
 *!-  Unpack16bits_8bits() to unpack an Uint16 into two Uint8 values.
 *!-  u16Value: 16-bit value to be unpacked into 8-bit parts.
 *!-  u8Byte1Ptr: Pointer to 8-bit destination for MSB.
 *!-  u8Byte0Ptr: Pointer to 8-bit destination for LSB.
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

/*
 *!-  Pack8bits_16bits() to pack two Uint8 values into one Uint16 value.
 *!-  u8Byte1: Most Significant Byte.
 *!-  u8Byte0: Least Significant Byte.
 */
uint16_t Pack8bits_16bits(
        const uint8_t u8Byte1,
        const uint8_t u8Byte0) {
    uint16_t u16Value = 0;

    u16Value = ( ((uint16_t) u8Byte1) << 8) |
               ((uint16_t) u8Byte0);

    return u16Value;
}

//!-  Set_Device_ID() to Set the Valid Device ID.
Bool Set_Device_ID(
        Modbus_Packet *const UsrPacket,
        uint8_t const UsrDevID) {

    Bool Is_Valid = FALSE;

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
 *!-  Validate_Function_Code() will validate if the
 *!-  Provided Function Code is Valid or Not.
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
        case Fun_Code15:
        case Fun_Code16:
        case Fun_Code20:
        case Fun_Code21:
        case Fun_Code22:
        case Fun_Code23:
        case Fun_Code24:
            Ckeck_OK = TRUE;
            break;

        default:
            Ckeck_OK = FALSE;
            break;
    }
    return Ckeck_OK;
}

/*
 *!-  Validate_Starting_Address() will validate if
 *!-  the Starting Address Provided is Valid or Invalid.
 */
Bool Validate_Starting_Address(
        uint8_t  const FunctionCode,
        uint16_t const StartAddress) {

    Bool Ckeck_OK = FALSE;
    switch(FunctionCode) {
        case Fun_Code01:
        case Fun_Code05:
        case Fun_Code15:
            if ((StartAddress >= MIN_COILS_ADDRESS) &&
                (StartAddress <= COILS_ADDR)) {
                Ckeck_OK = TRUE;
            }
            break;
        case Fun_Code02:
            if ((StartAddress >= MIN_DISCRETE_INPUTS_ADDRESS) &&
                (StartAddress <= DISCRETE_INPUTS_ADDR)) {
                Ckeck_OK = TRUE;
            }
            break;
        case Fun_Code03:
        case Fun_Code06:
        case Fun_Code16:
        case Fun_Code23:
            if ((StartAddress >= MIN_HOLDING_REGISTERS_ADDRESS) &&
                (StartAddress <= HOLDING_REGISTERS_ADDR)) {
                Ckeck_OK = TRUE;
            }
            break;
        case Fun_Code04:
            if ((StartAddress >= MIN_INPUT_REGISTERS_ADDRESS) &&
                (StartAddress <= INPUT_REGISTERS_ADDR)) {
                Ckeck_OK = TRUE;
            }
            break;
        default:
            Ckeck_OK = FALSE;
            break;
    }

    return Ckeck_OK;
}

/*
 *!-  Validate_Registers_Quantity() will validate if
 *!-  the Quantity of Registers Provided is Valid or Invalid.
 */
Bool Validate_Registers_Quantity(
        uint8_t  const FunctionCode,
        uint16_t const StartAddress,
        uint16_t const RegisterNo) {

    Bool Ckeck_OK = FALSE;
    switch(FunctionCode) {
        case Fun_Code01:
        case Fun_Code15:
            if ((RegisterNo >= MIN_COILS_NUMBERS) &&
                (RegisterNo <= COILS_NUMBERS)     &&
                (StartAddress + RegisterNo) <= COILS_ADDR) {
                    Ckeck_OK = TRUE;
            }
            break;
        case Fun_Code02:
            if ((RegisterNo >= MIN_DISCRETE_INPUTS_NUMBERS) &&
                (RegisterNo <= DISCRETE_INPUTS_NUMBERS)     &&
                (StartAddress + RegisterNo) <= DISCRETE_INPUTS_ADDR) {
                    Ckeck_OK = TRUE;
             }
             break;
        case Fun_Code03:
            if ((RegisterNo >= MIN_HOLDING_REGISTERS_NUMBERS) &&
                (RegisterNo <= HOLDING_REGISTERS_NUMBERS)     &&
                (StartAddress + RegisterNo) <= HOLDING_REGISTERS_ADDR) {
                    Ckeck_OK = TRUE;
            }
             break;
        case Fun_Code04:
        case Fun_Code16:
            if ((RegisterNo >= MIN_INPUT_REGISTERS_NUMBERS) &&
                (RegisterNo <= INPUT_REGISTERS_NUMBERS)     &&
                (StartAddress + RegisterNo) <= INPUT_REGISTERS_ADDR) {
                    Ckeck_OK = TRUE;
            }
             break;
        default:
            Ckeck_OK = FALSE;
            break;
    }

    return Ckeck_OK;
}

//!-  Set_Function_Code() to Set the Valid Device ID.
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

//!-  Set_StartAddress() to Set the Valid Starting Address
Bool Set_StartAddress(
        Modbus_Packet *const UsrPacket,
        uint16_t const UsrStartAddress,
        uint8_t  const FunctionCode) {

    Bool Is_Valid = FALSE;
    uint8_t Address[2] ={0};

    Is_Valid = Validate_Starting_Address(    \
                   FunctionCode, UsrStartAddress);

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

//!-  Set_Register_Quantity() to Set the Valid Starting Address
Bool Set_Register_Quantity(
        Modbus_Packet const *UsrPacket,
        uint16_t const UsrRegQuantity,
        uint8_t  const FunctionCode,
        uint16_t const StartAddress) {

    Bool Is_Valid = FALSE;
    uint8_t RegisterNo[2] ={0};

    Is_Valid = Validate_Registers_Quantity(    \
                       FunctionCode, StartAddress, UsrRegQuantity);

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

/*
 *!-  Get_Exception_FunctCode() to generate Exceptional Function Code
 *!-  By Adding 1 in MSB of Function Code.
 */
uint8_t Get_Exception_FunctCode(
        const uint8_t FunctionCode) {

    uint8_t exception = 0;

    exception = ((FunctionCode) || MSB1);
    return exception;
}

//!-  Get_Byte_Counts() for Getting the Byte Counts
uint8_t Get_Byte_Counts(
        const uint8_t ReqFunctionCode,
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

/****************************************************************************
!-  GLOBAL FUNCTIONS
*****************************************************************************/

//!-  Modbus_Init() for Initializing MODBUS Module.
Bool Modbus_Init(
        void) {

    Bool Ckeck_OK = TRUE;

    if (COILS_NUMBERS != MIN_COILS_NUMBERS) {
        if (COILS_NUMBERS > MAX_COILS_NUMBERS) {
            Ckeck_OK = FALSE;
        }
    }

    if (DISCRETE_INPUTS_NUMBERS != MIN_DISCRETE_INPUTS_NUMBERS) {
        if (DISCRETE_INPUTS_NUMBERS > MAX_DISCRETE_INPUTS_NUMBERS) {
            Ckeck_OK = FALSE;
        }
    }

    if (INPUT_REGISTERS_NUMBERS != MIN_INPUT_REGISTERS_NUMBERS) {
        if (INPUT_REGISTERS_NUMBERS > MAX_INPUT_REGISTERS_NUMBERS) {
            Ckeck_OK = FALSE;
        }
    }

    if (HOLDING_REGISTERS_NUMBERS != MIN_HOLDING_REGISTERS_NUMBERS) {
        if (HOLDING_REGISTERS_NUMBERS > MAX_HOLDING_REGISTERS_NUMBERS) {
            Ckeck_OK = FALSE;
        }
    }

    Clear_AllData();
    return Ckeck_OK;
}

/*
 *!-  Calculate_CRC16() to calculate the CRC of the Message Frame.
 *!-  unsigned char *MsgBuffer: Message to Calculate CRC Upon.
 *!-  unsigned uint16_t DataLength: Quantity of Bytes in Message.
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

//!-  Set_Single_Coil() for writing a single Coil.
Bool Set_Single_Coil(
        uint8_t const Coil_Number,
        uint8_t const Value) {

    Bool Check_Ok = FALSE;
    uint8_t Index = 0;
    Index = Coil_Number - 1;
    if (Index <= COILS_ADDR) {
        MODBUS_DATABASE->Coils[Index] = Value;
        Check_Ok = TRUE;
    }
    return Check_Ok;
}

//!-  Set_Single_Discrete_Input() for writing a single Discrete Input.
Bool Set_Single_Discrete_Input(
        uint8_t const DInput_Number,
        uint8_t const Value) {

    Bool Check_Ok = FALSE;
    uint8_t Index = 0;
    Index = DInput_Number - 1;
    if (Index <= DISCRETE_INPUTS_ADDR) {
        MODBUS_DATABASE->DInputs[Index] = Value;
        Check_Ok = TRUE;
    }
    return Check_Ok;
}

//!-  Set_Single_Input_Register() for writing a single Input Register.
Bool Set_Single_Input_Register(
        uint16_t const InputReg_Number,
        uint16_t const Value) {

    Bool Check_Ok = FALSE;
    uint16_t Index = 0;
    Index = InputReg_Number - 1;
    if (Index <= DISCRETE_INPUTS_ADDR) {
        MODBUS_DATABASE->IRegisters[Index] = Value;
        Check_Ok = TRUE;
    }
    return Check_Ok;
}

//!-  Set_Single_Holding_Register() for writing a single Holding Register.
Bool Set_Single_Holding_Register(
        uint16_t const HoldingReg_Number,
        uint16_t const Value) {

    Bool Check_Ok = FALSE;
    uint16_t Index = 0;
    Index = HoldingReg_Number - 1;
    if (Index <= DISCRETE_INPUTS_ADDR) {
        MODBUS_DATABASE->HRegisters[Index] = Value;
        Check_Ok = TRUE;
    }
    return Check_Ok;
}

//!-  Get_Single_Coil() to obtain a single Coil.
uint8_t Get_Single_Coil(
        uint8_t const Coil_Number) {

    uint8_t Value = 0;
    uint8_t Index = 0;
    if ((Coil_Number != ABSENT) &&
        (Coil_Number <= COILS_NUMBERS)) {
        Index = Coil_Number - 1;
        Value = MODBUS_DATABASE->Coils[Index];
    }
    return Value;
}

//!-  Get_Single_Discrete_Input() to obtain a single Discrete Input.
uint8_t Get_Single_Discrete_Input(
        uint8_t const DInput_Number) {

    uint8_t Value = 0;
    uint8_t Index = 0;
    if ((DInput_Number != ABSENT) &&
        (DInput_Number <= DISCRETE_INPUTS_NUMBERS)) {
        Index = DInput_Number - 1;
        Value = MODBUS_DATABASE->DInputs[Index];
    }
    return Value;
}

//!-  Get_Single_Input_Register() to obtain a single Input Register.
uint16_t Get_Single_Input_Register(
        uint16_t const InputReg_Number) {

    uint16_t Value = 0;
    uint16_t Index = 0;
    if ((InputReg_Number != ABSENT) &&
        (InputReg_Number <= INPUT_REGISTERS_NUMBERS)) {
        Index = InputReg_Number - 1;
        Value = MODBUS_DATABASE->IRegisters[Index];
    }
    return Value;
}

//!-  Get_Single_Holding_Register() to obtain a single Holding Register.
uint16_t Get_Single_Holding_Register(
        uint16_t const HoldingReg_Number) {

    uint16_t Value = 0;
    uint16_t Index = 0;
    if ((HoldingReg_Number != ABSENT) &&
        (HoldingReg_Number <= HOLDING_REGISTERS_NUMBERS)) {
        Index = HoldingReg_Number - 1;
        Value = MODBUS_DATABASE->HRegisters[Index];
    }
    return Value;
}

Bool Modbus_Request(
        uint8_t const Device_ID,
        uint8_t const ReqFunctionCode) {

    Bool Check_Ok = FALSE;

    return Check_Ok;
}

Bool Modbus_Response(
        uint8_t const Device_ID,
        uint8_t const RspFunctionCode) {

    Bool Check_Ok = FALSE;

    return Check_Ok;
}
