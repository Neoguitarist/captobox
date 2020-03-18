#pragma once

#include <Adafruit_BME680.h>

#include "Sensor.h"

/**
 * Capteur BME680.
 */
class BME680Sensor :
	Sensor
{
public:

	/**
	 * @param {logOutput} Flux sur lequel le capteur pourr �crire des messages de log.
	 */
	BME680Sensor(Stream& logOutput);

	void init();

	float read_temperature();
	float read_humidity();

private:

	/** Impl�mentation sous-jacente. */
	Adafruit_BME680 _bme680;

	/**
	 * Tente d'effectuer un relev� sur le capteur.
	 *
	 * @returns Vrai si le relev� a pu �tre fait, faux sinon.
	 */
	bool perform_reading();
};
