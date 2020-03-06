#include "Sensor.h"

Sensor::Sensor(Stream& logOutput, String pName) :
	_logOutput(logOutput), name(pName)
{}

void Sensor::init()
{}

void Sensor::log(String msg) const
{
	_logOutput.println("[" + name + "] " + msg);
}
