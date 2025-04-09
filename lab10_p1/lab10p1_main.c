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
#include <stdio.h>
#include <ti/devices/msp/msp.h>
#include "uart.h"

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
    }
    else {
            buffer[idx++] = character;
    }
  }
  lcd_set_ddram_addr(LCD_LINE1_ADDR);
  lcd_write_string("Name:");
  lcd_write_string(buffer);
  lcd_set_ddram_addr(LCD_LINE2_ADDR);
  lcd_write_string("Program Stopped");
}
