//*****************************************************************************
//*****************************    C Source Code    ***************************
//*****************************************************************************
//  DESIGNER NAME:  Mason Ferrara
//
//       LAB NAME:  Lab 11 Part 1
//
//      FILE NAME:  lab11_main.c
//
//-----------------------------------------------------------------------------
//*****************************************************************************
// DESCRIPTION:
//      This lab introduces students to the Serial Peripheral Interface (SPI) 
// — a synchronous, full-duplex communication protocol widely used in 
// embedded systems.
//
//      Unlike asynchronous communication where timing mismatches can lead
// to data errors, SPI uses a dedicated clock line to synchronize data
// transfer between devices. This ensures consistent timing and reliable
// communication, making SPI ideal for high-speed data exchange and control.
//
//      In this lab, students will use the MSPM0G3507 microcontroller and
// LaunchPad development boards to configure and utilize the SPI module
// to transmit and receive data. Through hands-on implementation, students
// will communicate with simulated or real SPI peripherals such as sensors,
// shift registers, or memory devices. The lab demonstrates how microcontrollers
// use SPI to maintain precise timing relationships and interface with a wide
// range of digital components in real-world embedded systems.
//
//*****************************************************************************

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
#include "spi.h"
#include "ti/devices/msp/m0p/mspm0g350x.h"
#include "ti/devices/msp/peripherals/hw_gpio.h"
#include "uart.h"
#include <stdio.h>
#include <ti/devices/msp/msp.h>

//-----------------------------------------------------------------------------
// Define function prototypes used by the program
//-----------------------------------------------------------------------------

void run_lab11_part1(void);
void uart_string_printf(const char *str);
uint8_t get_SPI_data(void);
void update_led(void);
void display_menu(void);
uint16_t string_to_uint16(const char *string);

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
#define DELETE (127)
#define ALL_LEDS (0xFF)
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
  spi1_init();
  IOMUX->SECCFG.PINCM[LP_SPI_CS0_IOMUX] =
      (IOMUX_PINCM_PC_CONNECTED | IOMUX_PINCM23_PF_GPIOB_DIO06);
  GPIOB->DOE31_0 |= GPIO_DOE31_0_DIO6_ENABLE;
  GPIOB->DOUT31_0 &= ~GPIO_DOUT31_0_DIO6_MASK;

  lcd_clear();

  run_lab11_part1();

  // Endless loop to prevent program from ending
  while (1)
    ;

} /* main */

