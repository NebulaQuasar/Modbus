/****************************************************************************/
/*!-  File Name: Modbus_Slave.c
 ****************************************************************************/
#include <Modbus.h>

int main() {
    Bool Ckeck_OK = FALSE;
    RTU_Configuration RTU_Config;
    Modbus_Packet Master_Request;

    //!-  Set the Configuration for UART Connection.
    RTU_Config->baudrate = Baudrate9600;
    RTU_Config->data_bits = DataBits8;
    RTU_Config->parity = EvenParity;
    RTU_Config->stop_bits = StopBits0;

    //!-  Receive the Request from Master.
    Receive_Request();
    //!-  Validate the Function Code from Master.

    //!-  Validate the Address Code from Master.

    //!-  Validate the Data Value from Master.

    //!-  Process the Received Function Code.

    //!-  Send the Response to Master.

}
