#include <unordered_map>
//Import des bibliothèques ESP8266
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266httpUpdate.h>
#include <TimeLib.h>
#include <elapsedMillis.h>
#include "OneButton.h"
#include <Adafruit_NeoPixel.h>
//Import des bibliotheques SPIFFS
#include "FS.h"
#include "src/spiffs_lib.h"
#include "src/Config.h"
#include "src/MOD1023Sensor.h"
#include "src/BME680Sensor.h"
#include "src/CCS811Sensor.h"
#include "src/Measure.h"

const String version = "version beta++";

Config config;
MOD1023Sensor mod1023(Serial);
BME680Sensor bme680(Serial);
CCS811Sensor ccs811(Serial);

std::unordered_map<std::string, MeasureReader> measureReaders =
	{
		{ "mod1023_co2", MeasureReader("MOD1023 - CO2 (ppm)", Measure::CO2, 450, 2000, []() { return mod1023.read_co2(); }) },
		{ "mod1023_hum", MeasureReader("MOD1023 - Humidité (%)", Measure::HUMIDITY, 0, 100, []() { return mod1023.read_humidity(); }) },
		{ "mod1023_pres", MeasureReader("MOD1023 - Pression (hPa)", Measure::PRESSURE, 300, 1100, []() { return mod1023.read_pressure(); }) },
		{ "mod1023_temp", MeasureReader("MOD1023 - Température (°C)", Measure::TEMPERATURE, -40, 85, []() { return mod1023.read_temperature(); }) },
		{ "mod1023_tvoc", MeasureReader("MOD1023 - TVOC (ppb)", Measure::TVOC, 125, 600, []() { return mod1023.read_tvoc(); }) },
		{ "bme680_hum", MeasureReader("BME680 - Humidité (%)", Measure::HUMIDITY, 0, 100, []() { return bme680.read_humidity(); }) },
		{ "bme680_temp", MeasureReader("BME680 - Température (°C)", Measure::TEMPERATURE, -40, 85, []() { return bme680.read_temperature(); }) },
		{ "ccs811_co2", MeasureReader("CCS811 - CO2 (ppm)", Measure::CO2, 450, 2000, []() { return ccs811.read_co2(); }) },
		{ "ccs811_temp", MeasureReader("CCS811 - Température (°C)", Measure::TEMPERATURE, -40, 85, []() { return ccs811.read_temperature(); }) },
		{ "ccs811_tvoc", MeasureReader("CCS811 - TVOC (ppb)", Measure::TVOC, 125, 600, []() { return ccs811.read_tvoc(); }) },
	};

String save = "false";
boolean writeTitle = true;
OneButton button(D3, true);
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(1, D4, NEO_GRB + NEO_KHZ800);

void setup()
{
	delay(1000);

	Serial.begin(9600); //debug série

	pixels.begin();
	pixels.setPixelColor(0, pixels.Color(10, 0, 0));
	pixels.show();

	button.attachDuringLongPress(erase_save);
	button.attachDoubleClick(toggle_save);

	// reset pin connected to rst
	pinMode(D0, OUTPUT);

	digitalWrite(D0, LOW);

	pinMode(BUILTIN_LED, OUTPUT);
	digitalWrite(BUILTIN_LED, HIGH);

	// démarrage de la gestion SPIFFS
	SPIFFS.begin(); 

	if (!read_config("/config.txt"))
	{
		Serial.println("Program stopped.");
		return;
	}

	if (config.client_mode.equals("true"))
	{
		WiFi.mode(WIFI_STA);
		connect_wifi(config.ssid_client.c_str(), config.password_client.c_str());
		web_update();
	}
	else
	{
		WiFi.mode(WIFI_AP);
		WiFi.softAP("captobox", "", 2);
	}

	MDNS.begin("captobox");
	MDNS.addService("http", "tcp", 80);

	// demarrage dur serveur de fichiers
	setup_server();
	server.begin();

	Serial.println("\nConfiguration");
	Serial.println("================================================");
	config.print(Serial);
	Serial.println("================================================");

	Serial.println("Initialisation des capteurs... ");
	mod1023.init();
	bme680.init();
	ccs811.init();
	Serial.println("OK.\n");

	digitalWrite(BUILTIN_LED, HIGH);

	pixels.setPixelColor(0, pixels.Color(0, 10, 0));
	pixels.show();
}

void loop()
{
	elapsedMillis timeElapsed;

	server.handleClient();
	button.tick();

	if (timeElapsed > config.frequency.toInt())
	{
		if (config.stream.equals("true"))
		{
			Serial.println("tick : " + millis());

			send_json(config.host.c_str(), config.request,
				String(measureReaders[config.capt1].read()),
				String(measureReaders[config.capt2].read()));

			Serial.println("tick : " + millis());
		}

		savedata();

		timeElapsed = 0;
	}
}

