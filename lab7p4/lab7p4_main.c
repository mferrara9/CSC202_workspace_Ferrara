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


//-----------------------------------------------------------------------------   
// Description:
// This function executes Part 4 of Lab 7, implementing a countdown timer that 
// starts at MAX_COUNT_VALUE and decrements every 0.2 seconds. When the timer 
// reaches MIN_COUNT_VALUE, it resets to MAX_COUNT_VALUE, updates the LCD display, 
// and activates the LEDs based on the count value.
//
// The user can interact with the countdown using pushbuttons:
// - If PB2 is pressed, the LCD alternates between displaying "PB2 PRESSED" and 
//   clearing the message.
// - If PB1 is pressed, the function exits the loop, clears the display, and 
//   shows "Program Stopped" on the LCD before ending execution.
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

//-----------------------------------------------------------------------------   
// Description:
// This function configures the interrupt for pushbutton PB1. The interrupt is 
// triggered on a rising edge signal from digital input DIO18. Once triggered, 
// the interrupt flag is cleared, and the interrupt is enabled with a priority 
// level of 2. The function ensures that the system responds appropriately when 
// PB1 is pressed.
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
void config_pb1_interrupts()
{
    GPIOB-> POLARITY31_16 = GPIO_POLARITY31_16_DIO18_RISE;

    GPIOB->CPU_INT.ICLR = GPIO_CPU_INT_ICLR_DIO18_CLR;

    GPIOB->CPU_INT.IMASK = GPIO_CPU_INT_IMASK_DIO18_SET;

    NVIC_SetPriority(GPIOB_INT_IRQn, 2);
    NVIC_EnableIRQ(GPIOB_INT_IRQn);
}

//-----------------------------------------------------------------------------   
// Description:
// This function configures interrupts for pushbutton PB2 using GPIOA. The interrupt 
// is triggered on a rising edge detected on DIO15. The function clears any 
// pending interrupt flags for DIO15, enables the interrupt mask for DIO15, and 
// sets the priority of the interrupt to level 2 in the Nested Vectored Interrupt 
// Controller (NVIC). Finally, the function enables the interrupt request in the NVIC.
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
void config_pb2_interrupts()
{
    GPIOA-> POLARITY15_0 = GPIO_POLARITY15_0_DIO15_RISE;

    GPIOA->CPU_INT.ICLR = GPIO_CPU_INT_ICLR_DIO15_CLR;

    GPIOA->CPU_INT.IMASK = GPIO_CPU_INT_IMASK_DIO15_SET;

    NVIC_SetPriority(GPIOA_INT_IRQn, 2);
    NVIC_EnableIRQ(GPIOA_INT_IRQn);
}

//-----------------------------------------------------------------------------   
// Description:
// This function serves as the interrupt handler for Group 1 interrupts. It 
// continuously checks the interrupt status of the CPUSS INT GROUP 1 and processes 
// the corresponding interrupt sources. If an interrupt is triggered by GPIOB pin 
// DIO18 (INT1), it sets the global flag g_PB1_pressed to true and clears the 
// interrupt flag. If an interrupt is triggered by GPIOA pin DIO15 (INT0), it sets 
// the global flag g_PB2_pressed to true and clears the interrupt flag.
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