//*****************************************************************************
//*****************************    C Source Code    ***************************
//*****************************************************************************
//  DESIGNER NAME:  Mason Ferrara
//
//       LAB NAME:  Lab 6
//
//      FILE NAME:  lab6_main.c
//
//-----------------------------------------------------------------------------
//
// DESCRIPTION:  
//    This lab introduces students to the fundamentals of interfacing with alphanumeric 
//  displays, specifically Liquid Crystal Displays (LCDs). Unlike LEDs, LCDs utilize 
//  liquid crystals and polarizers to modulate light, requiring a backlight or reflector 
//  to produce visible images. LCDs are widely used in consumer electronics and IoT 
//  applications due to their thin form factor, low power consumption, and versatility.  
//
//    In this lab, students will interface an LCD with the LaunchPad development board 
//  using the I2C communication protocol on the MSPM0G3507 microcontroller. They will 
//  learn how to send commands and data to control the display, including writing 
//  alphanumeric characters and managing cursor positioning. This hands-on experience 
//  will provide students with essential skills for integrating more advanced display 
//  technologies into embedded systems.  

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


//-----------------------------------------------------------------------------
// Define function prototypes used by the program
//-----------------------------------------------------------------------------
void run_lab6_part1(void);
void lab6_part1_to_part2(void);
void run_lab6_part2(void);
void lab6_part2_to_part3(void);
void run_lab6_part3(void);
void lab6_part3_to_part4(void);
void run_lab6_part4(void);

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

//-----------------------------------------------------------------------------
// Define global variables and structures here.
// NOTE: when possible avoid using global variables
//-----------------------------------------------------------------------------


// Define a structure to hold different data types

int main(void)
{
  clock_init_40mhz();
  launchpad_gpio_init();
  I2C_init();
  lcd1602_init();
  dipsw_init();
  keypad_init();
  lcd_clear();
  
  run_lab6_part1();
  lab6_part1_to_part2();
  run_lab6_part2();
  lab6_part2_to_part3();
  run_lab6_part3();
  lab6_part3_to_part4();
  run_lab6_part4();

 // Endless loop to prevent program from ending
 while (1);

} /* main */

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------  
// Description:
// This function runs Lab 6 Part 1 by sequentially displaying the uppercase 
// English alphabet (A-Z) on an LCD screen. It begins writing characters at 
// the first line of the LCD and automatically shifts to the second line 
// once it reaches the end of the first line. The function iterates through 
// each letter from 'A' to 'Z', writing each character to the LCD display.
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

void run_lab6_part1() {
    
    uint8_t letter;

    lcd_set_ddram_addr(LCD_LINE1_ADDR);
    for (letter = 'A'; letter <= 'Z'; letter++)
        {
            lcd_write_char(letter);
            if (letter == 'A' + CHARACTERS_PER_LCD_LINE - 0x01)
            {
                lcd_set_ddram_addr(LCD_LINE2_ADDR);
            }
        }
}

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------  
// Description:
// This function transitions from Lab 6 Part 1 to Part 2 upon detecting a  
// button press. It continuously monitors the state of push button PB2,  
// waiting for it to be pressed and released. Once the button press is  
// confirmed, it clears the LCD screen, displays the message "Running Part 2"  
// for a defined duration, and then clears the screen again. The function  
// ensures proper button debounce by implementing a delay before detecting  
// the button press.
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
void lab6_part1_to_part2() {

    char message1[] = "Running Part 2";
    while (is_pb_up(PB2_IDX)){
      msec_delay(TEN_MILLISEC_PAUSE);
    }
    while (is_pb_down(PB2_IDX))
    ;
      lcd_clear();
      lcd_write_string(message1);
      msec_delay(One_Sec_Pause);
      lcd_clear();
    
}

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------  
// Description:
// This function executes the second part of Lab 6 by displaying various 
// numeric values and messages on an LCD screen in response to a button press. 
// The function follows a state-based approach, cycling through four states 
// where different types of data (quadbyte, doublebyte, single byte, and a 
// completion message) are displayed. A push button is used to advance through 
// the states, with built-in debouncing to ensure reliable input detection.
//
// The function first clears the LCD, then displays predefined numeric values 
// at specific positions on the screen. After reaching the final state, it 
// displays a completion message and introduces a delay before ending execution. 
// Each state transition is triggered by a push button press, with debouncing 
// implemented to prevent unintended multiple triggers.
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