void setup_server()
{
	server.on("/", HTTP_GET, []()
		{
			if (!handleFileRead("/index.html")) server.send(404, "text/plain", "FileNotFound");
		});
	server.on("/config.html", HTTP_GET, []()
		{
			if (!handleFileRead("/config.html")) server.send(404, "text/plain", "FileNotFound");
		});

	server.on("/write", HTTP_GET, []()
		{
			File configFile;

			config.capt1 = server.arg("capt1").c_str();
			config.capt2 = server.arg("capt2").c_str();
			config.frequency = server.arg("frequency");
			config.client_mode = server.arg("client");
			config.ssid_client = server.arg("ssid");
			config.password_client = server.arg("password");
			config.request = server.arg("request");
			//config.host = server.arg("config.host");
			config.stream = server.arg("stream");

			config.print(Serial);

			server.send(200, "text/plain", "okay");

			configFile = SPIFFS.open("/config.txt", "w");

			server.send(200, "text/plain", "ecriture spiffs");

			config.save(configFile);
			configFile.close();

			Serial.println("fichier enregistré");

			delay(500);
			digitalWrite(D0, LOW);
			//ESP.restart();
			delay(500);
			//reset de l'esp (D0 donnecté avec une resistance de 510 ohms à RST)
		});

	server.on("/capt1_raw", HTTP_GET, []()
		{
			server.send(200, "text/json", String(measureReaders[config.capt1].read()));
		});

	server.on("/capt2_raw", HTTP_GET, []()
		{
			server.send(200, "text/json", String(measureReaders[config.capt2].read()));
		});

	server.on("/lecture", HTTP_GET, []()
		{
			MeasureReader& capt1 = measureReaders[config.capt1];
			MeasureReader& capt2 = measureReaders[config.capt2];
			String json = "{";
			json += "\"capt1\":\"" + String(capt1.read()) + "\"";
			json += ", \"name_capt1\":\"" + capt1.name + "\"";
			json += ", \"min_capt1\":\"" + String(capt1.min) + "\"";
			json += ", \"max_capt1\":\"" + String(capt1.max) + "\"";
			json += ", \"capt2\":\"" + String(capt2.read()) + "\"";
			json += ", \"name_capt2\":\"" + capt2.name + "\"";
			json += ", \"min_capt2\":\"" + String(capt2.min) + "\"";
			json += ", \"max_capt2\":\"" + String(capt2.max) + "\"";
			json += ", \"save\":\"" + save + "\"";
			json += "}";

			server.send(200, "text/json", json);

			json = String();
		});

	server.on("/lecture_config", HTTP_GET, []()
		{
			String json = "{";
			json += "\"ssid_client\":\"" + config.ssid_client + "\"";
			json += ", \"password_client\":\"" + config.password_client + "\"";
			json += ", \"request\":\"" + config.request + "\"";
			json += ", \"client\":\"" + config.client_mode + "\"";
			json += ", \"frequency\":\"" + config.frequency + "\"";
			json += ", \"capt1\":\"" + String(config.capt1.c_str()) + "\"";
			json += ", \"capt2\":\"" + String(config.capt2.c_str()) + "\"";
			json += ", \"host\":\"" + config.host + "\"";
			json += ", \"stream\":\"" + config.stream + "\"";
			json += ", \"save\":\"" + save + "\"";
			json += ", \"vers\":\"" + version + "\"";
			json += "}";

			server.send(200, "text/json", json);

			json = String();
		});

	//  == upload firmware

	server.onFileUpload([]()
		{
			HTTPUpload& upload = server.upload();
			
			if (server.uri() != "/update")
			{
				return;
			}

			if (upload.status == UPLOAD_FILE_START)
			{
				uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
				
				Serial.println("Upload Name: " + upload.filename);

				if (upload.filename.endsWith(".spiffs.bin"))
				{
					Serial.println("update to spiffs");

					// start with max available size
					if (!Update.begin(maxSketchSpace, U_SPIFFS))
					{
						Update.printError(Serial);
						Serial.println("ERROR");
					}
				}
				else if (upload.filename.endsWith(".d1_mini.bin"))
				{
					Serial.println("update to flash");

					// start with max available size
					if (!Update.begin(maxSketchSpace))
					{
						Update.printError(Serial);
						Serial.println("ERROR");
					}
				}
			}
			else if (upload.status == UPLOAD_FILE_WRITE)
			{
				Update.write(upload.buf, upload.currentSize);
			}
			else if (upload.status == UPLOAD_FILE_END)
			{
				Update.end(true);
			}
		});

	server.on("/update", HTTP_POST, []()
		{
			server.sendHeader("Connection", "close");
			server.sendHeader("Access-Control-Allow-Origin", "*");
			server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK rebootez la captobox");
			delay(50);
			digitalWrite(D0, LOW); //reset de l'esp (D0 donnecté avec une resistance de 510 ohms à RST)
		});

	server.on("/save_spiff", HTTP_GET, []()
		{
			server.send(200, "text/plain", "save ok");
			Serial.println("commande sauvegarde OK");
			save = "true";
		});

	server.on("/stop_save_spiff", HTTP_GET, []()
		{
			server.send(200, "text/plain", "save stop");
			save = "false";
		});

	server.on("/erase_save_spiff", HTTP_GET, []()
		{

		});

	server.onNotFound([]()
		{
			if (!handleFileRead(server.uri()))
				server.send(404, "text/plain", "FileNotFound");
		});
}

