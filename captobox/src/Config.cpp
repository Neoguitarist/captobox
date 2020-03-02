#include <vector>

#include "Config.h"

Config::Config()
{
	frequency = "5000";
}

void Config::load(Stream& input)
{
	std::vector<String> buff;

	while (input.available())
	{
		buff.push_back(input.readStringUntil('$'));
	}

	capt1 = buff[1];
	capt2 = buff[2];
	frequency = buff[3];
	client_mode = buff[4];
	ssid_client = buff[5];
	password_client = buff[6];
	request = buff[7];
	host = buff[8];
	stream = buff[9];
}

void Config::save(Stream& output) const
{
	String configLine;

	for (const String& token :
		std::vector<String>{
			capt1, capt2, frequency,
			client_mode, ssid_client, password_client,
			request, host, stream })
	{
		configLine += token + "$";
	}

	output.print(configLine);
}

void Config::print(Stream& output) const
{
	output.println("capt1 : " + capt1);
	output.println("capt2 : " + capt2);
	output.println("frequence : " + frequency);
	output.println("client  : " + client_mode);
	output.println("ssid : " + ssid_client);
	output.println("password : " + password_client);
	output.println("requete  : " + request);
	output.println("host : " + host);
	output.println("stream : " + stream);
}
