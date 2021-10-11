#ifndef JSONMAKER_H
#define JSONMAKER_H

#include "Particle.h"
#include "Arduino.h"

#define JSON_MAKER_BUFFER_SIZE 1024

class JsonMaker {
    public:

        /**
         * Constructor 
         **/
        JsonMaker();

        /**
         * Adds a new object to the buffer with an int value.
         * 
         * @param id The id for the value.
         * @param value An integer value to be stored.
         **/
        void add(String id, int value);

        /**
         * Adds a new object to the buffer with a string value.
         * 
         * @param id The id for the value.
         * @param value A string value to be stored.
         **/
        void add(String id, String value);

        /**
         * Adds a new object to the buffer with a float value.
         * 
         * @param id The id for the value.
         * @param value A float value to be stored.
         **/
        void add(String id, float value);

        /**
         * @return A JSON string representing the data stored in the
         *         JSON object.
         * */
        String get();

        /**
         * Removes the data stored in the JSON object and reinitializes
         * another buffer writer.
         * */
        void refresh();

    private:
        JSONBufferWriter* _writer;
        char _buf[JSON_MAKER_BUFFER_SIZE];

        /**
        * Initializes the JSON object.
        * */
        void _init();
    
};

#endif