void run_lab6_part2()
{
    uint8_t pressCount = 0;

    while (pressCount < 4) {
        switch (pressCount) {
            case 0:
                lcd_clear();
                lcd_set_ddram_addr(LCD_LINE1_ADDR + 3); 
                lcd_write_quadbyte((uint32_t)1234567890);
                break;
            case 1: 
                lcd_set_ddram_addr(LCD_LINE2_ADDR + 5);
                lcd_write_doublebyte(1234);
                break;
            case 2: 
                lcd_clear();
                lcd_set_ddram_addr(LCD_LINE1_ADDR + 7);
                lcd_write_byte(123);
                break;
            case 3: 
                lcd_clear();
                lcd_set_ddram_addr(LCD_LINE1_ADDR + 2);
                lcd_write_string("Part 2 Done.");
                msec_delay(1000);
                pressCount++;
                break;
        }
    if (pressCount != 4) {
        while (is_pb_up(PB1_IDX)){
        msec_delay(TEN_MILLISEC_PAUSE);
        }
        while(is_pb_down(PB1_IDX));
        msec_delay(TEN_MILLISEC_PAUSE); 
        pressCount++;
    } else {
        pressCount++;
    }

    }

}

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------  
// Description:
// This function transitions from Lab 6 Part 2 to Part 3 using a push button 
// (PB2) as a trigger. It first displays the message "Press PB2" on the LCD, 
// instructing the user to press the button. The function then waits for PB2 
// to be pressed and released, ensuring proper debounce handling. Once PB2 is 
// released, it clears the LCD and displays "Running Part 3" for a set duration 
// before clearing the screen again. This provides visual feedback to indicate 
// the transition to Part 3.
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

void lab6_part2_to_part3() {

    char message2[] = "Press PB2";
    char message3[] = "Running Part 3";

    lcd_clear();
    lcd_write_string(message2);
    while (is_pb_up(PB2_IDX));
    while (is_pb_down(PB2_IDX)) {
      lcd_clear();
      lcd_write_string(message3);
      msec_delay(One_Sec_Pause);
      lcd_clear();
    }
}

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------  
// Description:  
// This function executes Part 3 of Lab 6, implementing a countdown mechanism  
// displayed on an LCD screen. The countdown starts from 100 and decrements  
// each iteration, updating the LCD display accordingly. The function checks  
// for button presses to modify the countdown behavior:  
//   - If PB1 is pressed, the countdown resets to 100.  
//   - If PB2 is pressed, the countdown resets to 1 and terminates the loop.  
// The function introduces delays between iterations to control the display  
// update speed and ensure readability. Once the countdown completes, it  
// clears the LCD and displays a completion message before a final delay.  
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

void run_lab6_part3()
{
    uint32_t index;
    bool done = false;
    while (!done) {
        for ( index = 100; index != 0; index--) {
            lcd_clear();
            lcd_set_ddram_addr(LCD_LINE1_ADDR + 6);
            lcd_write_byte(index);
            if(is_pb_down(PB1_IDX)) {
                index = 101;
            }
            if (is_pb_down(PB2_IDX)) {
                index = 1;
                done = true;
            }
            msec_delay(Two_Hundred_Millisec_Pause);
        }
    }
    lcd_clear();
    lcd_write_string("Part 3 Done");
    msec_delay(One_Sec_Pause);
}

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------  
// Description:
// This function manages the transition from Lab 6 Part 3 to Part 4 using  
// a push button (PB2) as a trigger. Initially, it displays the message  
// "Press PB2" on the LCD screen, prompting the user to press the button.  
// Once PB2 is pressed and released, the function updates the LCD to  
// display "Running Part 4" for a brief period before clearing the screen.  
// The function ensures proper push-button debounce handling by waiting  
// for both press and release events before proceeding.
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

void lab6_part3_to_part4() {

    char message2[] = "Press PB2";
    char message4[] = "Running Part 4";

    lcd_clear();
    lcd_write_string(message2);
    while (is_pb_up(PB2_IDX));
    while (is_pb_down(PB2_IDX)) {
      lcd_clear();
      lcd_write_string(message4);
      msec_delay(One_Sec_Pause);
      lcd_clear();
    }
}

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------  
// Description:
// This function executes the fourth part of Lab 6, which continuously scans 
// a keypad for user input, processes the input, and displays the corresponding 
// hexadecimal value on an LCD screen. It maintains a character count to 
// determine when to shift to the second LCD line and clears the display 
// when the maximum count is reached. The function ensures proper keypress 
// handling by implementing a wait state to avoid unintended repeated inputs. 
// 
// The loop terminates when push button PB2 is pressed, at which point the LCD 
// is cleared, and a termination message is displayed.
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

void run_lab6_part4()
{
    uint8_t charCount = 0;
    bool done = false;

    while(!done) {
        uint8_t input = keypad_scan();
        wait_no_key_pressed();
        if (input != 16) 
        {
            
            if (charCount == 16) 
            {
                lcd_set_ddram_addr(LCD_LINE2_ADDR);
            }
            else if (charCount == 32) 
            {
                lcd_clear();
                charCount = 0;
                lcd_set_ddram_addr(LCD_LINE1_ADDR);
            }
            hex_to_lcd(input);
            charCount++;
            wait_no_key_pressed();

        } 
        if (is_pb_down(PB2_IDX)) {
            done = true;
        }
        
    }
    lcd_set_ddram_addr(LCD_LINE1_ADDR);
    lcd_clear();
    lcd_write_string("Program Stopped");
}