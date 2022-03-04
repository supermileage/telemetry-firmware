#ifndef _CAN_ACCESSORIES_MIDI_PLAYER_H_
#define _CAN_ACCESSORIES_MIDI_PLAYER_H_

#include <queue>
#include <stdlib.h>

#include "midi-parser.h"
#include "CanInterface.h"

using namespace can;

const char MIDI_FILE[] = "4D546864 00000006 00000001 00604D54 726B0000 061B00FF 03010000 FF580404 02240800 FF580404 02240800 903C6400 90426418 803C4000 903D6400 90416400 80424018 803D4000 903E6400 90406400 80414018 803E4000 903F6400 80404018 903E6400 803F4000 90406418 903D6400 90416418 903C6400 90426430 803C4000 903C6400 803D4000 803E4000 80404000 80414000 80424000 90426418 803C4000 903D6400 90416400 80424018 803D4000 903E6400 90406400 80414018 803E4000 903F6400 80404018 903E6400 803F4000 90406418 903D6400 803E4000 80404000 90416418 903C6400 803D4000 80414000 90426418 803C4000 903D6400 90416400 80424018 803D4000 903E6400 90406400 80414018 803E4000 903F6400 80404018 903E6400 803F4000 90406418 903D6400 803E4000 80404000 90416418 903C6400 803D4000 80414000 90426418 803C4000 903D6400 90416400 80424018 803D4000 903E6400 90406400 80414018 803E4000 903F6400 80404018 903E6400 803F4000 90406418 903D6400 90416418 903C6400 90426430 803C4000 903C6400 803D4000 803E4000 80404000 80414000 80424000 90426418 803C4000 903D6400 90416400 80424018 803D4000 903E6400 90406400 80414018 803E4000 903F6400 80404018 903E6400 803F4000 90406418 903D6400 90416418 903C6400 90426430 803C4000 903C6400 803D4000 803E4000 80404000 80414000 80424000 90426418 803C4000 903D6400 90416400 80424018 803D4000 903E6400 90406400 80414018 803E4000 903F6400 80404018 903E6400 803F4000 90406418 903D6400 803E4000 80404000 90416418 903C6400 803D4000 80414000 90426418 803C4000 903D6400 90416400 80424018 803D4000 903E6400 90406400 80414018 803E4000 903F6400 80404018 903E6400 803F4000 90406418 903D6400 803E4000 80404000 90416418 903C6400 803D4000 80414000 90426418 803C4000 903D6400 90416400 80424018 803D4000 903E6400 90406400 80414018 803E4000 903F6400 80404018 903E6400 803F4000 90406418 903D6400 90416418 903C6400 803E4000 80404000 90426430 803D4000 80414081 40803C40 00903C64 00804240 00904264 18803C40 00903D64 00904164 00804240 18803D40 00903E64 00904064 00804140 18803E40 00903F64 00804040 18903E64 00803F40 00904064 18903D64 00904164 18903C64 00904264 30803C40 00903C64 00803D40 00803E40 00804040 00804140 00804240 00904264 18803C40 00903D64 00904164 00804240 18803D40 00903E64 00904064 00804140 18803E40 00903F64 00804040 18903E64 00803F40 00904064 18903D64 00803E40 00804040 00904164 18903C64 00803D40 00804140 00904264 18803C40 00903D64 00904164 00804240 18803D40 00903E64 00904064 00804140 18803E40 00903F64 00804040 18903E64 00803F40 00904064 18903D64 00803E40 00804040 00904164 18903C64 00803D40 00804140 00904264 18803C40 00903D64 00904164 00804240 18803D40 00903E64 00904064 00804140 18803E40 00903F64 00804040 18903E64 00803F40 00904064 18903D64 00904164 18903C64 00904264 30803C40 00903C64 00803D40 00803E40 00804040 00804140 00804240 00904264 18803C40 00903D64 00904164 00804240 18803D40 00903E64 00904064 00804140 18803E40 00903F64 00804040 18903E64 00803F40 00904064 18903D64 00904164 18903C64 00904264 30803C40 00903C64 00803D40 00803E40 00804040 00804140 00804240 00904264 18803C40 00903D64 00904164 00804240 18803D40 00903E64 00904064 00804140 18803E40 00903F64 00804040 18903E64 00803F40 00904064 18903D64 00803E40 00804040 00904164 18903C64 00803D40 00804140 00904264 18803C40 00903D64 00904164 00804240 18803D40 00903E64 00904064 00804140 18803E40 00903F64 00804040 18903E64 00803F40 00904064 18903D64 00803E40 00804040 00904164 18903C64 00803D40 00804140 00904264 18803C40 00903D64 00904164 00804240 18803D40 00903E64 00904064 00804140 18803E40 00903F64 00804040 18903E64 00803F40 00904064 18903D64 00904164 18903C64 00803E40 00804040 00904264 30803D40 00804140 8140803C 40008042 4000FF2F 00";

// Class which transposes midi files to can messages
class CanAccessoriesMidiPlayer : public Handleable {
	public:

		CanAccessoriesMidiPlayer(CanInterface& can);

		~CanAccessoriesMidiPlayer();

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