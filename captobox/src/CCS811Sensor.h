#pragma once

#include <Adafruit_CCS811.h>

#include "Sensor.h"

/**
 * Capteur CCS811.
 */
class CCS811Sensor :
	public Sensor
{
public:

	/**
	 * @param {logOutput} Flux sur lequel le capteur pourr �crire des messages de log.
	 * @param {repeatDealy} D�lai d'attente entre deux tentatives de communication avec le capteur (en millisecondes).
	 * @param {calibrationTimeout} Dur�e maximale accord�e pour une calibration du capteur (en millisecondes).
	 * @param {readingTimeout} Dur�e maximale accord�e pour une lecture du capteur (en millisecondes).
	 */
	CCS811Sensor(Stream& logOutput, size_t repeatDelay = 50,
		size_t calibrationTimeout = 5000, size_t readingTimeout = 2000);

	void init();

	float read_co2();
	float read_tvoc();
	float read_temperature();

private:

	/** Impl�mentation sous-jacente. */
	Adafruit_CCS811 _ccs811;
	/** D�lai d'attente entre deux tentatives de communication avec le capteur (en millisecondes). */
	size_t _repeatDelay;
	/** Dur�e maximale accord�e pour une calibration du capteur (en millisecondes).
	 * Si cette dur�e est d�pass�e, la calibration est abandonn�e et une erreur est inscrite dans le log. */
	size_t _calibrationTimeout;
	/** Dur�e maximale accord�e pour une lecture du capteur (en millisecondes).
	 * Si cette dur�e est d�pass�e, la lecture est abandonn�e et une erreur est inscrite dans le log.  */
	size_t _readingTimeout;

	/**
	 * Attend que le capteur soit disponible.
	 * Si la dur�e maximale d'attente donn�e est d�pass�e, l'op�ration est abandonn�e.
	 *
	 * @param {timeout} Dur�e maximale d'attente (en millisecondes).
	 * @returns Vrai si le capteur est disponible, faux sinon.
	 */
	bool wait_for_availability(size_t timeout);
	/**
	 * Tente d'effectuer un relev� sur le capteur.
	 * La prise de relev� peut �chouer si le capteur n'est pas disponible dans le temps imparti.
	 *
	 * @returns Vrai si le relev� a pu �tre fait, faux sinon.
	 */
	bool perform_reading();
};

