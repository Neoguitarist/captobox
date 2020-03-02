#include <DallasTemperature.h>
#include <BlueDot_BME280.h>
#include <Adafruit_BMP085_U.h>
#include <DHT.h>
#include <iAQ-MOD1023.h>
#include <Wire.h>
#include <bme680_defs.h>
#include <bme680.h>
#include <Adafruit_BME680.h>

#include "Sensor.h"
#include "Measure.h"

#define RX 1
#define TX 0

class MOD1023 :
	Sensor
{
public:

	MOD1023(Stream& logOutput, int iaqPin) :
		Sensor(logOutput, "mod1023", "MOD1023")
	{
		_iaqPin = iaqPin;
	}

	virtual void init()
	{
		init_bme280();
		init_iaq();
	}

	float read_temperature()
	{
		return _bme280.readTempC();
	}

	float read_humidity()
	{
		return _bme280.readHumidity();
	}

	float read_pressure()
	{
		return _bme280.readPressure();
	}

	float read_cov()
	{
		iaq.readRegisters();
		return iaq.getTVOC();
	}

	float read_co2()
	{
		iaq.readRegisters();
		return iaq.getPrediction();
	}

private:

	BlueDot_BME280 _bme280;
	int _iaqPin;

	void init_bme280()
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

	void init_iaq()
	{
		if (_iaqPin == RX)
		{
			Wire.begin(TX, RX);
			log("Connecté sur entrée TX.");
		}
		else
		{
			Wire.begin();
			log("Connecté sur pin " + String(_iaqPin) + ".");
		}

		Wire.setClockStretchLimit(1500);
	}
};

class BME680 :
	Sensor
{
public:

	BME680(Stream& logOutput) :
		Sensor(logOutput, "bme680", "BME680")
	{}

	virtual void init()
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

	float read_temperature()
	{
		return perform_reading() ? _bme680.temperature : 0;
	};

	float read_humidity()
	{
		return perform_reading() ? _bme680.humidity : 0;
	};

private:

	Adafruit_BME680 _bme680;

	bool perform_reading()
	{
		if (!_bme680.performReading())
		{
			log("Failed to perform reading :(");
			return false;
		}

		return true;
	}
};

float sensor_start22(String type, int pin)
{
	std::list<Measure*> measures;
	Measure* targetMeasure = NULL;
	
	for (Measure* measure : measures)
	{
		if (measure->id == type)
		{
			targetMeasure = measure;
			break;
		}
	}

	return (targetMeasure == NULL) ? 0 : targetMeasure->read();
}
