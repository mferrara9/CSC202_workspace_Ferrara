//*****************************************************************************
//*****************************    C Source Code    ***************************
//*****************************************************************************
//  DESIGNER NAME:  Mason Ferrara
//
//       LAB NAME:  Lab 9 Part 3
//
//      FILE NAME:  lab9p2_main.c
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

#include "LaunchPad.h"
#include "adc.h"
#include "clock.h"
#include "cmsis_gcc.h"
#include "lcd1602.h"
#include <stdio.h>
#include <ti/devices/msp/msp.h>

//-----------------------------------------------------------------------------
// Define function prototypes used by the program
//-----------------------------------------------------------------------------
void config_pb1_interrupts(void);
void config_pb2_interrupts(void);

void run_lab9_part3(void);

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
#define CHANNEL_5 (0x5)
#define CHANNEL_7 (0x7)
#define MIN_COUNT_VALUE (100)
#define MAX_COUNT_VALUE (500)
#define MAX_ACD_VALUE (1023)
//-----------------------------------------------------------------------------
// Define global variables and structures here.
// NOTE: when possible avoid using global variables
//-----------------------------------------------------------------------------
bool g_PB1_pressed = false;
bool g_PB2_pressed = false;


// Define a structure to hold different data types

int main(void) {
  clock_init_40mhz();
  launchpad_gpio_init();
  I2C_init();
  lcd1602_init();
  dipsw_init();
  led_init();
  led_enable();
  lcd_clear();

  ADC0_init(ADC12_MEMCTL_VRSEL_INTREF_VSSA);

  motor0_init();
  motor0_pwm_init(4000, 0);
  motor0_pwm_enable();

  config_pb1_interrupts();
  config_pb2_interrupts();
  run_lab9_part3();

  NVIC_DisableIRQ(GPIOA_INT_IRQn);
  NVIC_DisableIRQ(GPIOB_INT_IRQn);
  // Endless loop to prevent program from ending
  while (1)
    ;

} /* main */

void run_lab9_part3() {
  bool done = false;
  char message2[] = "ADC VALUE =";
  char message5[] = "SERVO CNT =";
  uint32_t servo_count;

  while (!done) {

    lcd_set_ddram_addr(LCD_LINE1_ADDR);
    lcd_write_string(message2);
    lcd_set_ddram_addr(LCD_LINE2_ADDR);
    lcd_write_string(message5);
    msec_delay(Two_Hundred_Millisec_Pause);


    while (!g_PB1_pressed) {

      __ASM ("CPSID I");
      uint16_t adc_pot_value = ADC0_in(CHANNEL_7);
      __ASM ("CPSIE I");

      adc_pot_value >>= 2;

      servo_count = 
      ((adc_pot_value * (MAX_COUNT_VALUE - MIN_COUNT_VALUE)) / MAX_ACD_VALUE) + MIN_COUNT_VALUE;

      lcd_set_ddram_addr(LCD_LINE1_ADDR + 9);
      lcd_write_doublebyte(adc_pot_value);
      lcd_set_ddram_addr(LCD_LINE2_ADDR + 9);
      lcd_write_doublebyte(servo_count);
      motor0_set_pwm_count(servo_count);

      msec_delay(Two_Hundred_Fifty_Millisec_Pause);
    }
    if (g_PB1_pressed) {
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
//-----------------------------------------------------------------------------
// Description:
// This function configures interrupts for pushbutton PB1 using GPIOB. The
// interrupt is triggered on a rising edge detected on DIO15. The function
// clears any pending interrupt flags for DIO18, enables the interrupt mask for
// DIO15, and sets the priority of the interrupt to level 2 in the Nested
// Vectored Interrupt Controller (NVIC). Finally, the function enables the
// interrupt request in the NVIC.
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
void config_pb1_interrupts() {
  GPIOB->POLARITY31_16 = GPIO_POLARITY31_16_DIO18_RISE;

  GPIOB->CPU_INT.ICLR = GPIO_CPU_INT_ICLR_DIO18_CLR;

  GPIOB->CPU_INT.IMASK = GPIO_CPU_INT_IMASK_DIO18_SET;

  NVIC_SetPriority(GPIOB_INT_IRQn, 2);
  NVIC_EnableIRQ(GPIOB_INT_IRQn);
}

//-----------------------------------------------------------------------------
// Description:
// This function configures interrupts for pushbutton PB2 using GPIOA. The
// interrupt is triggered on a rising edge detected on DIO15. The function
// clears any pending interrupt flags for DIO15, enables the interrupt mask for
// DIO15, and sets the priority of the interrupt to level 2 in the Nested
// Vectored Interrupt Controller (NVIC). Finally, the function enables the
// interrupt request in the NVIC.
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
void config_pb2_interrupts() {
  GPIOA->POLARITY15_0 = GPIO_POLARITY15_0_DIO15_RISE;

  GPIOA->CPU_INT.ICLR = GPIO_CPU_INT_ICLR_DIO15_CLR;

  GPIOA->CPU_INT.IMASK = GPIO_CPU_INT_IMASK_DIO15_SET;

  NVIC_SetPriority(GPIOA_INT_IRQn, 2);
  NVIC_EnableIRQ(GPIOA_INT_IRQn);
}

//-----------------------------------------------------------------------------
// Description:
// This function serves as the interrupt handler for Group 1 interrupts. It
// continuously checks the interrupt status of the CPUSS INT GROUP 1 and
// processes the corresponding interrupt sources. If an interrupt is triggered
// by GPIOB pin DIO18 (INT1), it sets the global flag g_PB1_pressed to true and
// clears the interrupt flag. If an interrupt is triggered by GPIOA pin DIO15
// (INT0), it sets the global flag g_PB2_pressed to true and clears the
// interrupt flag.
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
void GROUP1_IRQHandler(void) {
  uint32_t group_iidx_status;
  uint32_t gpio_mis;

  do {
    group_iidx_status = CPUSS->INT_GROUP[1].IIDX;

    switch (group_iidx_status) {
    case (CPUSS_INT_GROUP_IIDX_STAT_INT1):
      gpio_mis = GPIOB->CPU_INT.MIS;
      if ((gpio_mis & GPIO_CPU_INT_MIS_DIO18_MASK) ==
          GPIO_CPU_INT_MIS_DIO18_SET) {
        g_PB1_pressed = true;

        GPIOB->CPU_INT.ICLR = GPIO_CPU_INT_ICLR_DIO18_CLR;
      }

      break;
    case (CPUSS_INT_GROUP_IIDX_STAT_INT0):
      gpio_mis = GPIOA->CPU_INT.MIS;
      if ((gpio_mis & GPIO_CPU_INT_MIS_DIO15_MASK) ==
          GPIO_CPU_INT_MIS_DIO15_SET) {
        g_PB2_pressed = true;

        GPIOA->CPU_INT.ICLR = GPIO_CPU_INT_ICLR_DIO15_CLR;
      }

      break;

    default:
      break;
    }
  } while (group_iidx_status != 0);
}
