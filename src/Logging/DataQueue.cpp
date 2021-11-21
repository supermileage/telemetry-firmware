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
		status = DataBufferOverflow;
		// reduce data in buffer until it fits within json writer buffer size
		unsigned i = 0;
		while (_writer->dataSize() > unsigned(JSON_WRITER_BUFFER_SIZE)) {
			_recoverDataFromBuffer(i++);
			payload = _writerGet();
		}
	} else if (currentPublish - _lastPublish <= 1) {
		status = PublishingAtMaxFrequency;
	} else {
		status = Normal;
	}

	_lastPublish = currentPublish;

	// get payload and publish
	if (PUBLISH_ENABLED) {
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

size_t DataQueue::getNumEventsInQueue() {
	return _publishQueue->getNumEvents();
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

void DataQueue::_init() {
	_publishQueue = &(PublishQueuePosix::instance());
	_publishQueue->setup();
	_publishQueue->withRamQueueSize(RAM_QUEUE_EVENT_COUNT);
	_writerInit();
}

void DataQueue::_recoverDataFromBuffer(unsigned removalIndex) {
	// parse and create copy of data in buffer
	JSONValue obj = JSONValue::parseCopy(_buf);
	JSONObjectIterator outerIterator(obj);

	// for testing
	if (DEBUG_SERIAL_ENABLE)
		DEBUG_SERIAL_LN("Complete JSON string to be reparsed : " + String(_buf));

	// reinitialize writer (opens JObject and JArray in data buffer)
	_writerInit();

	while (outerIterator.next()) {
		if (outerIterator.value().isArray()) {
			JSONArrayIterator arrayIterator(outerIterator.value());
			bool oneObjectInArray = arrayIterator.count() == 1;

			// LEVEL: Data array inside of main JObject
			while (arrayIterator.next()) {
				JSONObjectIterator peekingIterator(arrayIterator.value());

				// don't record current object if it's not holding any data
				bool currentObjectHasNoData = false;
				while (peekingIterator.next()) {
					if (peekingIterator.value().isObject()) {
						JSONObjectIterator checkerIterator(peekingIterator.value());
						currentObjectHasNoData = checkerIterator.count() == 0;
					}
				}
				if (currentObjectHasNoData)
					continue;

				// remove data from current object if we are at removal index in array, or if array only has one JObject
				bool removeDataFromCurrentObject = (arrayIterator.count() == removalIndex) || oneObjectInArray;
				JSONObjectIterator innerIterator(arrayIterator.value());
				
				// LEVEL: JObject which carries timestamp and inner-most JObject
				_writer->beginObject();
				while (innerIterator.next()) {
					_writer->name(String(innerIterator.name()));
					
					if (innerIterator.count() == 1) {
						// add timestamp
						_writer->value(innerIterator.value().toInt());
					} else {
						// get inner-most JObject and add all its data
						JSONObjectIterator innerestIterator(innerIterator.value());

						// LEVEL: inner-most JObject which carries event data
						_writer->beginObject();
						while (innerestIterator.next()) {
							// remove last data entry from jobject if we are supposed to
							if (removeDataFromCurrentObject && innerestIterator.count() == 0)
								break;

							_writer->name(String(innerestIterator.name()))
								.value(String(innerestIterator.value().toString()));
						}
						_writer->endObject();
					}
				}
				_writer->endObject();
			}
		}
	}
	// final closing of JArray and jobject handled by _writerGet() in publish()
}