#include "JsonMaker.h"

JsonMaker::JsonMaker() {
    init();
}

void JsonMaker::add(String id, int value) {
    writer->beginObject();
    writer->name("t").value(id);
    writer->name("d").value(value);
    writer->endObject();
}

void JsonMaker::add(String id, String value) {
    writer->beginObject();
    writer->name("t").value(id);
    writer->name("d").value(value);
    writer->endObject();
}

void JsonMaker::add(String id, float value) {
    writer->beginObject();
    writer->name("t").value(id);
    writer->name("d").value(value);
    writer->endObject();
}

String JsonMaker::get() {
    writer->endArray();
    writer->endObject();
    return String(buf);
}

void JsonMaker::refresh() {
    if(this->writer != NULL) delete this->writer;
    memset(buf, 0, sizeof(buf));
    this->writer = new JSONBufferWriter(buf, sizeof(buf) - 1);

    writer->beginObject();
    writer->name("time").value((int)Time.now());
    writer->name("d").beginArray();
}

void JsonMaker::init() {
    memset(buf, 0, sizeof(buf));
    this->writer = new JSONBufferWriter(buf, sizeof(buf) - 1);
    Time.zone(-8);

    writer->beginObject();
    writer->name("time").value((int)Time.now());
    writer->name("d").beginArray();
}
