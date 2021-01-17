#ifndef DATAQUEUE_H
#define DATAQUEUE_H

#include "PublishQueueAsyncRK.h"
#include "JsonMaker.h"

class DataQueue {

    public:

        /**
         * Contructor
         * */
        DataQueue();

        /**
         * Adds an integer value and its ID into the data queue.
         * An ID may not be added twice before publishing the data queue.
         * 
         * @param id A string representing an id for the data.
         * 
         * @param value An integer representing the data to be stored in 
         *               the queue.
         * */
        void add(String id, int value);

        /**
         * Adds a string value and its ID into the data queue.
         * An ID may not be added twice before publishing the data queue.
         * 
         * @param id A string representing an id for the data.
         * 
         * @param value A string representing the data to be stored in 
         *               the queue.
         * */
        void add(String id, String value);

        /**
         * Adds a float value and its ID into the data queue.
         * An ID may not be added twice before publishing the data queue.
         * 
         * @param id A string representing an id for the data.
         * 
         * @param value A float representing the data to be stored in 
         *               the queue.
         * */
        void add(String id, float value);

        /**
         * Publishes the data stored in the queue to the particle device cloud.
         * The client does not need to check if the particle board is connected 
         * to the network. This method is non-blocking.
         * 
         * @param event The name of the event to be published to.
         * 
         * @param flag1 The publish flag. Set to PUBLIC or PRIVATE.
         * 
         * @param flag2 The acknowledgement flag. Set to either WITH_ACK or NO_ACK
         * */
        void publish(String event, PublishFlags flag1, PublishFlags flag2);

    private:

        PublishQueueAsync* publishQueue;
        JsonMaker* jsonMaker;
        uint8_t publishQueueRetainedBuffer[16384];
};

#endif
