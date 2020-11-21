#include "JsonMaker.h"
#define BUFF_SIZE 1024

char buf[BUFF_SIZE];
JSONBufferWriter writer(buf, sizeof(buf) - 1); // Create JSONBufferWriter object called writer

JsonMaker::JsonMaker(){
    Time.zone(-8);
    clear();
}

void JsonMaker::clear(){
    memset(buf, 0, sizeof(buf)); //Clear array
    writer.beginObject();
    writer.name("time").value((int)Time.now());
    writer.name("d").beginArray();
}

void JsonMaker::add(String id, int value){
    writer.beginObject();
    writer.name("t").value(id);
    writer.name("d").value(value);
    writer.endObject();
}

String JsonMaker::get(){
    writer.endArray();
    writer.endObject();
    return String(buf);
}

