#include "DataQueue.h"

DataQueue::DataQueue(String publishHeader,  void (*callback)(PublishStatus)) {
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

	// assign publishing status
	PublishStatus status;
	if (currentPublish - _lastPublish <= 1) {
		status = PublishingAtMaxFrequency;
	} else if (_writer->dataSize() > JSON_WRITER_BUFFER_SIZE) {
		status = DataBufferOverflow;
	} else {
		status = Normal;
	}

	_lastPublish = currentPublish;

	// publish and refresh writer
	String payload = _writerGet();
	_publishQueue->publish(event, payload, flag1, flag2);
	_writerRefresh();

	// handle serial publishing behavior
	_publishCallback(status);
	DEBUG_SERIAL_LN(payload);
	DEBUG_SERIAL_LN("Number of events Queued: " + String(_publishQueue->getNumEvents()));
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
	return _writer->bufferSize();
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

void DataQueue::_init() {
	_publishQueue = &(PublishQueuePosix::instance());
	_publishQueue->setup();
	_publishQueue->withRamQueueSize(RAM_QUEUE_EVENT_COUNT);
	_writerInit();
}