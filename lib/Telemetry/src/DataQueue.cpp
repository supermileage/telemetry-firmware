#include "DataQueue.h"

#include "Particle.h"

DataQueue::DataQueue() {
	_init();
}

void DataQueue::add(String id, int value) {
	_writer->beginObject()
    	.name("t").value(id)
    	.name("d").value(value)
    .endObject();
}

void DataQueue::add(String id, String value) {
	_writer->beginObject()
    	.name("t").value(id)
    	.name("d").value(value)
    .endObject();
}

void DataQueue::add(String id, float value) {
	_writer->beginObject()
    	.name("t").value(id)
    	.name("d").value(value)
    .endObject();
}

void DataQueue::loop() {
	_publishQueuePosix->loop();
}

String DataQueue::publish(String event, PublishFlags flag1, PublishFlags flag2) {
	String payload = _writerGet();
	bool operationSuccess = PublishQueuePosix::instance().publish(event, payload, flag1, flag2);
	size_t numEvents = PublishQueuePosix::instance().getNumEvents();
	_writerRefresh();
	Serial.println("Posix Directory Path: " + (String)PublishQueuePosix::instance().getDirPath());
	Serial.println("Publish Operation Success: " + (String)(operationSuccess ? "true" : "false"));
	Serial.println("Number of events Queued: " + String(numEvents));
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

	_writer->beginObject()
		.name("time").value((int)Time.now())
		.name("d").beginArray();
}

void DataQueue::_init() {
	char *filePath = new char [9];
	strcpy(filePath, "/myqueue");

	_publishQueuePosix = &(PublishQueuePosix::instance());
	PublishQueuePosix::instance().setup();
	PublishQueuePosix::instance().withRamQueueSize(RAM_QUEUE_EVENT_COUNT).withFileQueueSize(FILE_QUEUE_EVENT_COUNT)
		.withDirPath(filePath);

	// Initialize Queue
	_writerInit();
}