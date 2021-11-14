#include "DataQueue.h"

DataQueue::DataQueue(String publishHeader) {
	_publishHeader = publishHeader;
	_init();
}

void DataQueue::wrapStart() {
	_writer->beginObject()
		.name("t").value((int)Time.now())
		.name("d").beginObject();
}

void DataQueue::wrapEnd() {
	_writer->endObject().endObject();
}

void DataQueue::loop() {
	_publishQueue->loop();
}

String DataQueue::publish(String event, PublishFlags flag1, PublishFlags flag2) {
	String payload = _writerGet();
	_publishQueue->publish(event, payload, flag1, flag2);
	_writerRefresh();

	// DEBUG_SERIAL_LN("Number of events Queued: " + String(_publishQueue->getNumEvents()));
	
    return payload;
}

String DataQueue::resetData() {
    String payload = _writerGet();
    _writerRefresh();
    return payload;
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

size_t DataQueue::getBufferSize() {
	return _writer->bufferSize();
}

size_t DataQueue::getDataSize() {
	return _writer->dataSize();
}

void DataQueue::_init() {
	_publishQueue = &(PublishQueuePosix::instance());
	_publishQueue->setup();
	_publishQueue->withRamQueueSize(RAM_QUEUE_EVENT_COUNT);
	_writerInit();

	// DEBUG_SERIAL_LN("Posix Directory Path: " + (String)_publishQueue->getDirPath());
}