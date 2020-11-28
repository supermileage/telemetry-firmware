#include "Particle.h"
#include "Arduino.h"

#define BUFF_SIZE 1024

class JsonMaker {
    public:
        JsonMaker();

        void init();

        void add(String id, int value);
        void add(String id, String value);
        void add(String id, float value);

        String get();

    private:
        JSONBufferWriter* writer;
        char buf[BUFF_SIZE];
    
};