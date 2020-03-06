#include "MOD1023Sensor.h"

MOD1023Sensor::MOD1023Sensor(Stream& logOutput) :
	Sensor(logOutput, "MOD1023")
{ }

void MOD1023Sensor::init()
{
	init_bme280();
	init_iaq();
}

float MOD1023Sensor::read_temperature()
{
	return _bme280.readTempC();
}

float MOD1023Sensor::read_humidity()
{
	return _bme280.readHumidity();
}

float MOD1023Sensor::read_pressure()
{
	return _bme280.readPressure();
}

float MOD1023Sensor::read_tvoc()
{
	iaq.readRegisters();
	return iaq.getTVOC();
}

float MOD1023Sensor::read_co2()
{
	iaq.readRegisters();
	return iaq.getPrediction();
}

void MOD1023Sensor::init_bme280()
{
	_bme280.parameter.communication = 0;
	_bme280.parameter.I2CAddress = 0x76;
	_bme280.parameter.sensorMode = 0b11;
	_bme280.parameter.IIRfilter = 0b100;
	_bme280.parameter.humidOversampling = 0b101;
	_bme280.parameter.tempOversampling = 0b101;
	_bme280.parameter.pressOversampling = 0b101;
	_bme280.init();
}

void MOD1023Sensor::init_iaq()
{
	Wire.begin();
	Wire.setClockStretchLimit(1500);
}
