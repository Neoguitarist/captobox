#pragma once

#include <iAQ-MOD1023.h>
#include <BlueDot_BME280.h>

#include "Sensor.h"

class MOD1023Sensor :
	Sensor
{
public:

	MOD1023Sensor(Stream& logOutput);

	void init();
	float read_temperature();
	float read_humidity();
	float read_pressure();
	float read_tvoc();
	float read_co2();

private:

	BlueDot_BME280 _bme280;

	void init_bme280();
	void init_iaq();
};
