/*
 * The function code field of a MODBUS data unit is coded in one byte.
 * Valid codes are in the range of 1 ... 255 decimal (the range 128 – 255 is 
 * reserved and used for exception responses).
 * When a message is sent from a Client to a Server device the function code
 * field tells the server what kind of action to perform
 * Function code "0" is not valid.
 * Sub-function codes are added to some function codes to define 
 * multiple actions.
 * */

#include <Modbus.h>

int main(){

}

bool Validate_Function_Code(){
    bool Ckeck_OK   = FALSE;
    uint16_t Code   = 0;
    
    // code = modbus -> Function_Code;
    /* Check the Function Code Here.*/
    
    switch (Code){

            case Fun_Code03:
            case Fun_Code16:
            case Fun_Code01:
            case Fun_Code02:
            case Fun_Code04:
            case Fun_Code05:
            case Fun_Code06:
            case Fun_Code07:
            case Fun_Code24:
            case Fun_Code23:
            case Fun_Code22:
            case Fun_Code21:
            case Fun_Code20:
            case Fun_Code15: 
                Check_OK = TRUE;
                break;
            default:
                Ckeck_OK = FALSE;
                break;
    }
    
    return Ckeck_OK;
}


void Modbus_Request(/*parameters*/){

}

void Modbus_Response(/*parameters*/){

    // Check Function Code --> Validate_Function_Code();
    // Check Address    --> Validate_Address();
    // Process the Requested Function Code.
    // Read / Write data in Registers/Coils
    // Respond Back
    // In case any ERROR in between Call    -->  Modbus_Exception_Response();
}

void Modbus_Exception_Response(/*parameters*/){

}

uint8_t   Modbus_Read_Bits(/*parameters*/){

}

uint8_t   Modbus_Write_Bits(/*parameters*/){

}

uint16_t Modbus_Read_Registers(/*parameters*/){

}

uint16_t Modbus_Write_Registers(/*parameters*/){

}














