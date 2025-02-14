//*****************************************************************************
//*****************************    Assembly Code    ***************************
//*****************************************************************************
//
//  DESIGNER NAME:  Mason Ferrara
//
//       LAB NAME:  Lab 3, part 2
//
//      FILE NAME:  lab3_p2.s
//
//-----------------------------------------------------------------------------
//
// DESCRIPTION:
//    This program serves as a practice exercise for students to implement 
//    bitwise operations in Assmebly. It includes a series of problems, each 
//    requiring the manipulation of bits in a test register using bitwise 
//    operations. The program initializes the UART communication, sets up 
//    the system clock, and provides a function 'msp_printf' to print 
//    messages over UART. 
//
//    Each problem is outlined with a description, and students are instructed 
//    to write code to solve the problem within the designated sections. 
//    Upon execution, the program demonstrates the effect of various 
//    bitwise operations on the test register, along with the corresponding 
//    output messages indicating the results of each operation. After 
//    completing all the problems, the program terminates.
//
//*****************************************************************************
//*****************************************************************************

// Make the following symbol available outside of this file 
.global my_asm_16bitset
.global my_asm_16bitclr
.global my_asm_16bitcheck




//-----------------------------------------------------------------------------
// DESCRIPTION:
//  This function sets the specified bit(s) in a 16-bit register value using
//  the provided bit mask. It performs a bitwise OR operation to set the bit(s).
//
// INPUT PARAMETERS:
//  R0 - The original 16-bit register value.
//  R1 - The bit mask indicating which bit(s) to clear.
//
// OUTPUT PARAMETERS:
//  none
//
// RETURN:
//  uint16_t - The modified register value with the specified bit(s) set.
// -----------------------------------------------------------------------------
my_asm_16bitset:
ORRS  R0,  R1       //R0 = R0 | R1
bx lr





//-----------------------------------------------------------------------------
// DESCRIPTION:
//  This function clears the specified bit(s) in a 16-bit register value using
//  the provided bit mask. It performs a bitwise AND operation with the
//  complement of the bit mask to clear the bit(s).
//
// INPUT PARAMETERS:
//  R0 - The original 16-bit register value.
//  R1 - The bit mask indicating which bit(s) to clear.
//
// OUTPUT PARAMETERS:
//  none
//
// RETURN:
//  uint16_t - The modified register value with the specified bit(s) cleared.
// -----------------------------------------------------------------------------
my_asm_16bitclr:
MVNS  R1, R1        // ~MASK
ANDS  R0,  R1       //R0 = R0 & R1
bx lr 




//-----------------------------------------------------------------------------
// DESCRIPTION:
//  This function checks if the specified bit(s) in a 16-bit register value are
//  set using the provided bit mask. It performs a bitwise AND operation to 
//  verify if the bit(s) are set.
//
// INPUT PARAMETERS:
//  reg_value - a 16-bit register value to check.
//  bit_mask  - a 16-bit mask indicating which bit(s) to check.
//
// OUTPUT PARAMETERS:
//  none
//
// RETURN:
//  bool - true if the specified bit(s) are set, false otherwise.
// -----------------------------------------------------------------------------
my_asm_16bitcheck:
ANDS R0, R1         //R0 = R0 & R1
CMP  R0, R1         //R0 = R1 ?
BEQ  bit_set        // Branch to bit_set if R0=R1
MOVS  R0, #0
bx lr

bit_set:
MOVS  R0, #1
bx lr 


.end