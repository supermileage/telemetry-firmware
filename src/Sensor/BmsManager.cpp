#include "BmsManager.h"

#define UPDATE_INTERVAL 1000
#define MILLISECONDS_BEFORE_DESELECT 10000

const uint32_t BmsManager::UpdateInterval = UPDATE_INTERVAL;
const uint32_t BmsManager::MillisecondsBeforeDeselect = MILLISECONDS_BEFORE_DESELECT;

BmsManager::BmsManager(CanSensorBms** bmsPtr, CanSensorBms* orion, CanSensorBms* tiny, BmsOption option) : _mainBmsPtr(bmsPtr), _orion(orion), _tiny(tiny), _currentOption(option) {
	setBms(option);
}

BmsManager::~BmsManager() { }

void BmsManager::begin() { }

void BmsManager::handle() {
	if (millis() > _lastTime + UPDATE_INTERVAL) {
		BmsOption newOption = None;
		
		if (_orion->getLastUpdateTime() > _tiny->getLastUpdateTime()) {
			newOption = Orion;
		} else if (_orion->getLastUpdateTime() < _tiny->getLastUpdateTime()) {
			newOption = Tiny;
		}
		
		if (newOption != None && _currentOption != newOption) {
			setBms(newOption);
		}

		_lastTime = millis();
	}
}

String BmsManager::getHumanName() {
	return "BmsManager";
}

void BmsManager::setBms(BmsOption option) {
	switch (option) {
		case BmsManager::Orion:
			_tiny->setIsAsleep(true);
			_orion->setIsAsleep(false);
			*_mainBmsPtr = _orion;
			break;
		case BmsManager::Tiny:
			_orion->setIsAsleep(true);
			_tiny->setIsAsleep(false);
			*_mainBmsPtr = _tiny;
			break;
		default:
			_tiny->setIsAsleep(true);
			_orion->setIsAsleep(true);
			// _mainBmsPtr should not be set to null (see vehicleUrban.cpp)
			break;
	}

	_currentOption = option;
}

int BmsManager::getCurrentBms(bool& valid) {
	valid = true;

	switch (_currentOption) {
		case BmsManager::Orion:
			return (_orion->getLastUpdateTime() + MILLISECONDS_BEFORE_DESELECT > millis()) ? _currentOption : BmsManager::None;
		case BmsManager::Tiny:
			return (_tiny->getLastUpdateTime() + MILLISECONDS_BEFORE_DESELECT > millis()) ? _currentOption : BmsManager::None;
		default:
			return _currentOption;
	}
}

String BmsManager::getCurrentBmsName() {
	if (_currentOption != None && *_mainBmsPtr != NULL) {
		return ((*_mainBmsPtr)->getHumanName());
	} else {
		return "None";
	}
}