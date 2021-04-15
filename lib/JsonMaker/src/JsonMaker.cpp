#include "JsonMaker.h"

/**
 * Constructor 
 **/
JsonMaker::JsonMaker() {
    init();
}

/**
 * Initializes the JSON object.
 * */
void JsonMaker::init() {
    memset(buf, 0, sizeof(buf));
    this->writer = new JSONBufferWriter(buf, sizeof(buf) - 1);

    writer->beginObject();
    writer->name("time").value((int)Time.now());
    writer->name("d").beginArray();
}

/**
 * Adds a new object to the buffer with an int value.
 * 
 * @param id The id for the value.
 * @param value An integer value to be stored.
 **/
void JsonMaker::add(String id, int value) {
    writer->beginObject();
    writer->name("t").value(id);
    writer->name("d").value(value);
    writer->endObject();
}

/**
 * Adds a new object to the buffer with a string value.
 * 
 * @param id The id for the value.
 * @param value A string value to be stored.
 **/
void JsonMaker::add(String id, String value) {
    writer->beginObject();
    writer->name("t").value(id);
    writer->name("d").value(value);
    writer->endObject();
}

/**
 * Adds a new object to the buffer with a float value.
 * 
 * @param id The id for the value.
 * @param value A float value to be stored.
 **/
void JsonMaker::add(String id, float value) {
    writer->beginObject();
    writer->name("t").value(id);
    writer->name("d").value(value);
    writer->endObject();
}

/**
 * @return A JSON string representing the data stored in the
 *         JSON object.
 * */
String JsonMaker::get() {
    writer->endArray();
    writer->endObject();
    return String(buf);
}

/**
 * Removes the data stored in the JSON object and reinitializes
 * another buffer writer.
 * */
void JsonMaker::refresh() {
    if(this->writer != NULL) delete this->writer;
    init();
}


