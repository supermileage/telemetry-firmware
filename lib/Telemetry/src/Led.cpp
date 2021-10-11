#include "Led.h"

Led::Led(uint16_t pin, uint8_t brightness){
    _pin = pin;
    _brightness = brightness;
    // Enable output pin
    pinMode(pin, OUTPUT);
    _state = OFF;
    off();
}

void Led::handle(){
    uint16_t current_time_in_interval = (millis() - _interval_time_start) % _interval;

    if(_state == FLASH_ON){ 
        if(current_time_in_interval > _interval / 2 ){
            _state = FLASH_OFF;
            analogWrite(_pin, 0);
        }
    }else if(_state == FLASH_OFF){
        if(!_flash_count_left){
            _state = OFF;
        }else if(current_time_in_interval <= _interval / 2){
            _state = FLASH_ON;
            _flash_count_left--;
            analogWrite(_pin, _brightness);
        }
    }else if(_state == FLASH_REPEAT){
        if(current_time_in_interval > _interval / 2 ){
            analogWrite(_pin, 0);
        }else{
            analogWrite(_pin, _brightness);
        }
    }
    else if(_state == PULSE){
        uint8_t pulse_brightness;
        if(current_time_in_interval < _interval / 2){
            pulse_brightness = (_brightness *current_time_in_interval * 2) / _interval;
        }else{
            pulse_brightness = (_brightness * (_interval - current_time_in_interval) * 2) / _interval;
        }

        analogWrite(_pin, pow(pulse_brightness, 2) / _brightness);
    }
}

void Led::on(){
    _state = ON;
    analogWrite(_pin, _brightness);
}

void Led::off(){
    _state = OFF;
    analogWrite(_pin, 0);
}

void Led::flash(uint16_t interval, uint8_t count){
    _state = FLASH_ON;
    _interval = interval;
    _flash_count_total = count;
    _flash_count_left = count - 1;
    _interval_time_start = millis();
    analogWrite(_pin, _brightness);
}

void Led::flashRepeat(uint16_t interval){
    _state = FLASH_REPEAT;
    _interval = interval;
    _interval_time_start = millis();
}

void Led::pulse(uint16_t interval){
    _state = PULSE;
    _interval = interval;
    _pulse_brightness = 0;
    _interval_time_start = millis();
}
