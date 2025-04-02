/*
 * Copyright (C) 2025 dBm Signal Dynamics Inc.
 *
 * File:            board.cpp
 * Project:         
 * Date:            Mar 29, 2025
 * Framework:       Arduino w. Arduino-Pico Core Pkge by Earl Philhower
 *                  (https://github.com/earlephilhower/arduino-pico)
 * 
 * cetalib "board" driver (USER LED, PUSHBUTTON & POTENTIOMETER interface functions)
 *
 * Hardware Configurations Supported:
 * 
 * CETA IoT Robot (Schematic #14-00069A/B), based on RPI-Pico-WH
 * (Select "Board = Raspberry Pi Pico W")
 * USER LED is a regular RED LED
 * 
 * Sparkfun XRP Robot Platform (#KIT-27644), based on the RPI RP2350B MCU
 * (Select "Board = SparkFun XRP Controller")
 * USER LED is a WS2812B NeoPixel driven with fixed color (RED)
 *
 */

/** Include Files *************************************************************/
#include <Arduino.h>            // Required for Arduino functions
#include <Adafruit_NeoPixel.h>  // Required for NeoPixel Functions
#include "board.h"              // "board" API declarations

/*** Symbolic Constants used in this module ***********************************/

/*** Global Variable Declarations *********************************************/

/*** Type Declarations ********************************************************/

#if defined(ARDUINO_RASPBERRY_PI_PICO_W)
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
#elif defined(ARDUINO_SPARKFUN_XRP_CONTROLLER)
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
    .wait_for_button        = &board_wait_for_button
};
#else
  #error Unsupported board selection
#endif

// led-related variables
static LED_STATE ledState;
static LED_FUNCTION_STATE ledFunctionState;
static unsigned long ledBlinkCurrentTime, ledBlinkPrevTime, ledBlinkInterval;
static unsigned long ledPatternCurrentTime, ledPatternPrevTime;
static const long ledPatternInterval = LED_PATTERN_INTERVAL;
static int ledOutputValue[5][10] = {       // provide 5 unique flashing patterns
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0},   // Pattern 1 - blink 1x/sec
  {1, 0, 1, 0, 0, 0, 0, 0, 0, 0},   // Pattern 2 - blink 2x/sec
  {1, 0, 1, 0, 1, 0, 0, 0, 0, 0},   // Pattern 3 - blink 3x/sec
  {1, 0, 1, 0, 1, 0, 1, 0, 0, 0},   // Pattern 4 - blink 4x/sec
  {1, 0, 1, 0, 1, 0, 1, 0, 1, 0}    // Pattern 5 - blink 5x/sec (constant)
};
static int ledPatternIndex;       // row pointer: selects flashing pattern
static int ledValueIndex;         // column pointer: selects current LED value

// button-related variables
static int buttonLevelCurrent, buttonLevelPrevious;

// potentiometer-related variables
static int potentiometerReading;

// define neopixel object
#if defined(ARDUINO_SPARKFUN_XRP_CONTROLLER)
Adafruit_NeoPixel pixel(1, LED_PIN, NEO_GRB + NEO_KHZ800);
#endif

/*** Private Function Prototypes **********************************************/

/*** Public Function Definitions **********************************************/

void board_init(void)
{
    // initiallize user LED
    #if defined(ARDUINO_RASPBERRY_PI_PICO_W)
        pinMode(LED_PIN, OUTPUT);   // set digital pin as output
        digitalWrite(LED_PIN, 0);   // set initial level
    #elif defined(ARDUINO_SPARKFUN_XRP_CONTROLLER)
        pixel.begin();              // INITIALIZE NeoPixel strip object (REQUIRED)
        pixel.setBrightness(10);    // Set a bearable brightness
        pixel.clear();              // Set all pixel colors to 'off'
        pixel.show();               // Update the pixel color
    #else
        #error Unsupported board selection
    #endif
    ledState = OFF;
    ledFunctionState = DEFAULT;

    // initiallize pushbutton
    #if defined(ARDUINO_RASPBERRY_PI_PICO_W)
        pinMode(BUTTON_PIN, INPUT); // set digital pin as input
    #else
        pinMode(BUTTON_PIN, INPUT_PULLUP); // set digital pin as input
    #endif
    buttonLevelCurrent = digitalRead(BUTTON_PIN);   // save button level

    #if defined(ARDUINO_RASPBERRY_PI_PICO_W)
    // initialize ADC resolution to 12-bit
        analogReadResolution(12);
    #endif
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
                        #if defined(ARDUINO_RASPBERRY_PI_PICO_W)
                            digitalWrite(LED_PIN, 1);
                        #elif defined(ARDUINO_SPARKFUN_XRP_CONTROLLER)
                            pixel.setPixelColor(0, pixel.Color(0, 150, 0));
                            pixel.show();
                        #else
                            #error Unsupported board selection
                        #endif
                        ledState = ON;
                        break;
                    case ON:
                        #if defined(ARDUINO_RASPBERRY_PI_PICO_W)
                            digitalWrite(LED_PIN, 0);
                        #elif defined(ARDUINO_SPARKFUN_XRP_CONTROLLER)
                            pixel.clear();
                            pixel.show();
                        #else
                            #error Unsupported board selection
                        #endif
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

                if(0 == ledOutputValue[ledPatternIndex][ledValueIndex++])
                {
                    #if defined(ARDUINO_RASPBERRY_PI_PICO_W)
                        digitalWrite(LED_PIN, 0);
                    #elif defined(ARDUINO_SPARKFUN_XRP_CONTROLLER)
                        pixel.clear();
                        pixel.show();
                    #else
                        #error Unsupported board selection
                    #endif
                }
                else
                {
                    #if defined(ARDUINO_RASPBERRY_PI_PICO_W)
                        digitalWrite(LED_PIN, 1);
                    #elif defined(ARDUINO_SPARKFUN_XRP_CONTROLLER)
                        pixel.setPixelColor(0, pixel.Color(0, 150, 0));
                        pixel.show();
                    #else
                        #error Unsupported board selection
                    #endif
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
    #if defined(ARDUINO_RASPBERRY_PI_PICO_W)
        digitalWrite(LED_PIN, 1);
    #elif defined(ARDUINO_SPARKFUN_XRP_CONTROLLER)
        pixel.setPixelColor(0, pixel.Color(0, 150, 0));
        pixel.show();
    #else
        #error Unsupported board selection
    #endif
    ledState = ON;
    ledFunctionState = DEFAULT;
}

void board_led_off(void)
{
    #if defined(ARDUINO_RASPBERRY_PI_PICO_W)
        digitalWrite(LED_PIN, 0);
    #elif defined(ARDUINO_SPARKFUN_XRP_CONTROLLER)
        pixel.clear();
        pixel.show();
    #else
        #error Unsupported board selection
    #endif
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

#if defined(ARDUINO_RASPBERRY_PI_PICO_W)
int board_get_potentiometer(void)
{
    // Note that the potentiometer output is shared with LEFT OPTO signal, so ensure that
    // the POTENTIOMETER signal is connected to the AN2 input before using this function
    return analogRead(POTENTIOMETER_PIN);
}
#endif

