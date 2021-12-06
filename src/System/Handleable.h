#ifndef _HANDLEABLE_H
#define _HANDLEABLE_H

#include "Particle.h"
#include "Handler.h"
class Handler;

class Handleable {
    public:
    
        static Handler handler;

        Handleable();

        ~Handleable();

        /**
         * INTERFACE
         * Begin Handleable Object
         * */
        virtual void begin() = 0;

        /**
         * INTERFACE
         * Run Handleable object every loop
         * */
        virtual void handle() = 0;
        

};

#endif
