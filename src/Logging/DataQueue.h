#ifndef _DATAQUEUE_H_
#define _DATAQUEUE_H_

// Particle cloud publish size limit is 1024B
#define JSON_BUFFER_SIZE 1024
#define JSON_OVERFLOW_CAPACITY 256
#define RAM_QUEUE_EVENT_COUNT 8

// JsonDocument size sets the maximum allocated memory for the object: memory usage depends on complexity of json
#define JSON_DOCUMENT_SIZE 2048

#include "Handleable.h"
#include "settings.h"
#include "PublishQueuePosixRK.h"

#undef max
#define ARDUINOJSON_ENABLE_PROGMEM 0
#include "ArduinoJson.h"

/**
 * @brief DataQueue which provides API for logging and publishing of data to Particle cloud
 * 
 * @note SYSTEM_THREAD(ENABLED) must be called in on startup, or this object may fail in unpredictable ways
 * @note formatting is specified in the methods _jsonDocumentInit and createDataObject.
 * If you wish to change the formatting, you must also modify (or remove) _recoverDataFromBuffer
 **/

class DataQueue : public Handleable {
    public:
        /**
         * Describes set of possible publishing states
         **/
        enum PublishStatus { Normal, PublishingAtMaxFrequency, DataBufferOverflow, JsonDocumentOverflow };

        struct PublishData {
            PublishStatus status;
            size_t jsonDocumentSize;
        };

        /**
         * Constructor
         * */
        DataQueue(String publishHeader, void (*publishMessage)(String, PublishData));

        ~DataQueue();

        /**
         * Initializes PublishQueuePosix and StaticJsonDocument member object
         * */
        void begin();

        /**
         * Wrapper for loop function of PublishQueuePosix
         * */
        void handle();

        /**
         * @brief Creates and adds a JsonObject to data array in JsonDocument member to which you can safely add
         * data to be published
         * 
         * @return JsonObject to add data to
         */
        JsonObject createDataObject();

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
         * @brief gets the max json string length
         * 
         * @return size_t - max length for published json string
         **/
        size_t getBufferSize();

        /**
         * @brief length of serialized json string in StaticJsonDocument
         * 
         * @return size_t - current json string length
         **/
        size_t getDataSize();

                /**
         * @brief Get the current size in memory of StaticJsonDocument
         * 
         * @return size_t - current memory usage of json document
         **/
        size_t getMemoryUsage();

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

        /**
         * @brief Returns false and prints an error message to Serial if StaticJsonDocument has overflowed
         * 
         * @note This is just a precaution.  Overflow is extremely unlikely given current project settings
         */
        bool verifyJsonStatus();

    private:
        StaticJsonDocument<JSON_DOCUMENT_SIZE> _jsonDocument;
        PublishQueuePosix* _publishQueue;
        void (*_publishCallback)(String, PublishData);
        unsigned long _lastPublish;
        String _publishHeader;
        

        /**
         * Removes the data stored in the StaticJsonDocument member and clears its currently held data
         * */
        void _jsonDocumentRefresh();

        /**
         * Initializes StaticJsonDocument member
        **/
        void _jsonDocumentInit();

        /**
         * @return A Json string representing the data stored in StaticJsonDocument member
         * */
        String _jsonBufferGet();

        /**
         * Reparses Json data and removes entries from varying JObjects in internal JArray
         * Only used in the case that _jsonDocuments's data buffer exceeds Particle cloud publish limits
         * 
         * @note this method expects that json data is formatted in a specific way and will fail if the formatting changes 
         * 
         * @return String payload -- json data string
         */
        String _recoverDataFromBuffer();
};

#endif
