#ifndef _LOG_COMMAND_H
#define _LOG_COMMAND_H

class LogCommand {
    public:
        virtual void log(DataQueue &dataQ) = 0;
};

#endif