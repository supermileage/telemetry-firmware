#ifndef _HANDLEABLE_H
#define _HANDLEABLE_H

#include "Particle.h"
#include "Handler.h"
// Forward declaration due to mutual inclusion
class Handler;

// Abstract class for any object that can be "handleable"
// (ie. would need a function to run regularly in the loop)
class Handleable {
    public:

        /**
         * STATIC - Returns Handler instance for all handleables
         * */
        static Handler &handlerInstance();

        /**
         * Constructor
         * 
         * Adds this object to the handler's list of handleables
         * */
        Handleable();

        /**
         * Destructor
         * 
         * Removes this object from the handler's list of handleables
         * */
        ~Handleable();

        /**
         * Begin Handleable Object
         * */
        virtual void begin() = 0;

        /**
         * Run Handleable object every loop
         * */
        virtual void handle() = 0;
        
    private:

        // One handler is shared by all handleables
        static Handler *_handlerInstance;

};

#endif
