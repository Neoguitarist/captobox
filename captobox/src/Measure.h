#pragma once

#include <WString.h>

#include "Sensor.h"

class Measure
{
public:

	const String id;
	const String label;
	const float min;
	const float max;

	Measure(String pId, String pLabel, float pMin, float pMax);

	float read(Sensor& sensor);
};

class TemperatureMeasure :
	Measure
{
public:

	TemperatureMeasure();

	float read(Sensor& sensor);
};
