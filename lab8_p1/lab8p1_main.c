//*****************************************************************************
//*****************************    C Source Code    ***************************
//*****************************************************************************
//  DESIGNER NAME:  Mason Ferrara
//
//       LAB NAME:  Lab 8 Part 1
//
//      FILE NAME:  lab8p1_main.c
//
//-----------------------------------------------------------------------------
//
// DESCRIPTION:
//      This lab introduces students to the fundamentals of Analog-to-Digital
// Conversion (ADC) and its role in bridging  he analog world with digital
// microcontroller systems. Digital computers operate using discrete signals (0s
// and 1s), while real-world data—such as temperature, light intensity, and
// voltage—varies continuously in analog form.  ADCs enable microcontrollers to
// process and analyze these real-world signals by converting them into digital
// values.
//
//      Using the MSPM0G3507 microcontroller and the LaunchPad development
//      board,
// students will configure and utilize the integrated ADC to read inputs from
// various sensors. These include a photosensor for measuring light intensity,
// a temperature sensor for detecting ambient conditions, and a potentiometer
// for manual voltage adjustments.
//
//      Through this lab, students will gain hands-on experience in configuring
// the ADC, understanding signal sampling  and quantization, and interpreting
// digital representations of analog sensor data. By completing this lab,
// students  will develop a deeper understanding of how microcontrollers
// interface with the real world, preparing them for applications in embedded
// systems, data acquisition, and sensor-based designs.
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
#include "lcd1602.h"
#include <stdio.h>
#include <ti/devices/msp/msp.h>

//-----------------------------------------------------------------------------
// Define function prototypes used by the program
//-----------------------------------------------------------------------------
void config_pb1_interrupts(void);
void OPA0_init(void);

void run_lab8_part1(void);

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
#define LIGHT_THRESHOLD (0x64)
//-----------------------------------------------------------------------------
// Define global variables and structures here.
// NOTE: when possible avoid using global variables
//-----------------------------------------------------------------------------
bool g_PB1_pressed = false;

// Define a structure to hold different data types

int main(void) {
  clock_init_40mhz();
  launchpad_gpio_init();
  I2C_init();
  lcd1602_init();
  dipsw_init();
  led_init();
  led_disable();
  lcd_clear();

  OPA0_init();
  ADC0_init(ADC12_MEMCTL_VRSEL_INTREF_VSSA);

  config_pb1_interrupts();
  run_lab8_part1();

  NVIC_DisableIRQ(GPIOA_INT_IRQn);
  NVIC_DisableIRQ(GPIOB_INT_IRQn);
  // Endless loop to prevent program from ending
  while (1)
    ;

} /* main */

//-----------------------------------------------------------------------------
// DESCRIPTION:
//    This function initializes the operational amplifier (OPA0) on the
//    MSPM0G3507 microcontroller. The configuration involves resetting the
//    module, enabling power, setting the gain bandwidth, and configuring
//    input and output pins. The OPA is then powered up and enabled for use
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
void OPA0_init(void) {
  OPA0->GPRCM.RSTCTL = (OA_RSTCTL_KEY_UNLOCK_W | OA_RSTCTL_RESETSTKYCLR_CLR |
                        OA_RSTCTL_RESETASSERT_ASSERT);

  OPA0->GPRCM.PWREN = (OA_PWREN_KEY_UNLOCK_W | OA_PWREN_ENABLE_ENABLE);

  // time for OPA to power up
  clock_delay(24);

  OPA0->CFGBASE &= ~(OA_CFGBASE_RRI_MASK);

  OPA0->CFGBASE |= ((uint32_t)OA_CFGBASE_GBW_HIGHGAIN);

  OPA0->CFG |= (OA_CFG_GAIN_MINIMUM | OA_CFG_MSEL_NC | OA_CFG_NSEL_EXTPIN0 |
                OA_CFG_PSEL_EXTPIN0 | OA_CFG_OUTPIN_ENABLED | OA_CFG_CHOP_OFF);

  // Enable the OPA
  OPA0->CTL |= OA_CTL_ENABLE_ON;

} /* OPA0_init */

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
void config_pb1_interrupts() {
  GPIOB->POLARITY31_16 = GPIO_POLARITY31_16_DIO18_RISE;

  GPIOB->CPU_INT.ICLR = GPIO_CPU_INT_ICLR_DIO18_CLR;

  GPIOB->CPU_INT.IMASK = GPIO_CPU_INT_IMASK_DIO18_SET;

  NVIC_SetPriority(GPIOB_INT_IRQn, 2);
  NVIC_EnableIRQ(GPIOB_INT_IRQn);
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
    }
  } while (group_iidx_status != 0);
}

//-----------------------------------------------------------------------------
// Description:
// This function executes Part 1 of Lab 8, implementing a light detection system
// that continuously monitors the ambient light level using an ADC sensor.
// The LCD displays whether the environment is "Light" or "Dark" based on the
// measured ADC value, and updates accordingly.
//
// The function operates as follows:
// - It initializes the LCD display and continuously reads the ADC value from
//   the light sensor (CHANNEL_7).
// - If the light level exceeds the LIGHT_THRESHOLD, the LCD displays "Light"
//   along with the ADC reading.
// - If the light level falls below the LIGHT_THRESHOLD, the LCD displays
//   "Dark" along with the ADC reading.
// - The user can terminate the function by pressing PB1, which clears the LCD
//   and displays "Program Stopped" before exiting.
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
void run_lab8_part1() {
  bool done = false;
  bool display = false;
  char message1[] = "Status:";
  char message2[] = "ADC =";
  char message3[] = "Light";
  char message4[] = "Dark";

  lcd_clear();

  while (!done) {

    lcd_set_ddram_addr(LCD_LINE1_ADDR);
    lcd_write_string(message1);
    lcd_set_ddram_addr(LCD_LINE2_ADDR);
    lcd_write_string(message2);
    msec_delay(Two_Hundred_Millisec_Pause);

    uint32_t light_level = ADC0_in(CHANNEL_7);

    if (light_level > LIGHT_THRESHOLD) // light
    {
      lcd_clear();
      lcd_set_ddram_addr(LCD_LINE1_ADDR);
      lcd_write_string(message1);
      lcd_set_ddram_addr(LCD_LINE1_ADDR + 9);
      lcd_write_string(message3);
      lcd_set_ddram_addr(LCD_LINE2_ADDR);
      lcd_write_string(message2);
      lcd_set_ddram_addr(LCD_LINE2_ADDR + 10);
      lcd_write_doublebyte(light_level);
    }
    if (light_level <= LIGHT_THRESHOLD) // dark
    {
      lcd_clear();
      lcd_set_ddram_addr(LCD_LINE1_ADDR);
      lcd_write_string(message1);
      lcd_set_ddram_addr(LCD_LINE1_ADDR + 9);
      lcd_write_string(message4);
      lcd_set_ddram_addr(LCD_LINE2_ADDR);
      lcd_write_string(message2);
      lcd_set_ddram_addr(LCD_LINE2_ADDR + 10);
      lcd_write_byte(light_level);
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