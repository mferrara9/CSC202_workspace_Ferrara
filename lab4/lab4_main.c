//*****************************************************************************
//*****************************    C Source Code    ***************************
//*****************************************************************************
//  DESIGNER NAME:  Mason Ferrara
//
//       LAB NAME:  Lab 4
//
//      FILE NAME:  lab4_main.c
//
//-----------------------------------------------------------------------------
//
// DESCRIPTION:
//    This program serves as a practice exercise for students to learn how to 
//    interface with output devices by controlling the MSPM0G307 microcontroller
//    LEDs and seven segment display. Students will also configure and manipulate
//    the IOMUX and GPIO modules and write and debug code to control the hardare 
//    components.
//
//    Students must develop a function for each part of the lab that properly 
//    performs the assigned task. Each function will run, one after another and
//    will be timed. Students use pauses within and between functions using the
//    msec_delay() function. After completing all the parts, the program terminates.
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


//-----------------------------------------------------------------------------
// Define function prototypes used by the program
//-----------------------------------------------------------------------------
void run_lab4_part1(void);
void run_lab4_part2(void);
void run_lab4_part3(void);
void run_lab4_part4(void);
void run_lab4_part5(void);
void run_lab4_part6(void);



//-----------------------------------------------------------------------------
// Define symbolic constants used by the program
//-----------------------------------------------------------------------------

#define Five_Hundred_Millisec_Pause                                      (0x1F4)
#define Two_Hundred_Millisec_Pause                                       (0xC8)
#define One_Hundred_Millisec_Pause                                       (0x64)
#define Five_Millisec_Pause                                              (0x5)
#define One_Sec_Pause                                                    (0x3E8)
#define Two_Sec_Pause                                                    (0x7D0)
#define Three_Sec_Pause                                                  (0xBB8)
#define Part_One_LEDs                                                    (0x3C)
#define Index_zero                                                       (0)
#define Zero_LED                                                         (0)
#define Counter_Zero                                                     (0x0)
#define Counter_Max                                                      (0xFF)
#define Up_One                                                           (0x1)
#define L_SEG_DIS                                                        (0x38)
#define DIG0                                                             (0)
#define DIG1                                                             (1)
#define DIG2                                                             (2)
#define DIG3                                                             (3)
#define FOUR_SEG_DIS                                                     (0x66)
#define C_SEG_DIS                                                        (0x39)
#define A_SEG_DIS                                                        (0x77)
#define F_SEG_DIS                                                        (0x71)
#define E_SEG_DIS                                                        (0x79)




//-----------------------------------------------------------------------------
// Define global variables and structures here.
// NOTE: when possible avoid using global variables
//-----------------------------------------------------------------------------


// Define a structure to hold different data types


int main(void)
{
  clock_init_40mhz();
  launchpad_gpio_init();
  led_init();
  led_enable();
  


  run_lab4_part1();

  run_lab4_part2();

  run_lab4_part3();

  seg7_init();
  led_disable();
  run_lab4_part4();

  run_lab4_part5();

  run_lab4_part6();




  
 
 // Endless loop to prevent program from ending
 while (1);




} /* main */


//-----------------------------------------------------------------------------
// Description:
// This function turns LED5 through LED 2, using the leds_on() function, and 
// then off using the leds_off() function.
//
// INPUT PARAMETERS:
//  none
// OUTPUT PARAMETERS:
//  none
// RETURN:
//  none
//-----------------------------------------------------------------------------

void run_lab4_part1()
{
  leds_on(Part_One_LEDs);
  msec_delay(One_Sec_Pause);
  leds_off();
}

