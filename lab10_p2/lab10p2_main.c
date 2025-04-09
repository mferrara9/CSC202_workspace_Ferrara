//*****************************************************************************
//*****************************    C Source Code    ***************************
//*****************************************************************************
//  DESIGNER NAME:  Mason Ferrara
//
//       LAB NAME:  Lab 10 Part 2
//
//      FILE NAME:  lab10p2_main.c
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
#include "adc.h"

//-----------------------------------------------------------------------------
// Define function prototypes used by the program
//-----------------------------------------------------------------------------

void run_lab10_part2(void);
void uart_string_printf(const char *str);
void flash_leds(void);
void inc_seg7(bool);
void display_temp(void);


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
#define ALL_LEDS (0xFF)
#define CHANNEL_5 (0x05)
#define DEGREE_SYMBOL (0xDF)

//-----------------------------------------------------------------------------
// Define global variables and structures here.
// NOTE: when possible avoid using global variables
//-----------------------------------------------------------------------------


// Define a structure to hold different data types

int main(void) {
  clock_init_40mhz();
  launchpad_gpio_init();
  led_init();
  seg7_init();
  UART_init(115200);
  I2C_init();
  lcd1602_init();

  lcd_clear();
  ADC0_init(ADC12_MEMCTL_VRSEL_INTREF_VSSA);

  run_lab10_part2();

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
void run_lab10_part2() {
  bool done = false;
  char character = 0;
  leds_off();
  inc_seg7(false);

  while (!done) {
    inc_seg7(false);
    uart_string_printf("\r\n MENU OPTIONS: \r\n");
    uart_string_printf("1. Increment count on 7-segment display\n");
    uart_string_printf("2. Show current temperature\n");
    uart_string_printf("3. Flash LEDs 3 times\n");
    uart_string_printf("4. End Program\n");
    uart_string_printf("\r\n Enter your selection: ");

    character = UART_in_char();
    UART_out_char(character);

    switch (character) {
      case '1':
        uart_string_printf("\n\rOption 1 detected \n\r\n\r");
        inc_seg7(true);
        break;

      case '2':
        uart_string_printf("\n\rOption 2 detected \n\r\n\r");
        display_temp();
        break;

      case '3':
        uart_string_printf("\n\rOption 3 detected \n\r\n\r");
        seg7_off();
        led_enable();
        flash_leds();
        led_disable();
        inc_seg7(false);
        break;

      case '4':
        uart_string_printf("\n\rOption 4 detected \n\r\n\r");
        uart_string_printf("\r\nThank you for using the program\r\n");
        done = true;
        break;
      default:
        uart_string_printf("\r\nERROR: Invalid Input\r\n");
      }
  }
  lcd_clear();
  lcd_set_ddram_addr(LCD_LINE1_ADDR);
  lcd_write_string("Program Stopped");
}

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
void flash_leds() 
{
    uint8_t idx = 0;
    
    for(idx = 0; idx < 3; idx++)
    {
        leds_on(ALL_LEDS);
        msec_delay(Two_Hundred_Fifty_Millisec_Pause);
        leds_off();
        msec_delay(Two_Hundred_Fifty_Millisec_Pause);
    }
    
}

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
void inc_seg7(bool inc_enable) 
{
    static uint8_t count = 0;

    if (inc_enable)
    {   count++;
        if(count == 10)
        count = 0;
    }
    seg7_hex(count, SEG7_DIG0_ENABLE_IDX);
}

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
void display_temp() 
{
    uint16_t temp_raw = ADC0_in(CHANNEL_5);
    uint16_t temp_degrees_c = (uint16_t)thermistor_calc_temperature(temp_raw);
    uint16_t temp_degrees_f = ((temp_degrees_c * 9) / 5) + 32;

    lcd_set_ddram_addr(LCD_LINE1_ADDR);
    lcd_write_string("TEMP =");
    lcd_set_ddram_addr(LCD_LINE1_ADDR + 5);
    lcd_write_doublebyte(temp_degrees_f);
    lcd_write_char(DEGREE_SYMBOL);
    lcd_write_string("F");
}

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
void uart_string_printf(const char *str)
{
    while(*str != '\0') 
    {
        UART_out_char(*str++);
    }
}