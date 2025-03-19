//*****************************************************************************
//*****************************    C Source Code    ***************************
//*****************************************************************************
//  DESIGNER NAME:  Mason Ferrara
//
//       LAB NAME:  Lab 7 part 4
//
//      FILE NAME:  lab7p4_main.c
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
#include "ti/devices/msp/m0p/mspm0g350x.h"
#include "ti/devices/msp/peripherals/hw_gpio.h"
#include "ti/devices/msp/peripherals/m0p/hw_cpuss.h"


//-----------------------------------------------------------------------------
// Define function prototypes used by the program
//-----------------------------------------------------------------------------

void config_pb1_interrupts(void);
void config_pb2_interrupts(void);

void run_lab7_part4(void);

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

//-----------------------------------------------------------------------------
// Define global variables and structures here.
// NOTE: when possible avoid using global variables
//-----------------------------------------------------------------------------
  bool g_PB1_pressed = false;
  bool g_PB2_pressed = false;

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

 

  config_pb1_interrupts();
  config_pb2_interrupts();
  run_lab7_part4();


 // Endless loop to prevent program from ending
 while (1);

} /* main */
//-------------------------------------------------------------------------------------
void wait_for_pb_pressed(uint8_t pb_idx){
    while (is_pb_up(pb_idx));
    msec_delay(TEN_MILLISEC_PAUSE);
}


//-------------------------------------------------------------------------------------
void run_lab7_part4(){
    bool done = false;
    uint8_t count = MAX_COUNT_VALUE;
    bool display = false;

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
            led_enable();
            leds_on(count);
            lcd_set_ddram_addr(LCD_LINE2_ADDR);
        }
        if (g_PB2_pressed) {
            g_PB2_pressed = false;
            lcd_set_ddram_addr(LCD_LINE2_ADDR);
            if(display)
            {
                lcd_write_string("                  ");
                display = false;
            }
            else{

                lcd_write_string("PB2 PRESSED");
                display = true;
            }
        }
        if (g_PB1_pressed){
            g_PB1_pressed = false;
            done = true;
        }
    }
    lcd_clear();
        lcd_set_ddram_addr(LCD_LINE2_ADDR);
        lcd_write_string("Program Stopped");
        msec_delay(Two_Hundred_Millisec_Pause);
}

//-------------------------------------------------------------------------------------
void config_pb1_interrupts()
{
    GPIOB-> POLARITY31_16 = GPIO_POLARITY31_16_DIO18_RISE;

    GPIOB->CPU_INT.ICLR = GPIO_CPU_INT_ICLR_DIO18_CLR;

    GPIOB->CPU_INT.IMASK = GPIO_CPU_INT_IMASK_DIO18_SET;

    NVIC_SetPriority(GPIOB_INT_IRQn, 2);
    NVIC_EnableIRQ(GPIOB_INT_IRQn);
}

//-------------------------------------------------------------------------------------
void config_pb2_interrupts()
{
    GPIOA-> POLARITY15_0 = GPIO_POLARITY15_0_DIO15_RISE;

    GPIOA->CPU_INT.ICLR = GPIO_CPU_INT_ICLR_DIO15_CLR;

    GPIOA->CPU_INT.IMASK = GPIO_CPU_INT_IMASK_DIO15_SET;

    NVIC_SetPriority(GPIOA_INT_IRQn, 2);
    NVIC_EnableIRQ(GPIOA_INT_IRQn);
}

//-------------------------------------------------------------------------------------
void GROUP1_IRQHandler(void)
{
    uint32_t group_iidx_status;
    uint32_t gpio_mis;

    do 
    {
        group_iidx_status = CPUSS->INT_GROUP[1].IIDX;
        
        switch (group_iidx_status)
          {
          case (CPUSS_INT_GROUP_IIDX_STAT_INT1):
            gpio_mis = GPIOB->CPU_INT.MIS;
            if((gpio_mis & GPIO_CPU_INT_MIS_DIO18_MASK) == GPIO_CPU_INT_MIS_DIO18_SET)
            {
                g_PB1_pressed = true;

                GPIOB->CPU_INT.ICLR = GPIO_CPU_INT_ICLR_DIO18_CLR;
            }

            break;
          case (CPUSS_INT_GROUP_IIDX_STAT_INT0):
            gpio_mis = GPIOA->CPU_INT.MIS;
            if((gpio_mis & GPIO_CPU_INT_MIS_DIO15_MASK) == GPIO_CPU_INT_MIS_DIO15_SET)
            {
                g_PB2_pressed = true;

                GPIOA->CPU_INT.ICLR = GPIO_CPU_INT_ICLR_DIO15_CLR;
            }

            break;

          default:
            break;
        }
    } while (group_iidx_status != 0);
}