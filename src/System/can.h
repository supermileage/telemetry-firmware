#ifndef CAN_NAMESPACE_H_
#define CAN_NAMESPACE_H_

#include <stdint.h>

namespace can {

	// This struct contains all the components of a CAN message. dataLength must be <= 8, 
	// and the first [dataLength] positions of data[] must contain valid data
	typedef uint8_t CanData[8];
	struct CanMessage {
		uint16_t id;
		uint8_t dataLength;
		CanData data;
	};

	// generic can message
	const CanMessage CAN_MESSAGE_NULL = {0x0, 0, {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};
}

#endif