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

	/** Nom donné au lecteur. */
	const String name;
	/** Type de mesure lue. */
	const Measure type;
	/** Borne inférieure des valeurs pouvant être lues. */
	const float min;
	/** Borne supérieure des valeurs pouvant être lues. */
	const float max;

	MeasureReader();
	MeasureReader(const MeasureReader& o);

	/**
	 * @param {pName} Nom à donner au lecteur.
	 * @param {pType} Type de mesure lue.
	 * @param {pMin} Borne inférieure des valeurs pouvant être lues.
	 * @param {pMax} Borne supérieure des valeurs pouvant être lues.
	 * @param {reader} Foncteur effectuant un relevé.
	 */
	MeasureReader(String pName, Measure pType, float pMin, float pMax,
		std::function<float()> reader);

	/**
	 * Effectue un relevé.
	 *
	 * @returns La valeur lue.
	 */
	float read();

private:

	/** Foncteur effectuant un relevé. */
	std::function<float()> _reader;
};
