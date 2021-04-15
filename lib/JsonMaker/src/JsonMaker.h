#ifndef JSONMAKER_H
#define JSONMAKER_H

#include "Particle.h"
#include "Arduino.h"

#define BUFFER_SIZE 1024

class JsonMaker {
    public:

        JsonMaker();

        void add(String id, int value);
        void add(String id, String value);
        void add(String id, float value);

        String get();

        void refresh();

    private:
        JSONBufferWriter* writer;
        char buf[BUFFER_SIZE];

        void init();
    
};

#endif
