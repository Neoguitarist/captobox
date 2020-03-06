#include "BME680Sensor.h"

BME680Sensor::BME680Sensor(Stream& logOutput) :
	Sensor(logOutput, "BME680")
{}

void BME680Sensor::init()
{
	if (!_bme680.begin())
	{
		log("Could not find a valid sensor, check wiring!");
		return;
	}

	// Set up oversampling and filter initialization
	_bme680.setTemperatureOversampling(BME680_OS_8X);
	_bme680.setHumidityOversampling(BME680_OS_2X);
	_bme680.setPressureOversampling(BME680_OS_4X);
	_bme680.setIIRFilterSize(BME680_FILTER_SIZE_3);
	// 320*C for 150 ms
	_bme680.setGasHeater(320, 150);
}

float BME680Sensor::read_temperature()
{
	return perform_reading() ? _bme680.temperature : 0;
};

float BME680Sensor::read_humidity()
{
	return perform_reading() ? _bme680.humidity : 0;
};

Adafruit_BME680 _bme680;

bool BME680Sensor::perform_reading()
{
	if (!_bme680.performReading())
	{
		log("Failed to perform reading :(");
		return false;
	}

	return true;
}
