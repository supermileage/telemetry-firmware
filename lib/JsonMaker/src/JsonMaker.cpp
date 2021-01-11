#include "JsonMaker.h"

/**
 *  Constructor 
 **/
JsonMaker::JsonMaker(){
    // Clear Buffer and JSONBufferWriter object
    memset(buf, 0, sizeof(buf));
    this->writer = new JSONBufferWriter(buf, sizeof(buf) - 1);

    Time.zone(-8);
    init();
}

/**
 * Deletes the JSONBufferWriter object if it is not null and creates
 * a new writer object. The buffer is cleared. Init JSON string.
 **/
void JsonMaker::init(){
    // Destroy previous JSONBufferWriter object, clear mem, and create new one
    if(this->writer != NULL) delete this->writer;
    memset(buf, 0, sizeof(buf));
    this->writer = new JSONBufferWriter(buf, sizeof(buf) - 1);

    writer->beginObject();
    writer->name("time").value((int)Time.now());
    writer->name("d").beginArray();
}

/**
 * Add a new object to the buffer with an int value
 * 
 * @param id: the id for the value
 * @param value: the integer value corresponding to its id
 **/
void JsonMaker::add(String id, int value){
    writer->beginObject();
    writer->name("t").value(id);
    writer->name("d").value(value);
    writer->endObject();
}

/**
 * Adds a new object to the buffer with a string value.
 * 
 * @param id: the id for the value
 * @param value: the string value corresponding to its id
 **/
void JsonMaker::add(String id, String value){
    writer->beginObject();
    writer->name("t").value(id);
    writer->name("d").value(value);
    writer->endObject();
}

/**
 * Adds a new object to the buffer with a string value.
 * 
 * @param id: the id for the value
 * @param value: the float value corresponding to its id
 **/
void JsonMaker::add(String id, float value){
    writer->beginObject();
    writer->name("t").value(id);
    writer->name("d").value(value);
    writer->endObject();
}

/**
 * Ends the JSON object and returns its string representation
 * 
 * @return a JSON string
 **/
String JsonMaker::get(){
    writer->endArray();
    writer->endObject();
    return String(buf);
}

