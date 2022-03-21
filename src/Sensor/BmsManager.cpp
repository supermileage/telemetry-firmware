#include "BmsManager.h"

#define UPDATE_INTERVAL 1000

BmsManager::BmsManager(CanSensorBms** bms, CanSensorBms* orion, CanSensorBms* tiny, BmsOptions option) : _bms(bms), _orion(orion), _tiny(tiny), _currentBms(option) {
	*_bms = option == Orion ? _orion : _tiny;
}

BmsManager::~BmsManager() { }

void BmsManager::begin() { }

void BmsManager::handle() {
	if (millis() > _lastTime + UPDATE_INTERVAL) {
		BmsOptions currentOption = None;

		if (_orion->getLastUpdateTime() > _tiny->getLastUpdateTime()) {
			currentOption = Orion;
		} else if (_orion->getLastUpdateTime() < _tiny->getLastUpdateTime()) {
			currentOption = Tiny;
		}
		
		if (currentOption != None && _currentBms != currentOption) {
			setBms(currentOption);
		}

		_lastTime = millis();
	}
}

void BmsManager::setBms(BmsOptions option) {
	bool isOrion = option == Orion;
	_orion->setIsAsleep(!isOrion);
	_tiny->setIsAsleep(isOrion);
	*_bms = isOrion ? _orion : _tiny;
	_currentBms = option;
}

String BmsManager::getCurrentBmsName() {
	if ((*_bms) != NULL) {
		return ((*_bms)->getHumanName());
	} else {
		return "None";
	}
}