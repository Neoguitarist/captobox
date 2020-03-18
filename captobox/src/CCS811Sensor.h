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
	 * @param {logOutput} Flux sur lequel le capteur pourr écrire des messages de log.
	 * @param {repeatDealy} Délai d'attente entre deux tentatives de communication avec le capteur (en millisecondes).
	 * @param {calibrationTimeout} Durée maximale accordée pour une calibration du capteur (en millisecondes).
	 * @param {readingTimeout} Durée maximale accordée pour une lecture du capteur (en millisecondes).
	 */
	CCS811Sensor(Stream& logOutput, size_t repeatDelay = 50,
		size_t calibrationTimeout = 5000, size_t readingTimeout = 2000);

	void init();

	float read_co2();
	float read_tvoc();
	float read_temperature();

private:

	/** Implémentation sous-jacente. */
	Adafruit_CCS811 _ccs811;
	/** Délai d'attente entre deux tentatives de communication avec le capteur (en millisecondes). */
	size_t _repeatDelay;
	/** Durée maximale accordée pour une calibration du capteur (en millisecondes).
	 * Si cette durée est dépassée, la calibration est abandonnée et une erreur est inscrite dans le log. */
	size_t _calibrationTimeout;
	/** Durée maximale accordée pour une lecture du capteur (en millisecondes).
	 * Si cette durée est dépassée, la lecture est abandonnée et une erreur est inscrite dans le log.  */
	size_t _readingTimeout;

	/**
	 * Attend que le capteur soit disponible.
	 * Si la durée maximale d'attente donnée est dépassée, l'opération est abandonnée.
	 *
	 * @param {timeout} Durée maximale d'attente (en millisecondes).
	 * @returns Vrai si le capteur est disponible, faux sinon.
	 */
	bool wait_for_availability(size_t timeout);
	/**
	 * Tente d'effectuer un relevé sur le capteur.
	 * La prise de relevé peut échouer si le capteur n'est pas disponible dans le temps imparti.
	 *
	 * @returns Vrai si le relevé a pu être fait, faux sinon.
	 */
	bool perform_reading();
};

