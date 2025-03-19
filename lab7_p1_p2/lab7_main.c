//*****************************************************************************
//*****************************    C Source Code    ***************************
//*****************************************************************************
//  DESIGNER NAME:  Mason Ferrara
//
//       LAB NAME:  Lab 7
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
#include <cstdint>
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

void run_lab7_part1(void);

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
#define SYS_TICK_TIMEOUT_PERIOD (0x63FFF)

#define NUM_STATES (18)
//-----------------------------------------------------------------------------
// Define global variables and structures here.
// NOTE: when possible avoid using global variables
//-----------------------------------------------------------------------------


// Define a structure to hold different data types
const uint8_t seg7_letter_code[] =
{
    0x6D, 0x00, 0x6D, 0x00, 0x6D, 0x00, // S
    0x3F, 0x00, 0x3F, 0x00, 0x3F, 0x00, // O
    0x6D, 0x00, 0x6D, 0x00, 0x6D, 0x00  // S
};

const uint8_t delay_count[] =
{
    0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, // dots
    0x24, 0x24, 0x24, 0x24, 0x24, 0x24, // dash
    0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x30  // dots
};

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
 
  run_lab7_part1();

 // Endless loop to prevent program from ending
 while (1);

} /* main */

//-------------------------------------------------------------------------------------
void SysTick_Handler(void)
{
    static uint16_t delay_time = 1;
    static uint16_t code_index = 0;

    delay_time--;
    if (delay_time == 0)
    {
        seg7_on(seg7_letter_code[code_index], SEG7_DIG0_ENABLE_IDX);

        delay_time = delay_count[code_index];
        code_index++;

        if (code_index == NUM_STATES)
        {
            code_index = 0;
        }
    }
}

//-------------------------------------------------------------------------------------
void wait_for_pb_pressed(uint8_t pb_idx){
    while (is_pb_up(pb_idx));
    msec_delay(TEN_MILLISEC_PAUSE);
}

//-------------------------------------------------------------------------------------
void wait_for_pb_released(uint8_t pb_idx){
    while (is_pb_down(pb_idx));
    msec_delay(TEN_MILLISEC_PAUSE);
}

//-------------------------------------------------------------------------------------
void run_lab7_part1(){
    bool done;
    uint32_t lcd_address;
    char message1[] = "Microcontrollers are fun.";
    for ( lcd_address = 0x4F; lcd_address < 0x40; lcd_address--;)
    {
        lcd_set_ddram_addr(lcd_address);
        lcd_write_string(message1);
        msec_delay(Two_Hundred_Millisec_Pause);
        lcd_clear();
        if (is_pb_down(PB1_IDX))
        {
            done = true;
        }
        
    }
    while (messsage[idx] != '\0') && !done 
    {
        lcd_set_ddram_addr(0x40);
        lcd_write_string();
        msec_delay(Two_Hundred_Millisec_Pause);
        is_pb_down(PB1_IDX);
        idx++
    }
}

//-------------------------------------------------------------------------------------