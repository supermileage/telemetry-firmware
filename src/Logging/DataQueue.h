#ifndef _DATAQUEUE_H_
#define _DATAQUEUE_H_

// Particle cloud publish size limit is 1024B
#define JSON_BUFFER_SIZE 1024
#define JSON_OVERFLOW_CAPACITY 256
#define RAM_QUEUE_EVENT_COUNT 8

// JsonDocument size sets the maximum allocated memory for the object in its entirety, not just the json string
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
 * 
 * @note formatting is specified in the methods _jsonDocumentInit and createDataObject.
 * If you wish to change the formatting, you must also modify (or comment out) _recoverDataFromBuffer
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
        StaticJsonDocument<JSON_DOCUMENT_SIZE> _jsonDocument;
        PublishQueuePosix* _publishQueue;
        void (*_publishCallback)(String, PublishStatus);
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
