//*****************************************************************************
//*****************************    C Source Code    ***************************
//*****************************************************************************
//  DESIGNER NAME:  Mason Ferrara
//
//       LAB NAME:  Lab 9 part 1
//
//      FILE NAME:  lab9p1_main.c
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

void run_lab9_part1(void);

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
#define PERCENT_SYMBOOL (0x25)

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
  led_init();
  led_enable();
  I2C_init();
  lcd1602_init();
  dipsw_init();
  lcd_clear();
  keypad_init();

  ADC0_init(ADC12_MEMCTL_VRSEL_INTREF_VSSA);

  motor0_init();
  motor0_pwm_init(4000, 0);
  motor0_pwm_enable();

  config_pb1_interrupts();
  config_pb2_interrupts();

  run_lab9_part1();

  NVIC_DisableIRQ(GPIOA_INT_IRQn);
  NVIC_DisableIRQ(GPIOB_INT_IRQn);
  // Endless loop to prevent program from ending
  while (1)
    ;

} /* main */

//-----------------------------------------------------------------------------
// Description:
// This function executes Part 4 of Lab 7, implementing a countdown timer that
// starts at MAX_COUNT_VALUE and decrements every 0.2 seconds. When the timer
// reaches MIN_COUNT_VALUE, it resets to MAX_COUNT_VALUE, updates the LCD
// display, and activates the LEDs based on the count value.
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
void run_lab9_part1() {
  bool done = false;

  typedef enum { MOTOR_OFF1, MOTOR_CW, MOTOR_OFF2, MOTOR_CCW } fsm_states_t;

  fsm_states_t state = MOTOR_OFF1;
  led_off(LED_BAR_LD1_IDX);
  led_off(LED_BAR_LD2_IDX);
  lcd_clear();

  while (!done) {
    uint8_t key_pressed = keypad_scan();
    if (key_pressed != NO_KEY_PRESSED) {
      uint16_t duty_cycle = (key_pressed * 100) / 15;
      lcd_set_ddram_addr(LCD_LINE1_ADDR + 3);
      lcd_write_string("MOTOR SPEED");
      lcd_set_ddram_addr(LCD_LINE2_ADDR + 4);
      lcd_write_doublebyte(duty_cycle);
      lcd_set_ddram_addr(LCD_LINE2_ADDR + 9);
      lcd_write_char(PERCENT_SYMBOOL);
      motor0_set_pwm_dc(duty_cycle);
      wait_no_key_pressed();
      msec_delay(TEN_MILLISEC_PAUSE);
    }

    if (g_PB2_pressed) {
      switch (state) {
      case MOTOR_OFF1:
        state = MOTOR_CW;
        led_on(LED_BAR_LD1_IDX);
        led_off(LED_BAR_LD2_IDX);
        break;

      case MOTOR_CW:
        state = MOTOR_OFF2;
        led_off(LED_BAR_LD1_IDX);
        led_off(LED_BAR_LD2_IDX);
        break;

      case MOTOR_OFF2:
        state = MOTOR_CCW;
        led_off(LED_BAR_LD1_IDX);
        led_on(LED_BAR_LD2_IDX);
        break;

      case MOTOR_CCW:
        state = MOTOR_OFF1;
        led_off(LED_BAR_LD1_IDX);
        led_off(LED_BAR_LD2_IDX);
        break;
      }
      msec_delay(One_Hundred_Millisec_Pause);
      g_PB2_pressed = false;
    }

    if (g_PB1_pressed) {
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
// level of 2. The function ensures that the system responds appropriately
// when PB1 is pressed.
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
// clears any pending interrupt flags for DIO15, enables the interrupt mask
// for DIO15, and sets the priority of the interrupt to level 2 in the Nested
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
// by GPIOB pin DIO18 (INT1), it sets the global flag g_PB1_pressed to true
// and clears the interrupt flag. If an interrupt is triggered by GPIOA pin
// DIO15 (INT0), it sets the global flag g_PB2_pressed to true and clears the
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