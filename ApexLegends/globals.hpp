#pragma once

struct globals
{
	struct
	{
		std::string version;
		std::string status;

		std::string ip = xorstr_("haruna7777.com");

		struct
		{
			std::string cipher;
			std::string iv;
		} key;
	} server_side;

	struct
	{
		std::string version = xorstr_("0.01");

		struct
		{
			std::string name = utilities::get_random_string(16);
			float width = 440.f;
			float height = 280.f;
		} window_settings;		

		struct
		{
			std::string key;
			std::string hwid = utilities::get_hwid();

			std::string token;
			std::string structure_cheat;

		} data;
	} client_side;
};

extern globals g_globals;