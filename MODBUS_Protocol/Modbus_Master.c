/****************************************************************************/
/*!-  File Name: Modbus_Master.c
 ****************************************************************************/
#include <Modbus.h>


static Modbus_Packet ModbusFrame;


int main(){

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
