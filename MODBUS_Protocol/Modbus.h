/*
 * Modbus.h
 *
 *  Created on: 22 Dec 2019
 *      Author: root
 */

/****************************************************************************
!-  File Name: Modbus.h
*****************************************************************************/

#ifndef __MODBUS_H_
#define __MODBUS_H_

//!-  Headers
#include <Modbus_Configuration.h>
#include <CRC.h>

/****************************************************************************
!-  GLOBAL DEFINITIONS
*****************************************************************************/

#define NULL  __null

#define TRUE   1
#define FALSE  0
#define ON     1
#define OFF    0
#define ABSENT 0

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

/*
 *!-  "Modbus_Data" STRUCTURE has All
 *!-  the DATATYPES Supported by MODBUS Protocol.
 *!-  The Values in Member can be set as per
 *!-  the Device Requirements.
 */
typedef struct {
    Coils              Coils      [COILS_NUMBERS] ;
    Discrete_Inputs    DInputs    [DISCRETE_INPUTS_NUMBERS];
    Input_Registers    IRegisters [INPUT_REGISTERS_NUMBERS];
    Holding_Registers  HRegisters [HOLDING_REGISTERS_NUMBERS];
} Modbus_Data;

//!-  "MODBUS_DATABASE" as a Virtual Storage Device.
static Modbus_Data  MODBUS_DATABASE;

/*
 *!-  "Function_Code" ENUM is used to Enumerate Some of the
 *!-  Basic Function Codes Supported by MODBUS Protocol.
 */
typedef enum {
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
    Fun_Code15 = 0x0F,  //!-  Write Multiple Coils
    Fun_Code20 = 0x14,  //!-  Read File Record
    Fun_Code21 = 0x15,  //!-  Write File Record
    Fun_Code22 = 0x16,  //!-  Mask Write Register
    Fun_Code23 = 0x17,  //!-  Read/Write Multiple Registers
    Fun_Code24 = 0x18,  //!-  Read FIFO
} Function_Code;

/*
 *!-  "Modbus_Exception_Code" ENUM is used to Enumerate
 *!-  Basic Exception Codes Supported by MODBUS Protocol.
 */
typedef enum {
    ILLEGAL_FUNCTION                = 0x01,
    ILLEGAL_DATA_ADDRESS            = 0x02,
    ILLEGAL_DATA_VALUE              = 0x03,
    SLAVE_DEVICE_FAILURE            = 0x04,
    ACKNOWLEDGE                     = 0x05,
    SLAVE_DEVICE_BUSY               = 0x06,
    MEMORY_PARITY_ERROR             = 0x08,
    GATEWAY_PATH_UNAVAILABLE        = 0x0A,
    TARGET_DEVICE_FAILED_TO_RESPOND = 0x0B,
} Modbus_Exception_Code;

/*
 *!-  Device_ID:
 *!-  Valid slave device addresses are in
 *!-  the range of 0 – 247 decimal. Address 0 is used for
 *!-  the broadcast address, which all slave devices Recognize.
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
    uint8_t      Address_Hi;
    uint8_t      Address_Low;
    uint8_t      Registers_Quantity_Hi;
    uint8_t      Registers_Quantity_Low;
    uint8_t      Byte_Count;
    Modbus_Data  Data;
    uint8_t      CRC_Hi;
    uint8_t      CRC_Low;
} Modbus_Packet;

/****************************************************************************
!-  GLOBAL FUNCTIONS
*****************************************************************************/

Bool Modbus_Init(
        void);

Bool Set_Single_Coil(
        uint8_t const Coil_Number,
        uint8_t const Value);

Bool Set_Single_Discrete_Input(
        uint8_t const DInput_Number,
        uint8_t const Value);

Bool Set_Single_Input_Register(
        uint16_t const InputReg_Number,
        uint16_t const Value);

Bool Set_Single_Holding_Register(
        uint16_t const HoldingReg_Number,
        uint16_t const Value);

uint8_t Get_Single_Coil(
        uint8_t const Coil_Number);

uint8_t Get_Single_Discrete_Input(
        uint8_t const DInput_Number);

uint16_t Get_Single_Input_Register(
        uint16_t const InputReg_Number);

uint16_t Get_Single_Holding_Register(
        uint16_t const HoldingReg_Number);

#endif /* __MODBUS_H_ */
