#ifndef _HANDLER_H
#define _HANDLER_H

// This Particle definition causes a conflict with some standard
// C++ libraries
#undef max
#include <vector>

#include "Particle.h"
#include "Handleable.h"
// Forward declaration due to mutual inclusion
class Handleable;

// Keeps track of all handleables and runs them
class Handler {
    public:
        
        /**
         * Add a handleable object to list of handleables
         * 
         * @param *handleable object to add
         * */
        void add(Handleable* handleable);

        /**
         * Remove a handleable object from list of handleables
         * 
         * @param *handleable object to remove
         * */
        void remove(Handleable* handleable);

        /**
         * Run the begin() function of all handleables
         * */
        void begin();

        /**
         * Run the handle() function of all handleables
         * */
        void handle();
    
    private:
        std::vector<Handleable*> _handleables;

};

#endif
