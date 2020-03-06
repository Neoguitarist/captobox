#pragma once

#include <WString.h>
#include <functional>

#include "Sensor.h"

enum Measure
{
	TEMPERATURE,
	PRESSURE,
	HUMIDITY,
	TVOC,
	CO2
};

class MeasureReader
{
public:

	const String name;
	const Measure type;
	const float min;
	const float max;

	MeasureReader();
	MeasureReader(const MeasureReader& o);
	MeasureReader(String pName, Measure pType, float pMin, float pMax,
		std::function<float()> reader);

	float read();

private:

	std::function<float()> _reader;
};
