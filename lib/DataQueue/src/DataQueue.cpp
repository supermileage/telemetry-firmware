#include "DataQueue.h"

DataQueue::DataQueue() {
	SYSTEM_THREAD(ENABLED);

	this->publishQueue = new PublishQueueAsync(publishQueueRetainedBuffer, sizeof(publishQueueRetainedBuffer));
	this->jsonMaker = new JsonMaker();

	publishQueue->setup();
}

void DataQueue::add(String id, int value) {
	jsonMaker->add(id, value);
}

void DataQueue::add(String id, String value) {
	jsonMaker->add(id, value);
}

void DataQueue::add(String id, float value) {
	jsonMaker->add(id, value);
}

void DataQueue::publish(String event, PublishFlags flag1, PublishFlags flag2) {
	String payload = jsonMaker->get();
	publishQueue->publish(event, payload, flag1, flag2);
}
