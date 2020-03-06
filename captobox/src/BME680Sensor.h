#pragma once

#include <bme680_defs.h>
#include <bme680.h>
#include <Adafruit_BME680.h>

#include "Sensor.h"
#include "Measure.h"

class BME680Sensor :
	Sensor
{
public:

	BME680Sensor(Stream& logOutput);

	void init();

	float read_temperature();
	float read_humidity();

private:

	Adafruit_BME680 _bme680;

	bool perform_reading();
};
