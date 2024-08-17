/*
 * Copyright (C) 2024 dBm Signal Dynamics Inc.
 *
 * File:            board.cpp
 * Project:         
 * Date:            June 24, 2024
 * Framework:       Arduino (Arduino-Pico Board Pkge by Earl Philhower v3.8.1)
 * 
 * cetalib "board" driver (USER LED, PUSHBUTTON & POTETIOMETER interface functions)
 *
 * Hardware Configuration:
 * CETA IoT Robot (schematic #14-00069A/B), based on RPI-Pico-WH 
 *
 */

/** Include Files *************************************************************/
#include <Arduino.h>            // Required for Arduino functions
#include "board.h"              // "board" API declarations

/*** Symbolic Constants used in this module ***********************************/

/*** Global Variable Declarations *********************************************/

/*** Type Declarations ********************************************************/
extern const struct BOARD_INTERFACE BOARD = {
    .initialize             = &board_init,
    .tasks                  = &board_tasks,
    .led_on                 = &board_led_on,
    .led_off                = &board_led_off,
    .led_toggle             = &board_led_toggle,
    .led_blink              = &board_led_blink,
    .led_pattern            = &board_led_pattern,
    .is_button_pressed      = &board_is_button_pressed,
    .is_button_released     = &board_is_button_released,
    .get_button_level       = &board_get_button_level,
    .wait_for_button        = &board_wait_for_button,
    .get_potentiometer      = &board_get_potentiometer
};

// led-related variables
LED_STATE ledState;
LED_FUNCTION_STATE ledFunctionState;
unsigned long ledBlinkCurrentTime, ledBlinkPrevTime, ledBlinkInterval;
unsigned long ledPatternCurrentTime, ledPatternPrevTime;
const long ledPatternInterval = LED_PATTERN_INTERVAL;
int ledOutputValue[5][10] = {       // provide 5 unique flashing patterns
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0},   // Pattern 1 - blink 1x/sec
  {1, 0, 1, 0, 0, 0, 0, 0, 0, 0},   // Pattern 2 - blink 2x/sec
  {1, 0, 1, 0, 1, 0, 0, 0, 0, 0},   // Pattern 3 - blink 3x/sec
  {1, 0, 1, 0, 1, 0, 1, 0, 0, 0},   // Pattern 4 - blink 4x/sec
  {1, 0, 1, 0, 1, 0, 1, 0, 1, 0}    // Pattern 5 - blink 5x/sec (constant)
};
int ledPatternIndex;       // row pointer: selects flashing pattern
int ledValueIndex;         // column pointer: selects current LED value

// button-related variables
int buttonLevelCurrent, buttonLevelPrevious;

// potentiometer-related variables
int potentiometerReading;

/*** Private Function Prototypes **********************************************/

/*** Public Function Definitions **********************************************/

void board_init(void)
{
    // initiallize user LED
    pinMode(LED_PIN, OUTPUT);   // set digital pin as output
    digitalWrite(LED_PIN, 0);   // set initial level
    ledState = OFF;
    ledFunctionState = DEFAULT;

    // initiallize pushbutton
    pinMode(BUTTON_PIN, INPUT); // set digital pin as input
    buttonLevelCurrent = digitalRead(BUTTON_PIN);   // save button level

    // initialize ADC resolution to 12-bit
    analogReadResolution(12);
}

void board_tasks(void)
{
    switch(ledFunctionState)
    {
        case DEFAULT:
            break;

        case BLINK:
            ledBlinkCurrentTime = millis();
            if((ledBlinkCurrentTime - ledBlinkPrevTime) >= ledBlinkInterval)
            {
                switch(ledState)
                {
                    case OFF:
                        digitalWrite(LED_PIN, 1);
                        ledState = ON;
                        break;
                    case ON:
                        digitalWrite(LED_PIN, 0);
                        ledState = OFF;
                        break;
                    default:
                        break;
                }
                ledBlinkPrevTime = ledBlinkCurrentTime;        
            }
            break;
        
        case PATTERN:
            ledPatternCurrentTime = millis();
            if((ledPatternCurrentTime - ledPatternPrevTime) >= ledPatternInterval)
            {
                ledPatternPrevTime = ledPatternCurrentTime;
                if(ledValueIndex > 9)
                {
                    ledValueIndex = 0;
                }

                switch(ledPatternIndex)
                {
                    case 0:
                        digitalWrite(LED_PIN, ledOutputValue[0][ledValueIndex++]);
                        break;

                    case 1:
                        digitalWrite(LED_PIN, ledOutputValue[1][ledValueIndex++]);
                        break;

                    case 2:
                        digitalWrite(LED_PIN, ledOutputValue[2][ledValueIndex++]);
                        break;

                    case 3:
                        digitalWrite(LED_PIN, ledOutputValue[3][ledValueIndex++]);
                        break;

                    case 4:
                        digitalWrite(LED_PIN, ledOutputValue[4][ledValueIndex++]);
                        break;

                    default:
                        break;
                }

            }
            break;
        
        default:
            break;
    }

    buttonLevelPrevious = buttonLevelCurrent;
    buttonLevelCurrent = digitalRead(BUTTON_PIN);

}

void board_led_on(void)
{
    digitalWrite(LED_PIN, 1);
    ledState = ON;
    ledFunctionState = DEFAULT;
}

void board_led_off(void)
{
    digitalWrite(LED_PIN, 0);
    ledState = OFF;
    ledFunctionState = DEFAULT;
}

void board_led_toggle(void)
{
    switch(ledState)
    {
        case OFF:
            board_led_on();
            break;
        case ON:
            board_led_off();
            break;
        default:
            break;
    }
}

void board_led_blink(int frequency)
{
    float blinkInterval;
    if(frequency <= 0)
    {
        Serial.println("Led Blink Frequency out of range (<= 0 Hz). Led Off");
        board_led_off();
        return;
    }
    else if(frequency > 20)
    {
        ledBlinkInterval = 25;
        Serial.println("Led Blink Frequency out of range (> 20 Hz). Freq = 20 Hz");
        ledBlinkPrevTime = millis();
        ledFunctionState = BLINK;
    }
    else
    {
        blinkInterval = (1.0/frequency)*0.5;
        ledBlinkInterval = (unsigned long)(blinkInterval*1000);
        //Serial.print("Led Blink Frequency (Hz): ");
        //Serial.println(frequency);
        ledBlinkPrevTime = millis();
        ledFunctionState = BLINK;
    }
}

void board_led_pattern(int pattern)
{
    if((pattern < 1)||(pattern > 5))
    {
        Serial.println("Led Pattern selection is out of range (1 - 5). Disabled.");
        board_led_off();
        return;
    }
    else
    {
        ledPatternPrevTime = millis();
        ledPatternIndex = pattern - 1;
        ledFunctionState = PATTERN;
    }
}

bool board_is_button_pressed(void)
{
    if((buttonLevelCurrent==0)&&(buttonLevelPrevious==1)){
        return true;
    }
    else
    {
        return false;
    }
}

bool board_is_button_released(void)
{
    if((buttonLevelCurrent==1)&&(buttonLevelPrevious==0)){
        return true;
    }
    else
    {
        return false;
    } 
}

void board_wait_for_button(void)
{
    do
    {
        board_tasks();
    } while (!board_is_button_pressed());  
}

int board_get_button_level(void)
{
    return (digitalRead(BUTTON_PIN));
}

int board_get_potentiometer(void)
{
    // Note that the potentiometer output is shared with LEFT OPTO signal, so ensure that
    // the POTENTIOMETER signal is connected to the AN2 input before using this function
    return analogRead(POTENTIOMETER_PIN);
}

