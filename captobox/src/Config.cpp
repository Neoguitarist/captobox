#include <vector>

#include "Config.h"

namespace
{
	String get_next_token(Stream& input)
	{
		return input.available() ? input.readStringUntil('$') : "";
	}
}

Config::Config()
{
	frequency = "5000";
}

void Config::load(Stream& input)
{
	capt1 = get_next_token(input).c_str();
	capt2 = get_next_token(input).c_str();
	frequency = get_next_token(input);
	client_mode = get_next_token(input);
	ssid_client = get_next_token(input);
	password_client = get_next_token(input);
	request = get_next_token(input);
	host = get_next_token(input);
	stream = get_next_token(input);
}

void Config::save(Stream& output) const
{
	String configLine;

	for (const String& token :
		std::vector<String>{
			String(capt1.c_str()), String(capt2.c_str()), frequency,
			client_mode, ssid_client, password_client,
			request, host, stream })
	{
		configLine += token + "$";
	}

	output.print(configLine);
}

void Config::print(Stream& output) const
{
	output.println("capt1 : " + String(capt1.c_str()));
	output.println("capt2 : " + String(capt2.c_str()));
	output.println("frequence : " + frequency);
	output.println("client  : " + client_mode);
	output.println("ssid : " + ssid_client);
	output.println("password : " + password_client);
	output.println("requete  : " + request);
	output.println("host : " + host);
	output.println("stream : " + stream);
}

