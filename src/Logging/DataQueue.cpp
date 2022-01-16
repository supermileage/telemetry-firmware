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
	_writerInit();
}

void DataQueue::handle() {
	_publishQueue->loop();
}

void DataQueue::publish(String event, PublishFlags flag1, PublishFlags flag2) {
	unsigned long currentPublish = millis() / 1000;
	String payload = _writerGet();

	// assign publishing status
	PublishStatus status;
	if (getDataSize() > unsigned(JSON_WRITER_BUFFER_SIZE)) {
		payload =  _recoverDataFromBuffer();
		status = DataBufferOverflow;
	} else if (currentPublish - _lastPublish <= 1) {
		status = PublishingAtMaxFrequency;
	} else {
		status = Normal;
	}

	_lastPublish = currentPublish;

	// get payload and publish
	if (PUBLISH_EN) {
		_publishQueue->publish(event, payload, flag1, flag2);
	}

	_writerRefresh();
	_publishCallback(payload, status);
}

String DataQueue::resetData() {
    String payload = _writerGet();
    _writerRefresh();
    return payload;
}

void DataQueue::wrapStart() {
	_writer->beginObject()
		.name("t").value((int)Time.now())
		.name("d").beginObject();
}

void DataQueue::wrapEnd() {
	_writer->endObject().endObject();
}

size_t DataQueue::getBufferSize() {
	return JSON_WRITER_BUFFER_SIZE;
}

size_t DataQueue::getDataSize() {
	return _writer->dataSize();
}

void DataQueue::_writerRefresh() {
	if(this->_writer != NULL) delete this->_writer;
    _writerInit();
}

String DataQueue::_writerGet() {
	_writer->endArray().endObject();
    return String(_buf);
}

void DataQueue::_writerInit() {
    memset(_buf, 0, sizeof(_buf));
	this->_writer = new JSONBufferWriter(_buf, sizeof(_buf) - 1);

	_writer->beginObject().name("v").value(_publishHeader)
		.name("l").beginArray();
}

size_t DataQueue::getNumEventsInQueue() {
	return _publishQueue->getNumEvents();
}

bool DataQueue::isCacheFull() {
	return _publishQueue->getNumEvents() >= _publishQueue->getFileQueueSize();
}

String DataQueue::_recoverDataFromBuffer() {
	StaticJsonDocument<JSON_WRITER_BUFFER_SIZE + JSON_WRITER_OVERFLOW_CAPACITY> doc;	
	unsigned nextArrayRemovalIndex = 0;
	unsigned nextObjectRemovalIndex = 0;
	unsigned dataSize = getDataSize();

	while (dataSize > (unsigned)JSON_WRITER_BUFFER_SIZE) {
		DeserializationError error = deserializeJson(doc, (const char*)_buf);

		if (error) {
			DEBUG_SERIAL_LN("Unable to deserialize Json in string buffer: returning");
			return String(_buf);
		}
		
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

		dataSize = serializeJson(doc, _buf);
	}

	return String(_buf);
}