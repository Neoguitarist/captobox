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
	 * @param {logOutput} Flux sur lequel le capteur pourr écrire des messages de log.
	 */
	BME680Sensor(Stream& logOutput);

	void init();

	float read_temperature();
	float read_humidity();

private:

	/** Implémentation sous-jacente. */
	Adafruit_BME680 _bme680;

	/**
	 * Tente d'effectuer un relevé sur le capteur.
	 *
	 * @returns Vrai si le relevé a pu être fait, faux sinon.
	 */
	bool perform_reading();
};
