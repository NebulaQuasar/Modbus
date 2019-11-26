/****************************************************************************/
/*!-  File Name: Modbus_Master.c
 ****************************************************************************/
#include <Modbus.h>

static Modbus_Packet ModbusFrame;

int main() {
    Bool Ckeck_OK = FALSE;
    RTU_Configuration RTU_Config;
    Modbus_Packet Master_Request;

    //!-  Set the Configuration for UART Connection.
    RTU_Config->baudrate = Baudrate9600;
    RTU_Config->data_bits = DataBits8;
    RTU_Config->parity = EvenParity;
    RTU_Config->stop_bits = StopBits0;

    //!-  Start a Timer.
    //!-  While(Time Not Complete) keep trying.
    Ckeck_OK = Setup_RTU_Connection(&RTU_Config);
    //!-  if (Ckeck_OK == FALSE) {
    //!-  printf("Trying to Connect... \n");
    //!-  }
    //!-  if (Ckeck_OK == TRUE) {break;}
    //!-  if (Timer Over) (break;)

    if (Ckeck_OK == FALSE) {
        printf("Error in Establishing Connection \n");
        //!-  exit(1);
    }

    //!-  Set Device ID of the Slave.
    Master_Request->Device_ID = 0x0A;  //!-  10 in Decimal

    //!-  Set Function Code of the Slave.
    Master_Request->Function_Code = 0x03;  //!-  03 in Decimal

    //!-  Set Sub-Function Code of the Slave.
    Master_Request->Subfunction_Code = 0x00;  //!-  00 in Decimal

    //!-  Set Start Address of the Slave.
    Master_Request->Address_Hi = 0x00;  //!-  00 in Decimal
    Master_Request->Address_Low = 0x00;  //!-  00 in Decimal

    //!-  Set Data to be Preset in the Slave.
    //!-  Not Needed Here as of now since function code is 03.

    //!-  Set Register Quantity.
    Master_Request->Registers_Quantity_Hi = 0x00;
    Master_Request->Registers_Quantity_Low = 0x01;

    //!-  Call the Request API of MODBUS.
    Modbus_Request(&Master_Request);

    //!-  Call the send API of MODBUS.


	Setup_RTU_Connection();


      Construct_Message(&ModbusFrame);
//      Uint8 u8Data[2];
//      Unpack16_8bits(u16AbsLoadTo16BitKg, &u8Data[0], &u8Data[1]);
}

void Construct_Message(Modbus_Packet *const MessagePtr){

    Funct_Code Read_Holding_Register = Fun_Code03;

    MessagePtr->Device_ID                      = 01;
    MessagePtr->Function_Code                  = Read_Holding_Register;
    MessagePtr->Subfunction_Code               = 0x00;
    MessagePtr->Address_Hi                     = 0x00;
    MessagePtr->Registers_Quantity_Hi          = 01;
    MessagePtr->Holding_Register->HRegister    = 01;

}
