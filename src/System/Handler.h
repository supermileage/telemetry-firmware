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

// Singleton class which keeps track of all handleables and runs them
class Handler {
    public:
        /**
         * Gets the Singleton instance of this class
         * 
         * @return reference to handler instance
         */
        static Handler &instance();

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
		 * @brief Checks whether the handler contains handleable
		 * 
		 * @returns true if handler contains handleable
		 */
		bool contains(Handleable* handleable);

        /**
         * Run the begin() function of all handleables
         * */
        void begin();

        /**
         * Run the handle() function of all handleables
         * */
        void handle();
    
    private:
        static Handler* _instance;
        std::vector<Handleable*> _handleables;
};

#endif