void savedata()
{
	if (!save.equals("true"))
	{
		pixels.setPixelColor(0, pixels.Color(0, 10, 0));
		pixels.show();
		return;
	}

	File saveFile = SPIFFS.open("/save.csv", "a");
	MeasureReader& capt1 = measureReaders[config.capt1];
	MeasureReader& capt2 = measureReaders[config.capt2];
	String dateStr;

	pixels.setPixelColor(0, pixels.Color(0, 0, 10));
	pixels.show();

	if (writeTitle)
	{
		saveFile.println("date:" + capt1.name + ":" + capt2.name);
		writeTitle = false;
	}

	dateStr = String(year());

	for (const int &token :
		std::vector<int>{ month(), day(), hour(), minute(), second() })
	{
		dateStr += "," + String(token);
	}

	saveFile.println(dateStr + ":" + String(capt1.read()) + ":" + String(capt2.read()));
	saveFile.close();
}

void erase_save()
{
	SPIFFS.remove("/save.csv");

	writeTitle = true;

	pixels.setPixelColor(0, pixels.Color(10, 0, 0));
	pixels.show();
	delay(50);

	pixels.setPixelColor(0, pixels.Color(0, 0, 10));
	pixels.show();
	delay(50);
}

void toggle_save()
{
	if (save.equals("true"))
	{
		save = "false";
	}
	else
	{
		save = "true";
	}
}

void connect_wifi(const char* ssid, const char* password)
{
	int timeout = 0;

	WiFi.begin(ssid, password);
	Serial.println("connection en cours...");

	while (WiFi.status() != WL_CONNECTED)
	{
		delay(500);
		timeout++;

		Serial.print(".");

		if (timeout > 50)
		{
			String configLine;
			File configuration = SPIFFS.open("/config.txt", "w");

			Serial.println("timeout");

			for (const String &token :
				std::vector<String>{
					String(config.capt1.c_str()), String(config.capt2.c_str()), config.frequency,
					"false", config.ssid_client, config.password_client,
					config.request, config.host, config.stream })
			{
				configLine += token + "$";
			}

			configuration.print(configLine);
			configuration.close();
			delay(500);

			digitalWrite(D0, LOW);
			ESP.reset();
			delay(500);
		}
	}

	Serial.println("\nWiFi connected\nIP address:\n" + String(WiFi.localIP()));
}

bool read_config(const char* fichier)
{
	File  file = SPIFFS.open(fichier, "r");

	if (!file)
	{
		Serial.println("Failed to open the configuration file (path: \"" + String(fichier) + "\").");
		return false;
	}
	
	config.load(file);

	return true;
}

void send_json(const char* host_in, String url_in, String data1, String data2)
{
	WiFiClient client;
	String contentType = "application/json";
	String json = "{";
	json += "\"value1\":\"" + data1 + "\"";
	json += ", \"value2\":\"" + data2 + "\"";
	json += "}";

	const int httpPort = 80;

	if (!client.connect(host_in, httpPort))
	{
		Serial.print("Requesting URL error: ");
		//stream = "false";
		return;
	}

	Serial.println("Requesting URL: " + url_in);

	// This will send the request to the server
	client.print(String("GET ") + url_in + " HTTP/1.1\r\n" +
		"Host: " + host_in + "\r\n" +
		"Cache-Control: no-cache" + "\r\n" +
		"Content-Type: " + contentType + "\r\n" +
		"Content-Length: " + json.length() + "\r\n" +
		"\r\n" +
		json + "\r\n" +
		"Connection: close\r\n\r\n");
}

void read_sensors()
{
	Serial.println("lecture des capteurs");
	Serial.println("capteur 1 : " + String(measureReaders[config.capt1].read()));
	Serial.println("capteur 2 : " + String(measureReaders[config.capt2].read()));
	delay(100);
}

void web_update()
{
	t_httpUpdate_return ret = ESPhttpUpdate.update("192.30.253.112", 80, "/julienrat/captobox/raw/master/bin/captobox.bin", "optional current version string here");
	
	switch (ret)
	{
		case HTTP_UPDATE_FAILED:
			Serial.println("[update] Update failed.");
			break;
		case HTTP_UPDATE_NO_UPDATES:
			Serial.println("[update] Update no Update.");
			break;
		case HTTP_UPDATE_OK:
			Serial.println("[update] Update ok."); // may not called we reboot the ESP
			break;
	}
}
