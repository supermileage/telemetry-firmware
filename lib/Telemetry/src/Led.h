#ifndef _LED_H_
#define _LED_H_
#include "Arduino.h"


class Led {
    public:
        /**
         * Constructor
         * 
         * @param pin The output pin for the LED anode
         * @param flag1 LED max brightness (0-255)
         * 
         * */
        Led(uint16_t pin, uint8_t brightness);

        /**
         * Handles flashing and pulsing, should be run as often as possible
         * */
        void handle();

        /**
         * Turn LED on
         * */
        void on();

        /**
         * Turn LED off
         * */
        void off();

        /**
         * Flash LED a specific number of times, then turn off. Duty cycle is always 50%
         * 
         * @param interval Time between flashes (ms)
         * @param count Number of times to flash before turning off
         * 
         * */
        void flash(uint16_t interval, uint8_t count);

        /**
         * Flash LED indefinitely. Duty cycle is always 50%
         * 
         * @param interval Time between flashes (ms)
         * 
         * */
        void flashRepeat(uint16_t interval);

        /**
         * Pulse LED indefinitely
         * 
         * @param interval Time between pulses (ms)
         * 
         * */
        void pulse(uint16_t interval);

    private:
        enum LED_STATE {OFF, ON, PULSE, FLASH_ON, FLASH_OFF, FLASH_REPEAT};
        
        uint16_t _pin;
        uint8_t _brightness;

        LED_STATE _state;

        long _interval_time_start;
        uint16_t _interval;
        uint8_t _flash_count_total;
        uint8_t _flash_count_left;
        uint8_t _pulse_brightness;

};

#endif
