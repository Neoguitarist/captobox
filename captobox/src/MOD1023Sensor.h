#pragma once

#include <iAQ-MOD1023.h>
#include <BlueDot_BME280.h>

#include "Sensor.h"

/**
 * Module MOD1023, comprenant un capteur BME280 et un capteur iAQ.
 */
class MOD1023Sensor :
	Sensor
{
public:

	/**
	 * @param {logOutput} Flux sur lequel le capteur pourr écrire des messages de log.
	 */
	MOD1023Sensor(Stream& logOutput);

	void init();

	float read_temperature();
	float read_humidity();
	float read_pressure();
	float read_tvoc();
	float read_co2();

private:

	/** Implémentation sous-jacente. */
	BlueDot_BME280 _bme280;

	/**
	 * Initialise le capteur BME280.
	 */
	void init_bme280();
	/**
	 * Initialise le capteur iAQ.
	 */
	void init_iaq();
};
