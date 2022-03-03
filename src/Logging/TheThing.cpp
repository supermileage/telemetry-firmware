#include "TheThing.h"

#define MICROS_PER_MINUTE 60000000
// note number of middle C
#define MIDI_C3 60

TheThing::TheThing(CanInterface& can) : _can(can) { }

TheThing::~TheThing() { }

void TheThing::start(const uint8_t *input, const uint32_t size) {
	_play = true;
	_buildNextCanMessage();
}

void TheThing::stop() {
	_play = false;
}

void TheThing::begin() {
	uint32_t length = sizeof(MIDI_FILE) / sizeof(MIDI_FILE[0]);
	uint8_t* buffer = _convertString(MIDI_FILE, &length);
	_parse(buffer, length);
}

void TheThing::handle() {
	if (_play) {
		_playInternal();
	}
}

void TheThing::_buildNextCanMessage() {
	_nextMessage = DEFAULT_MESSAGE;
	_nextMessageTime = micros() + (_midiEvents.front().time * _microsPerUnit);

	uint8_t i = 1;

	while (i < 8) {
		if (_midiEvents.front().status == MIDI_META_END_OF_TRACK)
			break;

		MidiEvent midiEvent = _midiEvents.front();
		_midiEvents.pop();
		++_nextMessage.dataLength;
		_nextMessage.data[i] = ((midiEvent.note - MIDI_C3) << 1) | (midiEvent.status == MIDI_STATUS_NOTE_ON);

		if (_midiEvents.front().time != midiEvent.time)
			break;
	}
}

void TheThing::_playInternal() {
	if (_nextMessage.dataLength == 1) {
		_play = false;
		return;
	}

	if (_nextMessageTime <= micros()) {
		_can.sendMessage(_nextMessage);
		_buildNextCanMessage();
	}
}

void TheThing::_parse(const uint8_t *input, const uint32_t size) {
	if (size == 0)
		return;

	midi_parser parser;
	parser.state = MIDI_PARSER_INIT;
	parser.in = input;
	parser.size = size;

	bool breakFromLoop = false;

	while (true) {
		midi_parse(&parser);

		switch (parser.state) {
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

		if (breakFromLoop)
			break;
	}
}

void TheThing::_parseHeader(midi_parser* parser) {
	_microsPerUnit = (MICROS_PER_MINUTE / _bpm) / parser->header.time_division;
}

void TheThing::_parseTrackMidi(midi_parser* parser) {
	MidiEvent newEvent = { parser->vtime, parser->midi.status, parser->midi.param1 };
	_midiEvents.push(newEvent);
}

void TheThing::_parseTrackMeta(midi_parser* parser) {
	if (parser->meta.type == MIDI_META_END_OF_TRACK) {
		MidiEvent newEvent = { parser->vtime, MIDI_META_END_OF_TRACK, 0 };
		_midiEvents.push(newEvent);
	}
}

uint8_t _getHexValue(char **bufferPtr) {
	while (isspace(**bufferPtr))
		(*bufferPtr)++;
	char nextByte[2] = { **bufferPtr, *((*bufferPtr) + 1) };
	*bufferPtr += 2;
	char* dummyPtr = nextByte;
	return strtoul(nextByte, &dummyPtr, 16);
}

uint8_t* TheThing::_convertString(const char* buf, uint32_t *length) {
	// get size of char buffer ("0BF45601 " == 9 characters -> 4 bytes in buffer)
	uint8_t* output = new uint8_t[((*length + 1) / 9) * 4];
	uint8_t* s_ptr;
	for (s_ptr = output; *buf; s_ptr++) {
		*s_ptr = _getHexValue((char**)&buf);
	}
	*length = s_ptr - output;
  	return output;
}