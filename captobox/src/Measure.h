#pragma once

#include <WString.h>
#include <functional>

#include "Sensor.h"

/**
 * Types de mesures pris en charge.
 */
enum Measure
{
	TEMPERATURE,
	PRESSURE,
	HUMIDITY,
	TVOC,
	CO2
};

/**
 * Lecteur de mesure.
 */
class MeasureReader
{
public:

	/** Nom donn� au lecteur. */
	const String name;
	/** Type de mesure lue. */
	const Measure type;
	/** Borne inf�rieure des valeurs pouvant �tre lues. */
	const float min;
	/** Borne sup�rieure des valeurs pouvant �tre lues. */
	const float max;

	MeasureReader();
	MeasureReader(const MeasureReader& o);

	/**
	 * @param {pName} Nom � donner au lecteur.
	 * @param {pType} Type de mesure lue.
	 * @param {pMin} Borne inf�rieure des valeurs pouvant �tre lues.
	 * @param {pMax} Borne sup�rieure des valeurs pouvant �tre lues.
	 * @param {reader} Foncteur effectuant un relev�.
	 */
	MeasureReader(String pName, Measure pType, float pMin, float pMax,
		std::function<float()> reader);

	/**
	 * Effectue un relev�.
	 *
	 * @returns La valeur lue.
	 */
	float read();

private:

	/** Foncteur effectuant un relev�. */
	std::function<float()> _reader;
};
