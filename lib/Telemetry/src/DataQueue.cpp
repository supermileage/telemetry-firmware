#include "DataQueue.h"

DataQueue::DataQueue() {
	_init();
}

void DataQueue::add(String id, int value) {
	_jsonMaker->add(id, value);
}

void DataQueue::add(String id, String value) {
	_jsonMaker->add(id, value);
}

void DataQueue::add(String id, float value) {
	_jsonMaker->add(id, value);
}

String DataQueue::publish(String event, PublishFlags flag1, PublishFlags flag2) {
	String payload = _jsonMaker->get();
	_publishQueue->publish(event, payload, flag1, flag2);
	_jsonMaker->refresh();
    return payload;
}

String DataQueue::resetData() {
    String payload = _jsonMaker->get();
    _jsonMaker->refresh();
    return payload;
}

void DataQueue::_writerRefresh() {
	if(this->_writer != NULL) delete this->_writer;
    _writerrInit();
}

String DataQueue::_writerGet() {
	_writer->endArray();
    _writer->endObject();
    return String(_buf);
}

void DataQueue::_writerInit() {
    memset(_buf, 0, sizeof(_buf));
	this->_writer = new JSONBufferWriter(_buf, sizeof(_buf) - 1);

	_writer->beginObject();
	_writer->name("time").value((int)Time.now());
	_writer->name("d").beginArray();
}

void DataQueue::_init() {
	// Initalize JSON writer
	_writerInit();

	// Initialize Queue
	_publishQueue = new PublishQueueAsyncRetained(_publishQueueRetainedBuffer, uint16_t(sizeof(_publishQueueRetainedBuffer)));
	_jsonMaker = new JsonMaker();
	_publishQueue->setup();
}