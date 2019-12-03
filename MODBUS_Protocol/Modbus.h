/****************************************************************************
!-  File Name: Modbus.h
*****************************************************************************/

#ifndef MODBUS_H
#define MODBUS_H

//!-  Headers
#include <stdint.h>
#include <CRC.h>

/****************************************************************************
!-  GLOBAL DEFINITIONS
*****************************************************************************/
//!-  GNU Specific
//#define NULL  __null

#define NULL   ((void *)0)

#define TRUE   1
#define FALSE  0
#define ON     1
#define OFF    0

#define MASTER  0

#define MSB1  0x80

#define MODBUS_MAX_ADU_LENGTH  256
#define MODBUS_MAX_PDU_LENGTH  253

#define MINSTARTADDRESS  0x0000
#define MAXSTARTADDRESS  0xFFFF

#define MINREGISTERQUANTITY  0x0001
#define MAXREGISTERQUANTITY  0x07D0

/****************************************************************************
!-  GLOBAL TYPE DEFINITIONS
*****************************************************************************/

typedef unsigned char Bool;

//static uint8_t ReqPacket_06[11]  = {0};
//static uint8_t ReqPacket_03[8]   = {0};

static uint8_t ReqPacket[256] = {0};
static uint8_t RspPacket[256] = {0};

//!-  The DataTypes defined in the MODBUS Protocol.

typedef uint8_t   Coils;
typedef uint8_t   Discrete_Input;
typedef uint16_t  Input_Registers;
typedef uint16_t  Holding_Registers;

/*
 *!-  The Below "Modbus_Data" Structure has all
 *!-  the DataTypes Supported by MODBUS.
 *!-  The Values in Member can be set as per
 *!-  the Device Requirements.
 */

typedef struct {
    Coils              Coil;
    Discrete_Input     DInput;
    Input_Registers    IRegister[10];
    Holding_Registers  HRegister;
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
    uint8_t      Device_ID;
    uint8_t      Function_Code;
    uint8_t      Subfunction_Code;
    uint8_t      Address_Hi;
    uint8_t      Address_Low;
    uint8_t      Registers_Quantity_Hi;
    uint8_t      Registers_Quantity_Low;
    uint8_t      Byte_Count;
    Modbus_Data  Holding_Register;
    uint8_t      CRC_Hi;
    uint8_t      CRC_Low;
} Modbus_Packet;

typedef enum {
    Fun_Code00 = 0x00,  //!-  Invalid Function Code
    Fun_Code08 = 0x08,  //!-  Diagnostic
    Fun_Code11 = 0x0B,  //!-  Get COM Event Counter
    Fun_Code12 = 0x0C,  //!-  Get COM Event Log
    Fun_Code17 = 0x11,  //!-  Report Slave ID
    Fun_Code43 = 0x2B,  //!-  Read Device Identification

    //!-  Function Codes that are Generally Used.
    //!-  Class 0 Codes
    Fun_Code03 = 0x03,  //!-  Read Multiple/Holding Registers
    Fun_Code16 = 0x10,  //!-  Write Multiple Registers

    //!-  Class 1 Codes
    Fun_Code01 = 0x01,  //!-  Read Coils Status
    Fun_Code02 = 0x02,  //!-  Read Discrete Inputs Status
    Fun_Code04 = 0x04,  //!-  Read Input Registers
    Fun_Code05 = 0x05,  //!-  Write Single Coil
    Fun_Code06 = 0x06,  //!-  Write Single Register
    Fun_Code07 = 0x07,  //!-  Read Exception Status (serial-only)

    //!-  Class 2 Codes
    Fun_Code24 = 0x18,  //!-  Read FIFO
    Fun_Code23 = 0x17,  //!-  Read/Write Multiple Registers
    Fun_Code22 = 0x16,  //!-  Mask Write Register
    Fun_Code21 = 0x15,  //!-  Write File Record
    Fun_Code20 = 0x14,  //!-  Read File Record
    Fun_Code15 = 0x0F   //!-  Write Multiple Coils
} Function_Code;

typedef enum {
    SubFun_Code00 = 0x00,  //!-  Return Query Data
    SubFun_Code01 = 0x01,  //!-  Restart Communications Option
    SubFun_Code02 = 0x02,  //!-  Return Diagnostic Register
    SubFun_Code03 = 0x03,  //!-  Change ASCII Input Delimiter
    SubFun_Code04 = 0x04,  //!-  Force Listen Only Mode
    SubFun_Code10 = 0x0A,  //!-  Clear Counters and Diagnostic Register
    SubFun_Code11 = 0x0B,  //!-  Return Bus Message Count
    SubFun_Code12 = 0x0C,  //!-  Return Bus Communication Error Count
    SubFun_Code13 = 0x0D,  //!-  Return Bus Exception Error Count
    SubFun_Code14 = 0x0E,  //!-  Return Slave Message Count
    SubFun_Code15 = 0x0F,  //!-  Return Slave No Response Count
    SubFun_Code16 = 0x10,  //!-  Return Slave NAK Count
    SubFun_Code17 = 0x11,  //!-  Return Slave Busy Count
    SubFun_Code18 = 0x12,  //!-  Return Bus Character Overrun Count
    SubFun_Code20 = 0x14,  //!-  Clear Overrun Counter and Flag
} Sub_Function_Code;

typedef enum {
    ILLEGAL_FUNCTION                = 0x01,
    ILLEGAL_DATA_ADDRESS            = 0x02,
    ILLEGAL_DATA_VALUE              = 0x03,
    SLAVE_DEVICE_FAILURE            = 0x04,
    ACKNOWLEDGE                     = 0x05,
    SLAVE_DEVICE_BUSY               = 0x06,
    MEMORY_PARITY_ERROR             = 0x08,
    GATEWAY_PATH_UNAVAILABLE        = 0x0A,
    TARGET_DEVICE_FAILED_TO_RESPOND = 0x0B
} Modbus_Exception_Code;

/****************************************************************************
!-  GLOBAL FUNCTIONS
*****************************************************************************/

Bool Modbus_Init(
        void);

void Modbus_Main(
        void);

uint8_t Get_Exception_FunctCode(
        uint8_t const FunctionCode);

Bool Set_Device_ID(
        Modbus_Packet *const UsrPacket,
        uint8_t const UsrDevID);

Bool Set_Function_Code(
        Modbus_Packet *const UsrPacket,
        uint8_t const UsrFunctCode);

Bool Set_SubFunction_Code(
        Modbus_Packet *const UsrPacket,
        uint8_t const UsrSubFunctCode);

Bool Set_StartAddress(
        Modbus_Packet *const UsrPacket,
        uint16_t const UsrStartAddress);

Bool Set_Register_Quantity(
        Modbus_Packet *const UsrPacket,
        uint16_t const UsrRegQuantity);

Bool Set_WriteSingleRegister(
        Modbus_Packet *const UsrPacket,
        uint16_t const UsrData);

void Clear_Frame(
        Modbus_Packet *const PacketPtr);

uint8_t Get_Byte_Counts(
        const uint8_t ReqFunctionCode,
        const uint16_t ReqRegistersQuantity);

uint8_t * Modbus_Request(
        Modbus_Packet *const UsrPacket);

uint8_t * Modbus_Response(
        Modbus_Packet *const UsrPacket);

#endif  /* MODBUS_H */






