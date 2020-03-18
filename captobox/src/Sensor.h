#pragma once

#include <WString.h>
#include <Stream.h>

/**
 * Capteur générique, servant de base pour l'implémentation d'un capteur spécifique.
 */
class Sensor
{
public:

	/** Nom donné au capteur. */
	const String name;

	/**
	 * @param {logOutput} Flux sur lequel le capteur pourr écrire des messages de log.
	 * @param {pName} Nom à donner au capteur.
	 */
	Sensor(Stream& logOutput, String pName);

	/**
	 * Initialise le capteur.
	 */
	virtual void init();

protected:

	/**
	 * Écrit le message de donné sur le flux de log.
	 *
	 * @param {msg} Message à écrire.
	 */
	virtual void log(String msg) const;

private:

	/** Flux de log. */
	Stream& _logOutput;
};
