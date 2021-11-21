#ifndef _DATAQUEUE_H_
#define _DATAQUEUE_H_

#define JSON_WRITER_BUFFER_SIZE 1024
#define RAM_QUEUE_EVENT_COUNT 8

#include "settings.h"
#include "PublishQueuePosixRK.h"

/**
 * SYSTEM_THREAD(ENABLED) must be called in the global scope of the 
 * main code, or this object may fail in unpredictable ways.
 **/

class DataQueue {

    public:
        enum PublishStatus { Normal, PublishingAtMaxFrequency, DataBufferOverflow };

        /**
         * Constructor
         * */
        DataQueue(String publishHeader, void (*publishMessage)(PublishStatus));

        /**
         * Adds an integer value and its ID into the data queue.
         * An ID may not be added twice before publishing the data queue.
         * 
         * @param id A string representing an id for the data.
         * 
         * @param value the data (of type T) to be stored in 
         *              the queue.
         * */
        template <typename T>
        void add(String id, T value) { _writer->name(id).value(value); };

        /**
         * Wrapper for loop function of PublishQueuePosix
         * */
        void loop();

        /**
         * Publishes the data stored in the queue to the particle device cloud.
         * The client does not need to check if the particle board is connected 
         * to the network. This method is non-blocking.
         * 
         * @param event The name of the event to be published to.
         * 
         * @param flag1 The publish flag. Set to PUBLIC or PRIVATE.
         * 
         * @param flag2 The acknowledgement flag. Set to either WITH_ACK or NO_ACK.
         * 
         * @return Published payload
         * */
        void publish(String event, PublishFlags flag1, PublishFlags flag2);

        /**
         * Returns data in writer buffer and reinstantiates JSONBufferWriter
         * 
         * @return Data in writer buffer before reset
         * */
        String resetData();

        /**
         * Defines data wrapping for start of individual logging event
         */
        void wrapStart();
        
        /**
         * Closes data wrapping for individual logging event
         */
        void wrapEnd();

        /**
         * @brief Get the buffer size of DataQueue's JsonWriter
         * 
         * @return size_t the current buffer size
         */
        size_t getBufferSize();

        /**
         * @brief Get the size of data currently held in JsonWriter's buffer
         * 
         * @return size_t the current data size
         */
        size_t getDataSize();

    private:
        JSONBufferWriter* _writer;
        char _buf[JSON_WRITER_BUFFER_SIZE];
        PublishQueuePosix* _publishQueue;
        void (*_publishCallback)(PublishStatus);
        unsigned long _lastPublish;
        String _publishHeader;
        

        /**
         * Removes the data stored in the JSON object and reinitializes
         * writer's buffer.
         * */
        void _writerRefresh();

        /**
         * Initializes JSON Writer for DataQueue
        **/
        void _writerInit();

        /**
         * @return A JSON string representing the data stored in the
         *         JSON object.
         * */
        String _writerGet();

        /**
         * Initializes the PublishQueueAsyncRetained and JSONBufferWriter objects by
         * allocating memory on the heap. This method invokes the setup method
         * for the queue object.
         * */
        void _init();
};

#endif