//-----------------------------------------------------------------------------
// Description:
// This function will sequence through the first 8 LEDs and turn each on and off
// with the use of embedded for loops within a while loop.
//
// INPUT PARAMETERS:
//  none
// OUTPUT PARAMETERS:
//  none
// RETURN:
//  none
//-----------------------------------------------------------------------------
void run_lab4_part2()
{
    uint32_t index = Index_zero;

    while( index < 5 )
    {
        uint32_t For_Loop_Index;

        for (For_Loop_Index = 0; For_Loop_Index < 8; For_Loop_Index++)
        {
            led_on(For_Loop_Index);
            msec_delay(Two_Hundred_Millisec_Pause);
            led_off(For_Loop_Index);
        }

        for (For_Loop_Index = 6; For_Loop_Index > 0; For_Loop_Index--)
        {
            led_on(For_Loop_Index);
            msec_delay(Two_Hundred_Millisec_Pause);
            led_off(For_Loop_Index);
        }
        index++;
    }
    led_on(Zero_LED);
    msec_delay(Two_Hundred_Millisec_Pause);
    led_off(Zero_LED);
    msec_delay(Five_Hundred_Millisec_Pause);
}

//-----------------------------------------------------------------------------
// Description:
// This function will count up in binary and display such on the LEDs until a
// binary value of 1111_1111 and rolls back over to 0. This uses an embedded for
// loop within a while loop.
//
// INPUT PARAMETERS:
//  none
// OUTPUT PARAMETERS:
//  none
// RETURN:
//  none
//-----------------------------------------------------------------------------
void run_lab4_part3()
{
    uint32_t index = Index_zero;


    while( index < 2 )
    {
        uint32_t counter = Counter_Zero;

        for (counter = Counter_Zero; counter <= Counter_Max; counter += Up_One)
        {
            leds_on(counter);
            msec_delay(One_Hundred_Millisec_Pause);
            //led_off(counter);
        }
        msec_delay(One_Hundred_Millisec_Pause);
        leds_off();

        index++;
    }
    msec_delay(Five_Hundred_Millisec_Pause);

}

//-----------------------------------------------------------------------------
// Description:
// This function displays the letter "L" on the seven segment display. This function
// disables the LEDs using the led_disable() function and turns on and off the seven segment
// display using the seg7_on() and seg7_off() functions.
//
// INPUT PARAMETERS:
//  none
// OUTPUT PARAMETERS:
//  none
// RETURN:
//  none
//-----------------------------------------------------------------------------
void run_lab4_part4()
{
    led_disable();
    seg7_on(L_SEG_DIS, DIG0);
    msec_delay(Five_Hundred_Millisec_Pause);
    seg7_off();
    msec_delay(Five_Hundred_Millisec_Pause);
}

//-----------------------------------------------------------------------------
// Description:
// This function will display and blink the number "4" on the seven segment display.
// This uses a while loop and turns the seven segment display on and off using the 
// seg7_on() and seg7_off() functions.
//
// INPUT PARAMETERS:
//  none
// OUTPUT PARAMETERS:
//  none
// RETURN:
//  none
//-----------------------------------------------------------------------------
void run_lab4_part5()
{
    uint32_t index = Index_zero;

    while( index < 4 )
    {

      seg7_on(FOUR_SEG_DIS, DIG2);
      msec_delay(Three_Sec_Pause);
      seg7_off();
      msec_delay(Two_Sec_Pause);

      index++;
    }
    msec_delay(Five_Hundred_Millisec_Pause);
}

//-----------------------------------------------------------------------------
// Description:
// This function uses multiplexing to display "CAFE" on the seven segment display
// using a while loop and the seg7_on() and seg7_off() functions.
//
// INPUT PARAMETERS:
//  none
// OUTPUT PARAMETERS:
//  none
// RETURN:
//  none
//-----------------------------------------------------------------------------
void run_lab4_part6()
{
    uint32_t index = Index_zero;

    while( index < 200 )
    {

      seg7_on(C_SEG_DIS, DIG0);
      msec_delay(Five_Millisec_Pause);
      seg7_on(A_SEG_DIS, DIG1);
      msec_delay(Five_Millisec_Pause);
      seg7_on(F_SEG_DIS, DIG2);
      msec_delay(Five_Millisec_Pause);
      seg7_on(E_SEG_DIS, DIG3);
      msec_delay(Five_Millisec_Pause);
      seg7_off();
      msec_delay(Five_Millisec_Pause);

      index++;
    }
}