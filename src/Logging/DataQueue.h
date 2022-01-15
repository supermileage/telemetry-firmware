#ifndef _DATAQUEUE_H_
#define _DATAQUEUE_H_

#if 0
    #define DEBUG_LOGGING_LN(x) Serial.println(x)
    #define DEBUG_LOGGING(x) Serial.print(x)
#else
    #define DEBUG_LOGGING_LN(x) x
    #define DEBUG_LOGGING(x) x
#endif

#define JSON_WRITER_BUFFER_SIZE 116
#define JSON_WRITER_OVERFLOW_CAPACITY 256
#define RAM_QUEUE_EVENT_COUNT 8

#include "Handleable.h"
#include "settings.h"
#include "PublishQueuePosixRK.h"

#undef max
#define ARDUINOJSON_ENABLE_PROGMEM 0
#include "ArduinoJson.h"

/**
 * SYSTEM_THREAD(ENABLED) must be called in the global scope of the 
 * main code, or this object may fail in unpredictable ways.
 **/

class DataQueue : public Handleable {

    public:
        /**
         * Describes set of possible publishing states
         **/
        enum PublishStatus { Normal, PublishingAtMaxFrequency, DataBufferOverflow };

        /**
         * Constructor
         * */
        DataQueue(String publishHeader, void (*publishMessage)(String, PublishStatus));

        ~DataQueue();

        /**
         * Initializes the PublishQueueAsyncRetained and JSONBufferWriter objects by
         * allocating memory on the heap. This method invokes the setup method
         * for the queue object.
         * */
        void begin();

        /**
         * Wrapper for loop function of PublishQueuePosix
         * */
        void handle();

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
         * Defines data wrapping for start of individual logging event:
         * Opens JObject and adds timestamp
         **/
        void wrapStart();
        
        /**
         * Closes data wrapping for individual logging event: closes JObject
         **/
        void wrapEnd();

        /**
         * @brief Get the buffer size of DataQueue's JsonWriter
         * 
         * @return size_t - the current buffer size
         **/
        size_t getBufferSize();

        /**
         * @brief Get the size of data currently held in JsonWriter's buffer
         * 
         * @return size_t - the current data size
         **/
        size_t getDataSize();

        /**
         * @brief Get the number of events in the publish queue
         * 
         * @return size_t
         **/
        size_t getNumEventsInQueue();

        /**
         * @brief returns true if File queue is full
         * 
         * @return bool
         **/
        bool isCacheFull();

    private:
        JSONBufferWriter* _writer;
        char _buf[JSON_WRITER_BUFFER_SIZE + JSON_WRITER_OVERFLOW_CAPACITY];
        PublishQueuePosix* _publishQueue;
        void (*_publishCallback)(String, PublishStatus);
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
         * Reparses Json data and removes last entry from JObject at removalIndex in internal JArray
         * Only used in the case that the JsonWriter's data buffer overflows
         * 
         * @return String payload -- json data string
         */
        String _recoverDataFromBuffer();


};

#endif
