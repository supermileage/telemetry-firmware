#include "DataQueue.h"

DataQueue::DataQueue(String publishHeader,  void (*callback)(String, PublishStatus)) {
	_publishHeader = publishHeader;
	_init();
	_publishCallback = callback;
	_lastPublish = 0;
}

void DataQueue::loop() {
	_publishQueue->loop();
}

void DataQueue::publish(String event, PublishFlags flag1, PublishFlags flag2) {
	unsigned long currentPublish = millis() / 1000;
	String payload = _writerGet();

	// assign publishing status
	PublishStatus status;
	if (_writer->dataSize() > unsigned(JSON_WRITER_BUFFER_SIZE)) {
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

void DataQueue::_init() {
	_publishQueue = &(PublishQueuePosix::instance());
	_publishQueue->setup();
	_publishQueue->withRamQueueSize(RAM_QUEUE_EVENT_COUNT);
	_writerInit();
}

String DataQueue::_recoverDataFromBuffer() {
	unsigned nextArrayRemovalIndex = 0;
	unsigned nextObjectRemovalIndex = 0;
	String payload = "";

	while (_writer->dataSize() > unsigned(JSON_WRITER_BUFFER_SIZE)) {
		JSONValue obj = JSONValue::parseCopy(_buf);

		DEBUG_SERIAL_LN("Complete JSON string to be reparsed : " + String(_buf));

		_writerInit();
		
		// iterate to data array inside of main JObject
		JSONObjectIterator outerIterator(obj);
		while (outerIterator.name() != "l")
			outerIterator.next();
		JSONArrayIterator arrayIterator(outerIterator.value());

		unsigned arrayCount = arrayIterator.count();
		if (arrayCount == 0)
			return _writerGet();

		bool oneObjectInArray = arrayCount == 1;
		unsigned arrayRemovalIndex = nextArrayRemovalIndex++ % arrayCount;

		// LEVEL: Data array inside of main JObject
		while (arrayIterator.next()) {
			bool removeDataFromCurrentObject = (arrayIterator.count() == arrayRemovalIndex) || oneObjectInArray;
			unsigned objectRemovalIndex = 0;

			// check if current JObject is holding any data
			JSONObjectIterator innerPeekingIterator(arrayIterator.value());
			while (innerPeekingIterator.name() != "d")
				innerPeekingIterator.next();
			JSONObjectIterator innerMostPeekingIterator(innerPeekingIterator.value());
			unsigned currentObjectDataCount = innerMostPeekingIterator.count();
			
			// don't add data if current JObject has no data or if it has one element and we're going to remove it anyways
			if (currentObjectDataCount == 0 || (removeDataFromCurrentObject && innerMostPeekingIterator.count() == 1))
				continue;
			
			// varies object data removal index
			if (removeDataFromCurrentObject)
			 	objectRemovalIndex = nextObjectRemovalIndex++ % currentObjectDataCount;

			// LEVEL: JObject which carries timestamp and inner-most JObject
			JSONObjectIterator innerIterator(arrayIterator.value());
			_writer->beginObject();
			while (innerIterator.next()) {
				_writer->name(String(innerIterator.name()));
				if (innerIterator.name() == "t") {
					// add timestamp
					_writer->value(innerIterator.value().toInt());
				} else {
					// LEVEL: inner-most JObject which carries event data
					JSONObjectIterator innerMostIterator(innerIterator.value());
					_writer->beginObject();
					while (innerMostIterator.next()) {
						// remove last data entry from jobject if we are supposed to
						if (removeDataFromCurrentObject && innerMostIterator.count() == objectRemovalIndex)
							continue;
							
						_writer->name(String(innerMostIterator.name()))
							.value(String(innerMostIterator.value().toString()));
					}
					_writer->endObject();
				}
			}
			_writer->endObject();
		}

		payload = _writerGet();
	}
	return payload;
}