//*****************************************************************************
//*****************************    C Source Code    ***************************
//*****************************************************************************
//  DESIGNER NAME:  Mason Ferrara
//
//       LAB NAME:  Lab 8 Part 3
//
//      FILE NAME:  lab8p3_main.c
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
#include "ti/devices/msp/peripherals/hw_adc12.h"
#include "adc.h"



//-----------------------------------------------------------------------------
// Define function prototypes used by the program
//-----------------------------------------------------------------------------
void config_pb1_interrupts(void);
void config_pb2_interrupts(void);
void wait_for_pb_pressed(uint8_t pb_idx);
void wait_for_pb_released(uint8_t pb_idx);

void run_lab8_part3(void);

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
#define CHANNEL_7 (0x7)
#define CHANNEL_5 (0x5)
#define LED_DIVISOR (455)

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
  I2C_init();
  lcd1602_init();
  dipsw_init();
  led_init();
  led_disable();
  lcd_clear();

  ADC0_init(ADC12_MEMCTL_VRSEL_INTREF_VSSA);
  

  config_pb1_interrupts();
  config_pb2_interrupts();
  run_lab8_part3();
 
  NVIC_DisableIRQ(GPIOA_INT_IRQn);
  NVIC_DisableIRQ(GPIOB_INT_IRQn);
 // Endless loop to prevent program from ending
 while (1);

} /* main */



//----------------------------------------------------------------------------- 
// Description:
// This function waits for a user to press the push button specified 
// by the given index. It includes debounce delays to ensure stable input
// detection.
//
// INPUT PARAMETERS:
//  pb_idx: The index of the push button to be monitored.
//
// OUTPUT PARAMETERS:
//  none
//
// RETURN:
//  none
//-----------------------------------------------------------------------------
void wait_for_pb_pressed(uint8_t pb_idx){
    while (is_pb_up(pb_idx));
    msec_delay(TEN_MILLISEC_PAUSE);
}

//----------------------------------------------------------------------------- 
// Description:
// This function waits for a user to release the push button specified 
// by the given index. It includes debounce delays to ensure stable input
// detection.
//
// INPUT PARAMETERS:
//  pb_idx: The index of the push button to be monitored.
//
// OUTPUT PARAMETERS:
//  none
//
// RETURN:
//  none
//-----------------------------------------------------------------------------
void wait_for_pb_released(uint8_t pb_idx){
    while (is_pb_down(pb_idx));
    msec_delay(TEN_MILLISEC_PAUSE);
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

//-----------------------------------------------------------------------------
void run_lab8_part3()
{
    bool done = false;
    char message2[] = "ADC =";
    char message5[] = "Temp =";
    char message6[] = "F";

    lcd_clear();
    led_enable();

    while (!done) {

        lcd_set_ddram_addr(LCD_LINE1_ADDR);
        lcd_write_string(message2);
        msec_delay(Two_Hundred_Millisec_Pause);

        uint16_t voltage_level = ADC0_in(CHANNEL_7);
        uint32_t led_index;
        uint32_t led_max = voltage_level / LED_DIVISOR;

        uint16_t temp_raw_adc = ADC0_in(CHANNEL_5);
        uint16_t temp_degrees_c = (uint16_t) thermistor_calc_temperature(temp_raw_adc);
        uint16_t temp_degrees_f = ((temp_degrees_c * 9)/5) + 32;
        
        if(!g_PB1_pressed)
        {
          leds_off();
          for (led_index = 0; led_index < led_max; led_index++)
            {
              led_on(led_index); 
            }
          lcd_set_ddram_addr(LCD_LINE1_ADDR);
          lcd_write_string(message2);
          lcd_set_ddram_addr(LCD_LINE1_ADDR + 9);
          lcd_write_doublebyte(voltage_level);

        }
        if(g_PB2_pressed){
            
            lcd_set_ddram_addr(LCD_LINE2_ADDR);
            lcd_write_string(message5);
            lcd_set_ddram_addr(LCD_LINE2_ADDR + 7);
            lcd_write_doublebyte(temp_degrees_f);
            lcd_set_ddram_addr(LCD_LINE2_ADDR + 13);
            lcd_write_string(message6);
            
            g_PB2_pressed = false;
        }
        
        if (g_PB1_pressed){
            g_PB1_pressed = false;
            done = true;
        }
        
    }
    lcd_clear();
    leds_off();
    lcd_set_ddram_addr(LCD_LINE2_ADDR);
    lcd_write_string("Program Stopped");
    msec_delay(Two_Hundred_Millisec_Pause);
}