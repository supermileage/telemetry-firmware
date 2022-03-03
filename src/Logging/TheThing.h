#ifndef _THE_THING_H_
#define _THE_THING_H_

#include <queue>
#include <stdlib.h>

#include "midi-parser.h"
#include "CanInterface.h"

using namespace can;

const char MIDI_FILE[] = "4D546864 00000006 00000001 00604D54 726B0000 00D900FF 03010000 FF580404 02240800 FF580404 02240800 90406418 80404018 90406418 80404018 90406418 80404018 90406418 80404018 90406418 80404018 90406418 80404018 90406418 80404018 90406418 80404018 90406418 80404018 90406418 80404018 90406418 80404018 90406418 80404018 90406418 80404018 90406418 80404018 90406418 80404018 90406418 80404018 903C6400 903F6460 803C4000 803F4060 903C6400 903F6460 803C4000 803F4060 903C6400 903F6460 803C4000 803F4060 903C6400 903F6460 803C4000 803F4000 FF2F00";

// Class which transposes midi files to can messages
class TheThing : public Handleable {
	public:

		TheThing(CanInterface& can);

		~TheThing();

		void begin() override;

		void handle() override;

		void start(uint16_t bpm);

		void stop();

	private:
		// midi event with delay before execution in arbitrary time units
		// and note # (status of -1 denotes end of track)
		struct MidiEvent {
			int64_t time;
			uint32_t status;
			uint8_t note;
		};
		// Default message with data[0] set to operation mode 2 (Accessory On/Off mode)
		const CanMessage DEFAULT_MESSAGE = { CAN_ACC_OPERATION, 0x1, { 0x2, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } };
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