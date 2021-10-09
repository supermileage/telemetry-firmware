#ifndef _LED_H_
#define _LED_H_
#include "Arduino.h"


class Led {
    public:
        Led(uint16_t pin, uint8_t brightness);

        void handle();
        void on();
        void off();
        void flash(uint16_t interval, uint8_t count);
        void flashRepeat(uint16_t interval);
        void pulse(uint16_t interval);

    private:
        enum LED_STATE {OFF, ON, PULSE, FLASH_ON, FLASH_OFF, FLASH_REPEAT};
        
        uint16_t _pin;
        uint8_t _brightness;
        LED_STATE _state;

        uint16_t _interval;
        uint8_t _flash_count_total;
        uint8_t _flash_count_left;
        
        long _interval_time_start;

        uint8_t _pulse_brightness;

};

#endif
