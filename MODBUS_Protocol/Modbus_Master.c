/****************************************************************************
!-  File Name: Modbus_Master.c
 ****************************************************************************/

//!-  Headers
#include <Modbus.h>

/****************************************************************************
!-  LOCAL DEFINITIONS
*****************************************************************************/

#define SLAVE_ID_1 1
#define FCODE_03 03
#define SUBFCODE_0 00

/****************************************************************************
!-  LOCAL VARIABLES
*****************************************************************************/

static Modbus_Packet ModbusFrame;

uint16_t Address = 0x0000;
uint16_t RegQty  = 1;
uint16_t Data    = 1;
uint8_t *PacketPtr = NULL;

/****************************************************************************
!-  LOCAL FUNCTIONS
*****************************************************************************/

int main() {

    Bool Ckeck_OK = FALSE;

    //!-  Call to Modbus_Init() Function.
    Ckeck_OK = Modbus_Init();
    if (Ckeck_OK != TRUE) {
        printf("Error in MODBUS Initialization \n");
        exit(1);
    }

    //!-  Call to Set_Device_ID() Function.
    //!-  This will set the Valid Device ID.
    Ckeck_OK = Set_Device_ID(&ModbusFrame, SLAVE_ID_1);
    if (Ckeck_OK != TRUE) {
        printf("Error: Check Device ID \n");
        exit(1);
    }

    //!-  Call to Set_Function_Code() Function.
    //!-  This will set the Valid Function Code.
    Ckeck_OK = Set_Function_Code(&ModbusFrame, FCODE_03);
    if (Ckeck_OK != TRUE) {
        printf("Error: Check Function Code \n");
        exit(1);
    }

    //!-  Call to Set_SubFunction_Code() Function.
    //!-  This will set the Valid Sub Function Code.
    //!-  This call is usually Not Used.
    //!-  Set Correct Function Code (i.e., 08 or 43) to use this Call.
    Ckeck_OK = Set_SubFunction_Code(&ModbusFrame, SUBFCODE_0);
    if (Ckeck_OK != TRUE) {
        printf("Error: Check Sub Function Code \n");
        exit(1);
    }

    //!-  Call to Set_StartAddress() Function.
    //!-  This will set the Valid Start Address.
    Ckeck_OK = Set_StartAddress(&ModbusFrame, Address);
    if (Ckeck_OK != TRUE) {
        printf("Error: Check Start Address \n");
        exit(1);
    }

    //!-  Call to Set_Register_Quantity() Function.
    //!-  This will set the Valid Register Quantity.
    Ckeck_OK = Set_Register_Quantity(&ModbusFrame, RegQty);
    if (Ckeck_OK != TRUE) {
        printf("Error: Check Register Quantity \n");
        exit(1);
    }

    //!-  Call to Set_WriteSingleRegister() Function.
    //!-  This will set the Data Value into Holding Register.
    Ckeck_OK = Set_WriteSingleRegister(&ModbusFrame, Data);
    if (Ckeck_OK != TRUE) {
        printf("Error: Check Data \n");
        exit(1);
    }

    //!-  Now the Structure Modbus Frame contains all the Required data
    //!-  To make a Request to Slave Device.

    //!-  Call to Modbus_Request() Function.
    //!-  This will form a Packet that can be sent on UART.
    PacketPtr = Modbus_Request(&ModbusFrame);

    //!-  The PacketPtr will be Send to USART.

}





























