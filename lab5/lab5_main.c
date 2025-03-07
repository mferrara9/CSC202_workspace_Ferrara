//*****************************************************************************
//*****************************    C Source Code    ***************************
//*****************************************************************************
//  DESIGNER NAME:  Mason Ferrara
//
//       LAB NAME:  Lab 5
//
//      FILE NAME:  lab5_main.c
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
#include "clock.h"
#include <stdio.h>
#include <ti/devices/msp/msp.h>

//-----------------------------------------------------------------------------
// Define function prototypes used by the program
//-----------------------------------------------------------------------------
void run_lab5_part1(void);
void run_lab5_part2(void);
void run_lab5_part3(void);
void run_lab5_part4(void);
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
#define Index_zero (0)
#define Zero_LED (0)
#define Counter_Zero (0x0)
#define Counter_Max (0xFF)
#define Up_One (0x1)
#define DIG0 (0)
#define DIG1 (1)
#define DIG2 (2)
#define DIG3 (3)
//-----------------------------------------------------------------------------
// Define global variables and structures here.
// NOTE: when possible avoid using global variables
//-----------------------------------------------------------------------------

// Define a structure to hold different data types

int main(void) {
  clock_init_40mhz();
  launchpad_gpio_init();
  seg7_init();
  led_init();
  dipsw_init();
  lpsw_init();
  keypad_init();

   run_lab5_part1();
   run_lab5_part2();
   run_lab5_part3();
   run_lab5_part4();

  // Endless loop to prevent program from ending
  while (1)
    ;

} /* main */

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------

//----------------------------------------------------------------------------- 
// Description:
// This function waits for the push button (PB2) to be pressed and released 
// When the button is pressed, it will display a hex value (0x03) on a 7-segment 
// display for a brief moment, then turn off the display when the button is 
// released. The function repeats this process for 3 iterations, ensuring proper 
// debouncing and timing.
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

void run_lab5_part1() {
  uint32_t index;
  for (index = Index_zero; index < 3; index++) {
    while (is_pb_up(2)) {
      msec_delay(TEN_MILLISEC_PAUSE);
    }
    while (is_pb_down(2)) {
      seg7_hex(0x03, DIG0);
      msec_delay(TEN_MILLISEC_PAUSE);
    }

    

    while (is_pb_up(2)) {
      msec_delay(TEN_MILLISEC_PAUSE);
    }
    while (is_pb_down(2)) {
      seg7_off();
      msec_delay(TEN_MILLISEC_PAUSE);
    }
    
    leds_off();
  }
  msec_delay(Five_Hundred_Millisec_Pause);
}

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------

//----------------------------------------------------------------------------- 
// Description:
// This function performs a state machine-based operation to read two nibbles 
// (low and high) from a dip switch, combine them into a full byte, and then 
// display the byte on a 7-segment display. It switches between states to get 
// the low and high nibbles, constructs the full byte, and then displays it on 
// one of two 7-segment displays depending on the state of a push button. The 
// function repeats this process for 3 iterations and includes debouncing 
// for switches and buttons.
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

void run_lab5_part2() {
  typedef enum FSM_STATES_T { GET_LOW, GET_HIGH, DISPLAY } FSM_STATES_T;

  uint32_t index_w = 0;
  uint8_t low_nibble = 0;
  uint8_t high_nibble = 0;
  uint8_t full_byte = 0;
  index_w = Index_zero;

  FSM_STATES_T state = GET_LOW;

  while (index_w < 3) {
    switch (state) {
    case (GET_LOW):
      seg7_off();

      if (is_lpsw_down(LP_SW2_IDX)) {

        low_nibble = dipsw_read();
        state = GET_HIGH;
        msec_delay(TEN_MILLISEC_PAUSE);

        while (is_lpsw_down(LP_SW2_IDX))
          ;

        msec_delay(TEN_MILLISEC_PAUSE);
      }
      break;

    case (GET_HIGH):
      if (is_lpsw_down(LP_SW2_IDX)) {
        high_nibble = dipsw_read();
        state = DISPLAY;
        msec_delay(TEN_MILLISEC_PAUSE);

        full_byte = (((high_nibble & 0x0F)) << 4) | ((low_nibble));

        while (is_lpsw_down(LP_SW2_IDX))
          ;

        msec_delay(TEN_MILLISEC_PAUSE);
      }
      break;

    case (DISPLAY):
      if (is_pb_down(PB1_IDX)) {
        seg7_on(full_byte, DIG2);
        while (is_pb_down(PB1_IDX));
        msec_delay(TEN_MILLISEC_PAUSE);
      }

      else {
        seg7_on(full_byte, DIG0);
      }

      if (is_lpsw_down(LP_SW2_IDX)) {
        state = GET_LOW;
        high_nibble = 0;
        low_nibble = 0;
        while(is_lpsw_down(LP_SW2_IDX));
        msec_delay(TEN_MILLISEC_PAUSE);
        index_w++;
      }
      break;
    }
  }
  seg7_off();
  leds_off();
  msec_delay(Five_Hundred_Millisec_Pause);
}

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------

//----------------------------------------------------------------------------- 
// Description:
// This function will sequentially turn on and off the LEDs based on key presses.
// It waits for a key to be pressed, turns on the corresponding LED, waits for 
// a brief delay, and then moves to the next LED. The sequence will continue 
// until all 8 LEDs have been processed. The function also handles waiting 
// for a key release and ensures that no key press is detected before moving 
// to the next iteration.
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

void run_lab5_part3() {
  uint32_t index_k;
  uint32_t keypressed;
  index_k = Index_zero;
  led_enable();
  while (index_k < 8) {
    keypressed = getkey_pressed();
    if (keypressed != 0x10) {
      leds_off();
      msec_delay(TEN_MILLISEC_PAUSE);
      leds_on(keypressed);
      msec_delay(TEN_MILLISEC_PAUSE);
      wait_no_key_pressed();
      

      index_k++;
    } else {
      wait_no_key_pressed();
    }
  }
  msec_delay(Five_Hundred_Millisec_Pause);
}

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------

//----------------------------------------------------------------------------- 
// Description:
// This function will sequence through a set of LEDs based on the number of 
// keys pressed on a keypad. For each valid key scan, it will turn on all LEDs 
// briefly, followed by turning them off, and repeat the process for a number of 
// times corresponding to the key value. The function repeats this process for 
// 4 iterations and includes delays and key release handling between each sequence.
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

void run_lab5_part4() {
  uint32_t index_s;
  uint32_t key_scanned;
  index_s = Index_zero;
  uint32_t LED_COUNTER;
  led_enable();
  while (index_s < 4) {
    key_scanned = keypad_scan();
    if (key_scanned != 0x10) {

      leds_off();

      for (LED_COUNTER = Index_zero; LED_COUNTER < key_scanned; LED_COUNTER++) {
        leds_on(0xFF);
        msec_delay(Five_Hundred_Millisec_Pause);
        leds_off();
        msec_delay(Five_Hundred_Millisec_Pause);
      }

      msec_delay(TEN_MILLISEC_PAUSE);
      wait_no_key_pressed();
      leds_off();

      index_s++;
    } else {
      wait_no_key_pressed();
    }
  }
  seg7_off();
  leds_off();
  msec_delay(Five_Hundred_Millisec_Pause);
}

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------