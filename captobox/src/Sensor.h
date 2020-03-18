#pragma once

#include <WString.h>
#include <Stream.h>

/**
 * Capteur g�n�rique, servant de base pour l'impl�mentation d'un capteur sp�cifique.
 */
class Sensor
{
public:

	/** Nom donn� au capteur. */
	const String name;

	/**
	 * @param {logOutput} Flux sur lequel le capteur pourr �crire des messages de log.
	 * @param {pName} Nom � donner au capteur.
	 */
	Sensor(Stream& logOutput, String pName);

	/**
	 * Initialise le capteur.
	 */
	virtual void init();

protected:

	/**
	 * �crit le message de donn� sur le flux de log.
	 *
	 * @param {msg} Message � �crire.
	 */
	virtual void log(String msg) const;

private:

	/** Flux de log. */
	Stream& _logOutput;
};
