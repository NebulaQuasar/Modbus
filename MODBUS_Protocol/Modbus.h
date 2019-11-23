/****************************************************************************
!-  File Name: Modbus.h
*****************************************************************************/

#ifndef MODBUS_H
#define MODBUS_H

#include <stdint.h>
#include <CRC.h>

/****************************************************************************
!-  GLOBAL DEFINITIONS
*****************************************************************************/

#define TRUE 1
#define FALSE 0
#define ON 1
#define OFF 0
#define MASTER 0

typedef unsigned char Bool;

#define MODBUS_MAX_ADU_LENGTH 256
#define MODBUS_MAX_PDU_LENGTH 253

#define MinStartingAddress 0x0000
#define MaxStartingAddress 0xFFFF
#define MinRegisterQuantity 0x0001
#define MaxRegisterQuantity 0x007D

#define MSB1 0x80
#define NULL __null

//!-  The DataTypes defined in the MODBUS Protocol.

typedef uint8_t Coils;
typedef uint8_t Discrete_Input;
typedef uint16_t Input_Registers;
typedef uint16_t Holding_Registers;

typedef enum {
    Baudrate4800 = 0,
    Baudrate9600,
    Baudrate14400,
    Baudrate19200,
    Baudrate38400,
    Baudrate57600,
    Baudrate115200,
    Baudrate128000,
    Baudrate256000,
    BaudrateNone,
} BaudRate;

typedef enum {
    NoneParity = 0,
    ODDParity,
    EvenParity,
} Parity;

typedef struct {
    BaudRate  baud;
    Parity    parity;
} RTU_Config;

/*
 *!-  The Below "Modbus_Data" Structure has all
 *!-  the DataTypes Supported by MODBUS.
 *!-  The Values in Member can be set as per
 *!-  the Device Requirements.
 */

typedef struct {
    Coils             Coil;
    Discrete_Input    DInput;
    Input_Registers   IRegister[10];
    Holding_Registers HRegister;
} Modbus_Data;

/*
 *!-  Device_ID:
 *!-  Valid slave device addresses are in
 *!-  the range of 0 – 247 decimal. Address 0 is used for
 *!-  the broadcast address, which all slave devices Recognise.
 *!-  Address:
 *!-  The Address of the First Register (2 bytes).
 *!-  Registers_Quantity:
 *!-  The Number of Registers to Read (2 bytes).
 *!-  Byte_Count:
 *!-  The Number of Bytes of DATA in the response (1 byte).
 *!-  Holding_Register:
 *!-  A Sequence of Bytes that contains the values of Holding Registers
 *!-  (2 bytes per register).
 *!-
 */

typedef struct {
    uint8_t     Device_ID;
    uint8_t     Function_Code;
    uint8_t     Subfunction_Code;
    uint8_t     Address_Hi;
    uint8_t     Address_Low;
    uint8_t     Registers_Quantity_Hi;
    uint8_t     Registers_Quantity_Low;
    uint8_t     Byte_Count;
    Modbus_Data Holding_Register;
    uint8_t     CRC_Hi;
    uint8_t     CRC_Low;
} Modbus_Packet;

typedef enum {
    Fun_Code00  =   00,      //!- Invalid Function Code

    //!- Class 0 Codes
    Fun_Code03  =   03,      //!- Read Multiple/Holding Registers
    Fun_Code16  =   16,      //!- Write Multiple Registers

    //!- Class 1 Codes
    Fun_Code01  =   01,      //!- Read Coils Status
    Fun_Code02  =   02,      //!- Read Discrete Inputs Status
    Fun_Code04  =   04,      //!- Read Input Registers
    Fun_Code05  =   05,      //!- Write Single Coil
    Fun_Code06  =   06,      //!- Write Single Register
    Fun_Code07  =   07,      //!- Read Exception Status (serial-only)

    //!- Class 2 Codes
    Fun_Code24  =   24,      //!- Read FIFO
    Fun_Code23  =   23,      //!- Read/Write Multiple Registers
    Fun_Code22  =   22,      //!- Mask Write Register
    Fun_Code21  =   21,      //!- Write File Record
    Fun_Code20  =   20,      //!- Read File Record
    Fun_Code15  =   15       //!- Write Multiple Coils
} Funct_Code;

typedef enum {
    ILLEGAL_FUNCTION                =   0x01,
    ILLEGAL_DATA_ADDRESS            =   0x02,
    ILLEGAL_DATA_VALUE              =   0x03,
    SLAVE_DEVICE_FAILURE            =   0x04,
    ACKNOWLEDGE                     =   0x05,
    SLAVE_DEVICE_BUSY               =   0x06,
    MEMORY_PARITY_ERROR             =   0x08,
    GATEWAY_PATH_UNAVAILABLE        =   0x0A,
    TARGET_DEVICE_FAILED_TO_RESPOND =   0x0B
} Modbus_Exception_Code;

typedef struct Configuration {
/*
 *!-  Configuration required to Establish Connection.
 */
} RTU_Config;

/****************************************************************************
!-  GLOBAL FUNCTIONS
*****************************************************************************/

Bool Modbus_Init(
        void);

void Modbus_Main(
        void);

Bool Setup_RTU_Connection(
        RTU_Config *const ConfigPtr);

Bool Validate_Function_Code(
        const uint8_t FunctionCode);

uint8_t Get_Exception_FunctCode(
        const uint8_t FunctionCode);

void Unpack16bits_8bits(
        const uint16_t u16Value,
        uint8_t *const u8Byte1Ptr,
        uint8_t *const u8Byte0Ptr);

uint16_t Pack8bits_16bits(
        const uint8_t u8Byte1,
        const uint8_t u8Byte0);

Bool Set_Device_ID(
        Modbus_Packet *UsrPacket,
        uint8_t UsrDevID);

Bool Set_Function_Code(
        Modbus_Packet *UsrPacket,
        uint8_t UsrFunctCode);

Bool Set_StartAddress(
        Modbus_Packet *UsrPacket,
        uint16_t UsrStartAddress);

Bool Set_SubFunction_Code(
        Modbus_Packet *UsrPacket,
        uint8_t UsrSubFunctCode);

Bool Set_Register_Quantity(
        Modbus_Packet *UsrPacket,
        uint16_t UsrRegQuant);

void Clear_Frame(
        Modbus_Packet *const PacketPtr);

uint8_t Get_Byte_Counts(
        const uint8_t ReqFunctionCode,
        const uint16_t ReqRegistersQuantity);

void Modbus_Request(
        Modbus_Packet *const REQPacket);

void Modbus_Response(
        void);

void Setup_RTU_Connection(/*parameters*/);

#endif  /* MODBUS_H */






