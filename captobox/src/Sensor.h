#pragma once

#include <WString.h>
#include <Stream.h>

class Sensor
{
public:

	const String name;

	Sensor(Stream& logOutput, String pName);

	virtual void init();

protected:

	virtual void log(String msg) const;

private:

	Stream& _logOutput;
};
