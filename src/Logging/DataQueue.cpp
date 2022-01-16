#include "DataQueue.h"

DataQueue::DataQueue(String publishHeader,  void (*callback)(String, PublishStatus)) {
	_publishHeader = publishHeader;
	_publishCallback = callback;
	_lastPublish = 0;
}

DataQueue::~DataQueue(){}

void DataQueue::begin() {
	_publishQueue = &(PublishQueuePosix::instance());
	_publishQueue->setup();
	_publishQueue->withRamQueueSize(RAM_QUEUE_EVENT_COUNT);
	_jsonDocumentInit();
}

void DataQueue::handle() {
	_publishQueue->loop();
}

JsonObject DataQueue:: createDataObject() {
	JsonObject object = _jsonDocument["l"].as<JsonArray>().createNestedObject();
	object["t"] = Time.now();
	return object.createNestedObject("d");
}

void DataQueue::publish(String event, PublishFlags flag1, PublishFlags flag2) {
	unsigned long currentPublish = millis() / 1000;
	String payload = _jsonBufferGet();
	PublishStatus status;

	if (getDataSize() > unsigned(JSON_BUFFER_SIZE)) {
		payload =  _recoverDataFromBuffer();
		status = DataBufferOverflow;
	} else if (currentPublish - _lastPublish <= 1) {
		status = PublishingAtMaxFrequency;
	} else {
		status = Normal;
	}

	_lastPublish = currentPublish;

	// publish payload
	if (PUBLISH_EN) {
		_publishQueue->publish(event, payload, flag1, flag2);
	}

	_jsonDocumentRefresh();
	_publishCallback(payload, status);
}

size_t DataQueue::getBufferSize() {
	return JSON_BUFFER_SIZE;
}

size_t DataQueue::getDataSize() {
	return measureJson(_jsonDocument);
}

void DataQueue::_jsonDocumentRefresh() {
	_jsonDocument.clear();
    _jsonDocumentInit();
}

String DataQueue::_jsonBufferGet() {
	char buf[JSON_BUFFER_SIZE + JSON_OVERFLOW_CAPACITY];
	memset(buf, 0, sizeof(buf));
	serializeJson(_jsonDocument, buf);
	return String(buf);
}

void DataQueue::_jsonDocumentInit() {
    _jsonDocument["v"] = _publishHeader;
	_jsonDocument.createNestedArray("l");
}

size_t DataQueue::getNumEventsInQueue() {
	return _publishQueue->getNumEvents();
}

bool DataQueue::isCacheFull() {
	return _publishQueue->getNumEvents() >= _publishQueue->getFileQueueSize();
}

String DataQueue::_recoverDataFromBuffer() {
	StaticJsonDocument<JSON_BUFFER_SIZE + JSON_OVERFLOW_CAPACITY> doc;
	unsigned nextArrayRemovalIndex = 0;
	unsigned nextObjectRemovalIndex = 0;
	unsigned dataSize = getDataSize();

	while (dataSize > (unsigned)JSON_BUFFER_SIZE) {
		JsonArray dataArray = doc["l"].as<JsonArray>();
		unsigned arrayCount = dataArray.size();
		unsigned arrayRemovalIndex = arrayCount != 0 ? nextArrayRemovalIndex++ % arrayCount : 0;

		if (arrayCount == 0)
			break;

		JsonObject object = dataArray[arrayRemovalIndex]["d"].as<JsonObject>();
		unsigned objectCount = object.size();

		// if there is only one key value pair in data object, then we remove the whole object
		// otherwise, we remove the key value pair at objectRemovalIndex
		if (objectCount <= 1) {
			dataArray.remove(arrayRemovalIndex);
		} else {
			unsigned objectRemovalIndex = nextObjectRemovalIndex++ % object.size();
			JsonObject::iterator it = object.begin();
			it += objectRemovalIndex;
			object.remove(it->key());
		}

		dataSize = getDataSize();
	}

	return _jsonBufferGet();
}