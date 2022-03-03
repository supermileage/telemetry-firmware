#ifndef _THE_THING_H_
#define _THE_THING_H_

#include <queue>
#include <stdlib.h>

#include "midi-parser.h"
#include "CanInterface.h"

using namespace can;

const char MIDI_FILE[] = "4D546864 00000006 00000001 00604D54 726B0000 005A00FF 03010000 FF580404 02240800 FF580404 02240800 903C6418 903D6418 903E6418 903F6418 90406418 90416418 803C4000 803D4000 803E4000 803F4000 80404000 80414030 903C6400 90406481 40803C40 00804040 00FF2F00";

// Class which transposes midi files to can messages
class TheThing : public Handleable {
	public:

		TheThing(CanInterface& can);

		~TheThing();

		void begin() override;

		void handle() override;

		void start(const uint8_t* input, const uint32_t size);

		void stop();

	private:
		// midi event with delay before execution (in midi time uints)
		// and note # (status of -1 denotes end of track)
		struct MidiEvent {
			int64_t time;
			uint32_t status;
			uint8_t note;
		};
		const CanMessage DEFAULT_MESSAGE = { CAN_ACC_OPERATION, 0x1, { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } };
		CanInterface& _can;
		CanMessage _nextMessage;
		uint64_t _nextMessageTime = 0;
		uint32_t _microsPerUnit;
		uint16_t _bpm;
		bool _play = false;
		std::queue<MidiEvent> _midiEvents;

		void _parse(const uint8_t* input, const uint32_t size);

		void _playInternal();

		void _buildNextCanMessage();

		void _parseHeader(midi_parser* parser);

		void _parseTrackMidi(midi_parser* parser);

		void _parseTrackMeta(midi_parser* parser);

		uint8_t* _convertString(const char *buf, uint32_t *length);

		uint8_t _getHexValue(char **bufferPtr);
};

#endif