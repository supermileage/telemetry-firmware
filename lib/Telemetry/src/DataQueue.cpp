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

void DataQueue::_init() {
	_publishQueue = new PublishQueueAsyncRetained(_publishQueueRetainedBuffer, uint16_t(sizeof(_publishQueueRetainedBuffer)));
	_jsonMaker = new JsonMaker();
	_publishQueue->setup();
}