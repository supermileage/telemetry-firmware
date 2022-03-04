#include "CanAccessoriesMidiPlayer.h"

#define MICROS_PER_MINUTE 60000000
// note number of middle C
#define MIDI_C3 60

CanAccessoriesMidiPlayer::CanAccessoriesMidiPlayer(CanInterface& can) : _can(can) { }

CanAccessoriesMidiPlayer::~CanAccessoriesMidiPlayer() { }

void CanAccessoriesMidiPlayer::start(uint16_t bpm) {
	if (_play) {
		stop();
	}

	_bpm = bpm;
	_play = true;

	uint32_t length = sizeof(MIDI_FILE) / sizeof(MIDI_FILE[0]);
	uint8_t* buffer = _convertString(MIDI_FILE, &length);
	
	_parse(buffer, length);
	delete[] buffer;
	DEBUG_SERIAL_LN("Queue has " + String(_midiEvents.size()) + " events");
	_buildNextCanMessage();
	DEBUG_SERIAL_LN("Queue has " + String(_midiEvents.size()) + " events");
}

void CanAccessoriesMidiPlayer::stop() {
	_play = false;

	// clear event queue
	std::queue<MidiEvent> empty;
	std::swap(_midiEvents, empty);
}

void CanAccessoriesMidiPlayer::begin() { }

void CanAccessoriesMidiPlayer::handle() {
	if (_play) {
		_playInternal();
	}
}

void CanAccessoriesMidiPlayer::_buildNextCanMessage() {
	_nextMessage = DEFAULT_MESSAGE;
	_nextMessageTime = micros() + (_midiEvents.front().time * _microsPerUnit);

	for (uint8_t i = 1; i < 8; i++) {
		if (_midiEvents.front().status == MIDI_META_END_OF_TRACK)
			break;

		MidiEvent midiEvent = _midiEvents.front();
		_midiEvents.pop();
		++_nextMessage.dataLength;
		_nextMessage.data[i] = ((midiEvent.note - MIDI_C3) << 1) | (midiEvent.status == MIDI_STATUS_NOTE_ON);

		DEBUG_SERIAL_LN("Adding midi event -- note: " + String(midiEvent.note) + " -- time: " + String((long)midiEvent.time) + " -- status: " + String(midiEvent.status));

		// if the next midi event has a time of 0, it means it occurs in unison with the current
		if (_midiEvents.front().time != 0)
			break;
	}

	DEBUG_SERIAL("Can message added with value : { " + String(_nextMessage.id) + ", 0x" + String(_nextMessage.dataLength) + ", { ");

	for (uint8_t i = 0; i < _nextMessage.dataLength; i++) {
		DEBUG_SERIAL("0x" + String(_nextMessage.data[i]));
		if (i != (_nextMessage.dataLength - 1))
			DEBUG_SERIAL(", ");
		else
			DEBUG_SERIAL(" } }\n");
	}

	DEBUG_SERIAL_LN("Queue has " + String(_midiEvents.size()) + " events");
}

void CanAccessoriesMidiPlayer::_playInternal() {
	if (_nextMessage.dataLength == 1) {
		stop();
		return;
	}

	if (_nextMessageTime <= micros()) {
		_can.sendMessage(_nextMessage);
		_buildNextCanMessage();
	}
}

void CanAccessoriesMidiPlayer::_parse(const uint8_t *input, const uint32_t size) {
	if (size == 0)
		return;

	midi_parser parser;
	parser.state = MIDI_PARSER_INIT;
	parser.in = input;
	parser.size = size;

	bool breakFromLoop = false;
	enum midi_parser_status status;

	while (true) {
		status = midi_parse(&parser);

		switch (status) {
			case MIDI_PARSER_EOB:
				breakFromLoop = TRUE;
				break;
			case MIDI_PARSER_ERROR:
				breakFromLoop = TRUE;
				break;
			case MIDI_PARSER_INIT:
				break;
			case MIDI_PARSER_HEADER:
				_parseHeader(&parser);
				break;
			case MIDI_PARSER_TRACK:
				break;
			case MIDI_PARSER_TRACK_MIDI:
				_parseTrackMidi(&parser);
				break;
			case MIDI_PARSER_TRACK_META:
				_parseTrackMeta(&parser);
				break;
			case MIDI_PARSER_TRACK_SYSEX:
				break;
			default:
				breakFromLoop = TRUE;
				break;
		}

		if (breakFromLoop) {
			return;
		}
	}
}

void CanAccessoriesMidiPlayer::_parseHeader(midi_parser* parser) {
	_microsPerUnit = (MICROS_PER_MINUTE / _bpm) / parser->header.time_division;
	DEBUG_SERIAL_LN("Micros per time unit set to : " + String(parser->header.time_division));
}

void CanAccessoriesMidiPlayer::_parseTrackMidi(midi_parser* parser) {
	MidiEvent newEvent = { parser->vtime, parser->midi.status, parser->midi.param1 };
	DEBUG_SERIAL_LN("Midi event added with time: " + String((long)parser->vtime));
	_midiEvents.push(newEvent);
}

void CanAccessoriesMidiPlayer::_parseTrackMeta(midi_parser* parser) {
	if (parser->meta.type == MIDI_META_END_OF_TRACK) {
		DEBUG_SERIAL_LN("End of track event added with time: " + String((long)parser->vtime));
		MidiEvent newEvent = { parser->vtime, MIDI_META_END_OF_TRACK, 0 };
		_midiEvents.push(newEvent);
	}
}

// Dynamically allocates memory for returned buffer: Make sure to delete uint8_t buffer once you are finished using it 
uint8_t* CanAccessoriesMidiPlayer::_convertString(const char* buf, uint32_t *length) {
	// get size of char buffer ("0BF45601 " == 9 characters -> 4 bytes in buffer)
	int padding = (*length % 8) / 2;
	uint8_t* output = new uint8_t[(((*length + 1) / 9) * 4) + padding];
	uint8_t* s_ptr;

	DEBUG_SERIAL_LN("Predicted length : " + String((((*length + 1) / 9) * 4) + padding));

	// buffer pointer is incremented in getHexValue
	for (s_ptr = output; *buf; s_ptr++) {
		*s_ptr = _getHexValue((char**)&buf);
	}

	*length = s_ptr - output;
	DEBUG_SERIAL_LN("Actual length : " + String(*length));
  	return output;
}

uint8_t CanAccessoriesMidiPlayer::_getHexValue(char **bufferPtr) {
	while (isspace(**bufferPtr))
		(*bufferPtr)++;
	char nextByte[2] = { **bufferPtr, *((*bufferPtr) + 1) };
	*bufferPtr += 2;
	char* dummyPtr = nextByte;
	return strtoul(nextByte, &dummyPtr, 16);
}