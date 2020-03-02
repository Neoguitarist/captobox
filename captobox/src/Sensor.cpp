#include "Sensor.h"

Sensor::Sensor(Stream& logOutput, String pId, String pLabel) :
	_logOutput(logOutput), id(pId), label(pLabel)
{}

void Sensor::init()
{}

void Sensor::log(String msg) const
{
	_logOutput.println("[" + label + "] " + msg);
}
