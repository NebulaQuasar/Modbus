#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>
#include <limits.h>
#include <time.h>

#ifndef MODBUS_H
#define MODBUS_H

#define TRUE                        1
#define FALSE                       0 
#define ON                          1 
#define OFF                         0

#define MODBUS_MAX_ADU_LENGTH       256
#define MODBUS_MAX_PDU_LENGTH       253

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
}   Fun_Code;

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
}   Modbus_Exception_Code;

typedef struct Configuration{
/*
 * Define the Required Structure Members Here.
 * Like Baudrate, COM Port etc.
 */
} RTU_Config;

typedef union {

    uint8_t     Coil;
    uint8_t     Discrete_Input;
    uint16_t    Input_Registers;
    uint16_t    Holding_Registers;

} Modbus_Data;


/*
 *  Device_ID: Valid slave device addresses are in 
 *  the range of 0 – 247 decimal. Address 0 is used for 
 *  the broadcast address, which all slave devices recognize
 *
 *
 * */

typedef struct {
    uint8_t     Device_ID;
    uint8_t     Function_Code;
    uint8_t     Subfunction_Code;
    uint8_t     Address_High;
    uint8_t     Address_Low;
    Modbus_Data Data;
    uint16_t    CRC_Check;
} Modbus;

/*
 * To Setup the Serial Connection
 */
void Setup_RTU_Connection(/*parameters*/);

/*
 * The function code is the first item to be validated. 
 * If the function code is not recognized by the device receiving the
 * request, it responds with an exception
 */
bool Validate_Function_Code(/*parameters*/);

/* Each request from a master can perform a single operation on a
 * single bank of data. 
 * That is, a single request can read from the coils bank or write to the
 * coils bank, but not both. 
 * There are exceptions, however. For example, function code 23 allows
 * a master to write holding registers and read holding registers in a
 * single request/response cycle. 
 * However, this is not a commonly implemented function code. 
 * Both master and slave device documentation should be checked to
 * verify whether this function code is available.
 */

void    Modbus_Request(/*parameters*/);

void    Modbus_Response(/*parameters*/);

void    Modbus_Exception_Response(/*parameters*/);

uint8_t     Modbus_Read_Bits(/*parameters*/);

uint8_t     Modbus_Write_Bits(/*parameters*/);

uint16_t    Modbus_Read_Registers(/*parameters*/);

uint16_t    Modbus_Write_Registers(/*parameters*/);

uint16_t    Modbus_CRC_Calculate(/*parameters*/);

#endif  /* MODBUS_H */






