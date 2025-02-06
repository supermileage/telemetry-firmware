#include "IntervalCommand.h"

// IntervalCommand

IntervalCommand::IntervalCommand(uint16_t interval) {
	_interval = interval;
}

uint16_t IntervalCommand::getInterval() {
	return _interval;
}

const std::vector<IntervalCommand*>& IntervalCommand::getCommands() {
    return IntervalCommandHandler::instance().getCommands();
}

// IntervalCommandHandler

IntervalCommand::IntervalCommandHandler* IntervalCommand::IntervalCommandHandler::_instance;

IntervalCommand::IntervalCommandHandler& IntervalCommand::IntervalCommandHandler::instance() {
	if (!_instance) {
		_instance = new IntervalCommandHandler();
	}
	return *_instance;
}

void IntervalCommand::IntervalCommandHandler::add(IntervalCommand* command) {
	_commands.push_back(command);
}

const std::vector<IntervalCommand*>& IntervalCommand::IntervalCommandHandler::getCommands() {
	return _commands;
}