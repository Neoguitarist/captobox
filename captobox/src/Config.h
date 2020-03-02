#pragma once

#include <WString.h>
#include <Stream.h>

class Config
{
public:

	String capt1;
	String capt2;
	String frequency;
	String client_mode;
	String ssid_client;
	String password_client;
	String request;
	String stream;
	String host;

	Config();

	void load(Stream& input);
	void save(Stream& output) const;
	void print(Stream& output) const;
};
