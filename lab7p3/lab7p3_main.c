//*****************************************************************************
//*****************************    C Source Code    ***************************
//*****************************************************************************
//  DESIGNER NAME:  Mason Ferrara
//
//       LAB NAME:  Lab 7 part 3
//
//      FILE NAME:  lab7_main.c
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
#define MSPM0_CLOCK_FREQUENCY (40E6)
#define SYST_TICK_PERIOD (10.25E-3)
#define SYST_TICK_PERIOD_COUNT (SYST_TICK_PERIOD * MSPM0_CLOCK_FREQUENCY)

void SysTick_Handler(void);
void wait_for_pb_pressed(uint8_t pb_idx);
void wait_for_pb_released(uint8_t pb_idx);


void run_lab7_part3(void);


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
#define MAX_COUNT_VALUE (100)
#define MIN_COUNT_VALUE (0)

#define SYS_TICK_TIMEOUT_PERIOD (0x63FFF)




//-----------------------------------------------------------------------------
// Define global variables and structures here.
// NOTE: when possible avoid using global variables
//-----------------------------------------------------------------------------


// Define a structure to hold different data types


int main(void)
{
  clock_init_40mhz();
  launchpad_gpio_init();
  led_init();
  led_disable();
  seg7_init();
  I2C_init();
  lcd1602_init();
  dipsw_init();
  lcd_clear();

  sys_tick_init(SYST_TICK_PERIOD_COUNT);
 
  run_lab7_part3();

  //sys_tick_disable();
  //seg7_off();

 // Endless loop to prevent program from ending
 while (1);

} /* main */

//-------------------------------------------------------------------------------------
void SysTick_Handler(void)
{
    uint32_t mask = 1;
    uint32_t count = 0;
    uint32_t read;
    uint32_t index;

    read = dipsw_read();

    for (index = 0; index < 4; index++)
    {
        if ((read & mask) == mask)
        {
            count = count + 1;
        }
        mask <<= 1;
    }
    seg7_hex(count, SEG7_DIG0_ENABLE_IDX);
}

//-------------------------------------------------------------------------------------
void run_lab7_part3()
{
    bool done = false;
    uint8_t count = MAX_COUNT_VALUE;

    lcd_clear();

    while (!done) {

        lcd_set_ddram_addr(LCD_LINE1_ADDR + 6);
        lcd_write_byte(count--);
        msec_delay(Two_Hundred_Millisec_Pause);

        if (count == MIN_COUNT_VALUE)
        {
            count = MAX_COUNT_VALUE;

            lcd_set_ddram_addr(LCD_LINE1_ADDR + 6);
            lcd_write_byte(count);
        }

        lcd_clear();
    }
    
}


    