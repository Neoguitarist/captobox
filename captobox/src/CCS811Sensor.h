#pragma once

#include <Adafruit_CCS811.h>

#include "Sensor.h"

class CCS811Sensor :
	public Sensor
{
public:

	CCS811Sensor(Stream& logOutput);

	void init();

	float read_co2();
	float read_tvoc();
	float read_temperature();

private:

	Adafruit_CCS811 _ccs811;
	size_t _repeatDelay;
	size_t _calibrationTimeout;
	size_t _readingTimeout;

	bool wait_for_availability(size_t timeout);
	bool perform_reading();
};

