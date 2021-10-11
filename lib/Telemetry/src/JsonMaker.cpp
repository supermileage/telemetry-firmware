#include "JsonMaker.h"


JsonMaker::JsonMaker() {
    _init();
}

void JsonMaker::_init() {
    memset(_buf, 0, sizeof(_buf));
    this->_writer = new JSONBufferWriter(_buf, sizeof(_buf) - 1);

    _writer->beginObject();
    _writer->name("time").value((int)Time.now());
    _writer->name("d").beginArray();
}

void JsonMaker::add(String id, int value) {
    _writer->beginObject();
    _writer->name("t").value(id);
    _writer->name("d").value(value);
    _writer->endObject();
}

void JsonMaker::add(String id, String value) {
    _writer->beginObject();
    _writer->name("t").value(id);
    _writer->name("d").value(value);
    _writer->endObject();
}

void JsonMaker::add(String id, float value) {
    _writer->beginObject();
    _writer->name("t").value(id);
    _writer->name("d").value(value);
    _writer->endObject();
}

String JsonMaker::get() {
    _writer->endArray();
    _writer->endObject();
    return String(_buf);
}

void JsonMaker::refresh() {
    if(this->_writer != NULL) delete this->_writer;
    _init();
}


