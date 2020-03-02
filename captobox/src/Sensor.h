#pragma once

#include <list>
#include <WString.h>
#include <Stream.h>

class Sensor
{
public:

	const String id;
	const String label;

	Sensor(Stream& logOutput, String pId, String pLabel);

	virtual void init();

protected:

	virtual void log(String msg) const;

private:

	Stream& _logOutput;
};

class ISensorVisitor
{
public:

	void visit()
};
