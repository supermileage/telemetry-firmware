#include "DataQueue.h"
#include "Particle.h"

#include "settings.h"

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
	_publishQueue->loop();
}

String DataQueue::publish(String event, PublishFlags flag1, PublishFlags flag2) {
	String payload = _writerGet();
	_publishQueue->publish(event, payload, flag1, flag2);
	_writerRefresh();

	DEBUG_SERIAL("Number of events Queued: " + String(_publishQueue->getNumEvents()));
	
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
	_publishQueue = &(PublishQueuePosix::instance());
	_publishQueue->setup();
	_publishQueue->withRamQueueSize(RAM_QUEUE_EVENT_COUNT);
	_writerInit();

	DEBUG_SERIAL("Posix Directory Path: " + (String)_publishQueue->getDirPath());
}