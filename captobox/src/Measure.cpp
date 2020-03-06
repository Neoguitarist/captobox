#include "Measure.h"

MeasureReader::MeasureReader() :
	MeasureReader("", (Measure)0, 0, 0, []() { return 0; })
{}

MeasureReader::MeasureReader(const MeasureReader& o) :
	MeasureReader(o.name, o.type, o.min, o.max, o._reader)
{}

MeasureReader::MeasureReader(
	String pName, Measure pType, float pMin, float pMax, std::function<float()> reader) :
	name(pName), type(pType), min(pMin), max(pMax), _reader(reader)
{}

float MeasureReader::read()
{
	return _reader();
}
