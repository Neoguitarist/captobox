#include "CCS811Sensor.h"

CCS811Sensor::CCS811Sensor(Stream& logOutput) :
	Sensor(logOutput, "CCS811")
{
	_repeatDelay = 50;
	_calibrationTimeout = 5000;
	_readingTimeout = 1000;
}

void CCS811Sensor::init()
{
	if (!_ccs811.begin())
	{
		log("Could not find a valid sensor, check wiring!");
		return;
	}

	if (!wait_for_availability(_calibrationTimeout))
	{
		log("Failed to reach sensor in time for calibration.");
		return;
	}

	_ccs811.setTempOffset(_ccs811.calculateTemperature() - 25.0);
}

float CCS811Sensor::read_co2()
{
	return perform_reading() ? _ccs811.geteCO2() : 0;
}

float CCS811Sensor::read_tvoc()
{
	return perform_reading() ? _ccs811.getTVOC() : 0;
}

float CCS811Sensor::read_temperature()
{
	return perform_reading() ? _ccs811.calculateTemperature() : 0;
}

bool CCS811Sensor::wait_for_availability(size_t timeout)
{
	for (size_t elapsedTime = 0;
		elapsedTime < timeout;
		elapsedTime += _repeatDelay)
	{
		if (_ccs811.available())
		{
			return true;
		}

		delay(_repeatDelay);
	}

	return false;
}

bool CCS811Sensor::perform_reading()
{
	return wait_for_availability(_readingTimeout) ? !_ccs811.readData() : false;
}
