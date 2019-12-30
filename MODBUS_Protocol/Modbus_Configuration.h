/*
 * Modbus_Configuration.h
 *
 *  Created on: 22-Dec-2019
 *      Author: root
 */

/****************************************************************************
!-  File Name:: Modbus_Configuration.h
 ****************************************************************************/

#ifndef __MODBUS_CONFIGURATION_H_
#define __MODBUS_CONFIGURATION_H_

//!-  Headers
#include <stdint.h>

//!-  Defining Unsigned Character as "Bool"
typedef unsigned char  Bool;

//!-  The DataTypes defined in the MODBUS Protocol.
typedef uint8_t   Coils;
typedef uint8_t   Discrete_Inputs;
typedef uint16_t  Input_Registers;
typedef uint16_t  Holding_Registers;

//!-  Minimum Number of Coils/Registers
#define MIN_COILS_NUMBERS              (0)
#define MIN_DISCRETE_INPUTS_NUMBERS    (0)
#define MIN_INPUT_REGISTERS_NUMBERS    (0)
#define MIN_HOLDING_REGISTERS_NUMBERS  (0)

//!-  Maximum Number of Coils/Registers
#define MAX_COILS_NUMBERS              (256)
#define MAX_DISCRETE_INPUTS_NUMBERS    (256)
#define MAX_INPUT_REGISTERS_NUMBERS    (65536)
#define MAX_HOLDING_REGISTERS_NUMBERS  (65536)

//!-  Minimum Number of Coils/Registers
#define MIN_COILS_ADDRESS              (0x0000)
#define MIN_DISCRETE_INPUTS_ADDRESS    (0x0000)
#define MIN_INPUT_REGISTERS_ADDRESS    (0x0000)
#define MIN_HOLDING_REGISTERS_ADDRESS  (0x0000)

//!-  Maximum Number of Coils/Registers
#define MAX_COILS_ADDRESS              (0x00FF)
#define MAX_DISCRETE_INPUTS_ADDRESS    (0x00FF)
#define MAX_INPUT_REGISTERS_ADDRESS    (0xFFFF)
#define MAX_HOLDING_REGISTERS_ADDRESS  (0xFFFF)

/*
 *!-  Configure Number of Coils & Registers
 *!-  to create Virtual Coils/Registers
 *!-  as per the Device Requirements.
 */
#define COILS_NUMBERS              (0)
#define DISCRETE_INPUTS_NUMBERS    (0)
#define INPUT_REGISTERS_NUMBERS    (0)
#define HOLDING_REGISTERS_NUMBERS  (1)

#define COILS_ADDR              \
                (COILS_NUMBERS - 1)
#define DISCRETE_INPUTS_ADDR    \
                (DISCRETE_INPUTS_NUMBERS - 1)
#define INPUT_REGISTERS_ADDR    \
                (INPUT_REGISTERS_NUMBERS - 1)
#define HOLDING_REGISTERS_ADDR  \
                (HOLDING_REGISTERS_NUMBERS - 1)

#endif /* __MODBUS_CONFIGURATION_H_ */
