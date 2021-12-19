#ifndef CAN_ACCESSORIES_LISTENER
#define CAN_ACCESSORIES_LISTENER

#include <array>

#include "SensorCanBase.h"
#include "CanInterface.h"
#include "Delegate.h"


class CanAccessoriesListener : public SensorCanBase {
	public:
		typedef std::array<uint8_t, 8> StatusIds;
		CanAccessoriesListener(CanInterface& canInterface, uint16_t id, StatusIds ids);

		void begin();

		void handle();

		String getHumanName();

		String getStatus(uint8_t statusId);
	private:
		StatusIds _idArray;
		CanInterface::CanMessage _statusMessage;

		void _updateMessage(uint8_t data);

		uint8_t _getCanMessageDataIndex(uint8_t statusId);

		class CanAccessoriesMessageParser : public Delegate {
			public:
				CanAccessoriesMessageParser(CanAccessoriesListener* owner);

				void* execute(void* arg);
			private:
				CanAccessoriesListener* _owner;
		};
};

#endif