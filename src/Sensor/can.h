#ifndef CAN_NAMESPACE_H_
#define CAN_NAMESPACE_H_

namespace can {

	// This struct contains all the components of a CAN message. dataLength must be <= 8, 
	// and the first [dataLength] positions of data[] must contain valid data
	struct CanMessage {
		uint16_t id;
		uint8_t dataLength;
		uint8_t data[8];
	};

	// generic can message
	const CanMessage CAN_MESSAGE_NULL = {0x0, 0, {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};
}

#endif