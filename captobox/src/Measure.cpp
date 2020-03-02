#include "Measure.h"

Measure::Measure(String pId, String pLabel, float pMin, float pMax) :
	id(pId), label(pLabel), min(pMin), max(pMax)
{}

float Measure::read(Sensor& sensor)
{
	return sensor.accept(this);
}

TemperatureMeasure::TemperatureMeasure() :
	Measure("", "", -40, 85)
{}

float TemperatureMeasure::read(Sensor& sensor)
{
	return sensor.read_temperature();
}
