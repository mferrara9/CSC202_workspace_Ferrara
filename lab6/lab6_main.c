//*****************************************************************************
//*****************************    C Source Code    ***************************
//*****************************************************************************
//  DESIGNER NAME:  Mason Ferrara
//
//       LAB NAME:  Lab 6
//
//      FILE NAME:  lab6_main.c
//
//-----------------------------------------------------------------------------
//
// DESCRIPTION:
//    This program serves as a ... 
//
//*****************************************************************************
//*****************************************************************************

//-----------------------------------------------------------------------------
// Loads standard C include files
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Loads MSP launchpad board support macros and definitions
//-----------------------------------------------------------------------------

#include <stdio.h>
#include <ti/devices/msp/msp.h>
#include "clock.h"
#include "LaunchPad.h"
#include "lcd1602.h"


//-----------------------------------------------------------------------------
// Define function prototypes used by the program
//-----------------------------------------------------------------------------
void run_lab6_part1(void);
void lab6_part1_to_part2(void);
//-----------------------------------------------------------------------------
// Define symbolic constants used by the program
//-----------------------------------------------------------------------------
#define Five_Hundred_Millisec_Pause (0x1F4)
#define Two_Hundred_Millisec_Pause (0xC8)
#define One_Hundred_Millisec_Pause (0x64)
#define TEN_MILLISEC_PAUSE (0xA)
#define Five_Millisec_Pause (0x5)
#define One_Sec_Pause (0x3E8)
#define Two_Sec_Pause (0x7D0)
#define Three_Sec_Pause (0xBB8)

//-----------------------------------------------------------------------------
// Define global variables and structures here.
// NOTE: when possible avoid using global variables
//-----------------------------------------------------------------------------


// Define a structure to hold different data types

int main(void)
{
  clock_init_40mhz();
  launchpad_gpio_init();
  I2C_init();
  lcd1602_init();
  lcd_clear();
  


  run_lab6_part1();

  lab6_part1_to_part2();





 // Endless loop to prevent program from ending
 while (1);

} /* main */


void run_lab6_part1() {
    
    uint8_t letter;

    lcd_set_ddram_addr(LCD_LINE1_ADDR);
    for (letter = 'A'; letter <= 'Z'; letter++)
        {
            lcd_write_char(letter);
            if (letter == 'A' + CHARACTERS_PER_LCD_LINE - 0x01)
            {
                lcd_set_ddram_addr(LCD_LINE2_ADDR);
            }
        }
}

void lab6_part1_to_part2() {

    char message1[] = "Running Part 2";
    while (is_pb_up(2)) {
      msec_delay(TEN_MILLISEC_PAUSE);
    }
    while (is_pb_down(2)) {
      lcd_clear();
      lcd_write_string(message1);
      msec_delay(One_Sec_Pause);
      lcd_clear();
      msec_delay(TEN_MILLISEC_PAUSE);
    }
}

