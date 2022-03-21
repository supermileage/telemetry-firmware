#include "BmsManager.h"

#define UPDATE_INTERVAL 1000

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

void BmsManager::setBms(BmsOption option) {
	bool isOrion = option == Orion;
	_orion->setIsAsleep(!isOrion);
	_tiny->setIsAsleep(isOrion);
	*_mainBmsPtr = isOrion ? _orion : _tiny;
	_currentOption = option;
}

String BmsManager::getCurrentBmsName() {
	if ((*_mainBmsPtr) != NULL) {
		return ((*_mainBmsPtr)->getHumanName());
	} else {
		return "None";
	}
}