#include "DataQueue.h"

DataQueue::DataQueue(String vehicleName,  void (*callback)(String, PublishData)) {
	_vehicleName = vehicleName;
	_publishCallback = callback;
	_lastPublish = 0;
}

DataQueue::~DataQueue(){}

void DataQueue::begin() {
	_publishQueue = &(PublishQueuePosix::instance());
	_publishQueue->setup();
	_publishQueue->withRamQueueSize(RAM_QUEUE_EVENT_COUNT);
	_jsonDocumentInit();
	_initialized = true;
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
	PublishData data = { Normal, _jsonDocument.memoryUsage() };
	String payload = _jsonBufferGet();

	if (getDataSize() >= unsigned(JSON_BUFFER_SIZE)) {
		payload =  _recoverDataFromBuffer();
		data.status = DataBufferOverflow;
	} else if (currentPublish - _lastPublish <= 1) {
		data.status = PublishingAtMaxFrequency;
	} else if (_jsonDocument.overflowed()) {
		data.status = JsonDocumentOverflow;
	}

	_lastPublish = currentPublish;

	// publish payload
	if (PUBLISH_EN) {
		_publishQueue->publish(event, payload, flag1, flag2);
	}

	// clear and reset reinitialize json document and invoke publish callback
	_jsonDocumentRefresh();
	_publishCallback(payload, data);
}

size_t DataQueue::getBufferSize() {
	return JSON_BUFFER_SIZE;
}

size_t DataQueue::getDataSize() {
	return measureJson(_jsonDocument);
}

size_t DataQueue::getMemoryUsage() {
	return _jsonDocument.memoryUsage();
}

bool DataQueue::verifyJsonStatus() {
	return !_jsonDocument.overflowed();
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
    _jsonDocument["v"] = _vehicleName;
	_jsonDocument.createNestedArray("l");
}

size_t DataQueue::getNumEventsInQueue() {
	return _publishQueue->getNumEvents();
}

bool DataQueue::isCacheFull() {
	return _publishQueue->getNumEvents() >= _publishQueue->getFileQueueSize();
}

String DataQueue::_recoverDataFromBuffer() {
	unsigned dataSize = getDataSize();
	unsigned nextArrayRemovalIndex = 0;
	unsigned nextObjectRemovalIndex = 0;

	// each loop: removes key-value pair from one of the JsonObjects in array, shifts removal indices by one
	while (dataSize >= (unsigned)JSON_BUFFER_SIZE) {
		JsonArray dataArray = _jsonDocument["l"].as<JsonArray>();
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