//-----------------------------------------------------------------------------
// Description:
// This function implements the main control loop for Lab 11, Part 1. It 
// displays an interactive menu via UART, allowing the user to perform 
// operations such as setting SPI transmit data, sending and receiving SPI 
// data, updating LEDs, or exiting the program.
//
// The current operation status is displayed on the LCD, while the user's 
// input and program feedback are handled through the UART terminal. SPI data 
// transmission and reception are also echoed on the LCD to provide real-time 
// visual feedback.
//
// The loop continues running until the user selects the option to end the 
// program.
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
void run_lab11_part1() {
  bool done = false;
  char menu_option = 0;
  uint8_t xmit_data = 0;
  uint8_t rcv_data = 0;

  lcd_set_ddram_addr(LCD_LINE1_ADDR);
  lcd_write_string("Program Running");
  lcd_set_ddram_addr(LCD_LINE2_ADDR);
  lcd_write_string("See Serial Port");

  while (!done) {
    display_menu();

    menu_option = UART_in_char();
    UART_out_char(menu_option);

    switch (menu_option) {
    case '1':
      xmit_data = get_SPI_data();
      break;

    case '2':
      uart_string_printf("\r\nData sent via SPI.\r\n");
      spi1_write_data(xmit_data);
      rcv_data = spi1_read_data();
      lcd_clear();
      lcd_set_ddram_addr(LCD_LINE1_ADDR);
      lcd_write_string("SEND: ");
      lcd_write_byte(xmit_data);
      lcd_set_ddram_addr(LCD_LINE2_ADDR);
      lcd_write_string("RECV: ");
      lcd_write_byte(rcv_data);
      break;

    case '3':
      update_led();
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
// This function prompts the user via UART to enter a valid decimal number 
// between 0 and 255, then converts the input string to an 8-bit unsigned 
// integer (`uint8_t`). The function handles basic input editing, including 
// backspace support, and echoes characters to the terminal for user feedback.
//
// The function continuously prompts the user until a valid input within the 
// specified range is received. Once valid, it returns the parsed value, which 
// can be used for SPI data transmission or other application logic.
//
// INPUT PARAMETERS:
//  none
//
// OUTPUT PARAMETERS:
//  none
//
// RETURN:
//  The validated 8-bit unsigned integer value entered by the user
//-----------------------------------------------------------------------------
uint8_t get_SPI_data(void) {
  char buffer[MAX_CHAR_SIZE] = {0};
  uint8_t index = 0;
  uint16_t value = 0;

  while (1) {
    uart_string_printf("\r\nSet SPI xmit menu selected\r\n");
    uart_string_printf("Enter a valid decimal number (0 to 255): ");
    index = 0;
    memset(buffer, 0, MAX_CHAR_SIZE);

    while (1) {
      char input = UART_in_char();

      if (input == '\r') {
        UART_out_char('\r');
        UART_out_char('\n');
        break;
      }

      if ((input == BACKSPACE || input == DELETE) && index > 0) {
        index--;
        UART_out_char('\b');
        UART_out_char(' ');
        UART_out_char('\b');
        continue;
      }

      if (input >= '0' && input <= '9' && index < MAX_CHAR_SIZE - 1) {
        buffer[index++] = input;
        UART_out_char(input);
      }
    }

    value = string_to_uint16(buffer);
    if (value <= 255) {
      return (uint8_t)value;
    } else {
      uart_string_printf("Invalid input. Try again.\r\n");
    }
  }
}

//-----------------------------------------------------------------------------
// Description:
// This function prints a formatted menu of user-selectable options to the 
// UART terminal. The menu provides numbered choices for setting data, 
// sending data, updating LEDs, or ending the program. It then prompts the 
// user to enter a selection.
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
void display_menu() {
  uart_string_printf("\r\n MENU OPTIONS: \r\n");
  uart_string_printf("1. Set Data to Send\n");
  uart_string_printf("2. Send Data\n");
  uart_string_printf("3. Update LEDs\n");
  uart_string_printf("4. End Program\n");
  uart_string_printf("\r\n Enter your selection: ");
}

//-----------------------------------------------------------------------------
// Description:
// This function performs a brief SPI chip-select pulse to trigger an update 
// of external LEDs. It sets the chip-select line (`LP_SPI_CS0_MASK`) high 
// for 10 milliseconds, then pulls it low. This pulse is used to latch or 
// refresh the LED state on the connected device.
//
// Additionally, it sends a message over UART to indicate that the LED update 
// sequence has been executed. This can be useful for debugging or confirming 
// system behavior during runtime.
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
void update_led() {
  uart_string_printf("\r\nLEDs updated.\r\n");
  GPIOB->DOUT31_0 |= LP_SPI_CS0_MASK;
  msec_delay(10);
  GPIOB->DOUT31_0 &= ~LP_SPI_CS0_MASK;
}

//-----------------------------------------------------------------------------
// Description:
// This function converts a null-terminated ASCII string representing a 
// decimal number into its corresponding 16-bit unsigned integer value 
// (`uint16_t`). Each character in the string must be a digit between 
// '0' and '9'. The function multiplies the current result by 10 and adds 
// the numeric value of each digit sequentially to build the final integer.
//
// This utility is useful in embedded systems where numeric input is received 
// as a string (e.g., from UART or a user interface) and needs to be converted 
// for arithmetic or control logic.
//
// INPUT PARAMETERS:
//  string – pointer to the null-terminated string containing only numeric 
//           characters ('0' to '9')
//
// OUTPUT PARAMETERS:
//  none
//
// RETURN:
//  The 16-bit unsigned integer result corresponding to the numeric string
//
//-----------------------------------------------------------------------------
uint16_t string_to_uint16(const char *string) {
  uint16_t result = 0;

  while (*string != '\0') {
    result = result * 10 + (*string - '0');
    string++;
  }

  return result;
}

//-----------------------------------------------------------------------------
// Description:
// This function sends a null-terminated string over the UART interface one
// character at a time using `UART_out_char()`. It continues transmitting until
// the null character
// (`'\0'`) is encountered, signaling the end of the string.
//
// This utility function is commonly used for displaying messages on a serial
// terminal during debugging or user interaction in embedded systems.
//
// INPUT PARAMETERS:
//  str – pointer to the null-terminated string to be transmitted
//
// OUTPUT PARAMETERS:
//  none
//
// RETURN:
//  none
//-----------------------------------------------------------------------------
void uart_string_printf(const char *str) {
  while (*str != '\0') {
    UART_out_char(*str++);
  }
}