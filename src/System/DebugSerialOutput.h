#ifndef _DEBUG_SERIAL_H_
#define _DEBUG_SERIAL_H_

#include "Particle.h"

class DebugSerial {
    public:
        ~DebugSerial();
        static DebugSerial& instance();
        bool getSilent();
        void setSilent(bool silent);
        void println(const char* s);
        void println();
        void print(const char* s);
        void printf(const char* format, ...);

    private:
        static DebugSerial* _instance;
        static bool _silent;

        DebugSerial();
};

#endif