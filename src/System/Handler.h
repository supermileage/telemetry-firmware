#ifndef _HANDLER_H
#define _HANDLER_H

#undef max

#include <vector>
#include "Particle.h"
#include "Handleable.h"
class Handleable;

class Handler {
    public:
        
        void add(Handleable* handleable);

        void begin();

        void handle();

        uint16_t getSize();
    
    private:
        std::vector<Handleable*> _handleables;

};

#endif
