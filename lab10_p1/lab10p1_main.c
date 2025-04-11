//*****************************************************************************
//*****************************    C Source Code    ***************************
//*****************************************************************************
//  DESIGNER NAME:  Mason Ferrara
//
//       LAB NAME:  Lab 10 Part 1
//
//      FILE NAME:  lab10p1_main.c
//
//-----------------------------------------------------------------------------
// DESCRIPTION:
//      This lab introduces students to the fundamentals of serial communication  
// using the UART (Universal Asynchronous Receiver-Transmitter) module on the  
// MSPM0G3507 microcontroller. Unlike parallel communication, which transmits  
// multiple bits simultaneously, serial communication sends data one bit at a time,  
// making it efficient for long-distance and low-pin-count applications.
//
//      Students will configure the UART module to send and receive data via a  
// serial terminal, gaining hands-on experience with basic data exchange, character  
// echoing, and string transmission. The lab also introduces a simple menu-driven  
// interface, enabling structured user interaction with the microcontroller.
//
//      These exercises provide a foundation for working with more advanced serial  
// protocols and demonstrate how microcontrollers communicate with external devices  
// in real-time embedded systems.
//
//*****************************************************************************
//*****************************************************************************

//-----------------------------------------------------------------------------
// Loads standard C include files
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Loads MSP launchpad board support macros and definitions
//-----------------------------------------------------------------------------

#include "LaunchPad.h"
#include "clock.h"
#include "lcd1602.h"
#include "uart.h"
#include <stdio.h>
#include <ti/devices/msp/msp.h>


//-----------------------------------------------------------------------------
// Define function prototypes used by the program
//-----------------------------------------------------------------------------

void run_lab10_part1(void);

//-----------------------------------------------------------------------------
// Define symbolic constants used by the program
//-----------------------------------------------------------------------------
#define Five_Hundred_Millisec_Pause (0x1F4)
#define Two_Hundred_Millisec_Pause (0xC8)
#define Two_Hundred_Fifty_Millisec_Pause (0xFA)
#define One_Hundred_Millisec_Pause (0x64)
#define TEN_MILLISEC_PAUSE (0xA)
#define Five_Millisec_Pause (0x5)
#define One_Sec_Pause (0x3E8)
#define Two_Sec_Pause (0x7D0)
#define Three_Sec_Pause (0xBB8)
#define MAX_CHAR_SIZE (16)
#define NULL_CHAR ('\0')
#define BACKSPACE (0x08)
//-----------------------------------------------------------------------------
// Define global variables and structures here.
// NOTE: when possible avoid using global variables
//-----------------------------------------------------------------------------

// Define a structure to hold different data types

int main(void) {
  clock_init_40mhz();
  launchpad_gpio_init();
  UART_init(115200);
  I2C_init();
  lcd1602_init();
  lcd_clear();

  run_lab10_part1();

  // Endless loop to prevent program from ending
  while (1)
    ;

} /* main */

//-----------------------------------------------------------------------------
// Description:
// This function executes Part 1 of Lab 10, which demonstrates basic UART input  
// handling and LCD display functionality. The user is prompted to enter a name  
// via a UART terminal interface, with each typed character echoed back in real-time.
//
// The input is collected into a buffer until the user presses the Enter key ('\r'),  
// which signals the end of input. Backspace handling is included, allowing the  
// user to correct input as needed.
//
// Once input is complete, the program displays "Name:" followed by the entered  
// text on the first line of the LCD. The second line of the LCD displays the  
// message "Program Stopped" to indicate termination of the input session.
//
// INPUT PARAMETERS:
//  none
//
// OUTPUT PARAMETERS:
//  none
//
// RETURN:
//  none
//-----------------------------------------------------------------------------
void run_lab10_part1() {
  bool done = false;
  int8_t buffer[MAX_CHAR_SIZE];
  char character = 0;
  uint8_t idx = 0;

  while (!done) {
    character = UART_in_char();
    UART_out_char(character);

    if (character == '\r') {
      done = true;
      buffer[idx++] = (int8_t)NULL_CHAR;
    }
    if (character == BACKSPACE) {
      buffer[idx--] = character;
    } else {
      buffer[idx++] = character;
    }
  }
  lcd_set_ddram_addr(LCD_LINE1_ADDR);
  lcd_write_string("Name:");
  lcd_write_string(buffer);
  lcd_set_ddram_addr(LCD_LINE2_ADDR);
  lcd_write_string("Program Stopped");
}
