#include "Led.h"

Led::Led(uint16_t pin, uint8_t brightness){
    _pin = pin;
    _brightness = brightness;
    pinMode(pin, OUTPUT);
    _state = OFF;
    off();
}

void Led::handle(){
    if(_state == FLASH_ON){ 
        if((millis() - _interval_time_start) % _interval > _interval / 2 ){
            _state = FLASH_OFF;
            analogWrite(_pin, 0);
        }
    }else if(_state == FLASH_OFF){
        if(!_flash_count_left){
            _state = OFF;
        }else if((millis() - _interval_time_start) % _interval <= _interval / 2){
            _state = FLASH_ON;
            _flash_count_left--;
            analogWrite(_pin, _brightness);
        }
    }else if(_state == FLASH_REPEAT){
        if((millis() - _interval_time_start) % _interval < _interval / 2 ){
            analogWrite(_pin, _brightness);
        }else{
            analogWrite(_pin, 0);
        }
    }
    else if(_state == PULSE){
        uint16_t current_time_in_cycle = (millis() - _interval_time_start) % _interval;
        if(current_time_in_cycle < _interval / 2){
            analogWrite(_pin, (_brightness * current_time_in_cycle) / (_interval / 2));
        }else{
            analogWrite(_pin, (_brightness * (_interval - current_time_in_cycle)) / (_interval / 2));
        }
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
