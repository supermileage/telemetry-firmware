#ifndef _CAN_LISTENER_H
#define _CAN_LISTENER_H

#include "SensorCan.h"

class CanListener {
    public:

        typedef enum {
            PASSIVE=0,
            REQUEST
        } CAN_LISTENER;

        /**
         * Constructor 
         * @param &can object to use for sending/receiving transactions
         * @param type of listener
         **/
        CanListener(SensorCan &can, CAN_LISTENER type, );


    protected:
        SensorCan &_can;
        CAN_LISTENER type;


};

#endif
