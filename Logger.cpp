#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <direct.h>
#include <string>
#include <basetsd.h>

namespace Logger
{
	void Log(const char* msg) {
		mkdir("C:\\SOUL");

		std::string value;
		std::string line;
		std::string cfg = "C:\\SOUL\\log.txt";
		std::ifstream in(cfg);
		if (in.is_open())
		{
			while (getline(in, line))
			{
				value.append(line + "\n");
			}
		}

		std::ofstream out;
		out.open(cfg);
		if (out.is_open())
		{
			out << value << std::endl;
			out << msg << std::endl;

			out.close();
		}
	}
}